/*
MIT License

Copyright (c) 2024 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clip.h"

#include "mock_clip_event_callback.hpp"
#include "mock_clip_command_callback.hpp"

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Args;
using ::testing::Field;
using ::testing::FloatEq;
using ::testing::StrEq;
using ::testing::Eq;
using ::testing::AllOf;

class ClipE2ETest : public Test
{
protected:
    virtual void SetUp()
    {
        ClipEventCallback_Mock::create();
        ClipCommandCallback_Mock::create();
    }

    virtual void TearDown()
    {
        ClipEventCallback_Mock::destroy();
        ClipCommandCallback_Mock::destroy();
    }
};

extern "C" const struct clip g_clip;
extern "C" const struct clip_commands g_cmd1;
extern "C" const struct clip_commands g_cmd2;

MATCHER_P(IsCommand_Name, cmd_name, "Equality matcher for command name")
{
    return strcmp(arg->name, cmd_name) == 0;
}

MATCHER_P2(IsEventArg_CallCommandCallback, cmd_name, cmd_line, "Equality matcher for event call_command_callback")
{
    return strcmp(arg->call_command_callback.cmd->name, cmd_name) == 0 && strcmp(arg->call_command_callback.cmd_line, cmd_line) == 0;
}

MATCHER_P2(IsEventArg_CommandNotFound, cmd_name, cmd_not_found_name, "Equality matcher for event command_not_found")
{
    bool cmd_name_ok = false;

    if (cmd_name == nullptr) {
        cmd_name_ok = arg->command_not_found.cmd == nullptr;
    } else {
        cmd_name_ok = strcmp(arg->command_not_found.cmd->name, cmd_name) == 0;
    }

    return cmd_name_ok && strcmp(arg->command_not_found.cmd_name, cmd_not_found_name) == 0;
}

MATCHER_P2(IsEventArg_ArgumentsError, cmd_name, error, "Equality matcher for event arguments_error")
{
    bool cmd_name_ok = strcmp(arg->arguments_error.cmd->name, cmd_name) == 0;
    bool error_type_ok = arg->arguments_error.error == error;
    return cmd_name_ok && error_type_ok;
}

MATCHER_P2(IsEventArg_Help, cmd_name, commands_names_list, "Equality matcher for event help")
{
    bool cmd_name_ok = false;
    bool commands_names_list_ok = true;

    if (cmd_name == nullptr) {
        cmd_name_ok = arg->help.cmd == nullptr;
    } else {
        cmd_name_ok = strcmp(arg->help.cmd->name, cmd_name) == 0;
    }

    size_t commands_num = 0;
    const clip_command **commands = arg->help.commands;
    while (*commands != nullptr) {
        if (commands_num < commands_names_list.size()) {
            if (strcmp(commands_names_list.at(commands_num).c_str(), (*commands)->name) != 0) {
                commands_names_list_ok = false;
            }
        }
        commands++;
        commands_num++;
    }

    if (commands_num != commands_names_list.size()) {
        commands_names_list_ok = false;
    }

    return cmd_name_ok && commands_names_list_ok;
}

using CmdArg = std::variant<const char*, bool, int32_t, uint32_t, float, std::vector<uint8_t>>;

MATCHER_P(IsArg, cmd_arg, "Equality matcher for command arguments")
{
    if (auto *p = std::get_if<const char*>(&cmd_arg)) {
        return arg.type == CLIP_ARG_TYPE_STRING && strcmp(arg.val_str, *p) == 0;

    } else if (auto *p = std::get_if<bool>(&cmd_arg)) {
        return arg.type == CLIP_ARG_TYPE_BOOL && arg.val_bool == *p;

    } else if (auto *p = std::get_if<int32_t>(&cmd_arg)) {
        return arg.type == CLIP_ARG_TYPE_INT && arg.val_int == *p;

    } else if (auto *p = std::get_if<uint32_t>(&cmd_arg)) {
        return arg.type == CLIP_ARG_TYPE_UINT && arg.val_uint == *p;

    } else if (auto *p = std::get_if<float>(&cmd_arg)) {
        return arg.type == CLIP_ARG_TYPE_FLOAT && arg.val_float == *p;
        
    } else if (auto *p = std::get_if<std::vector<uint8_t>>(&cmd_arg)) {
        uint8_t *data = NULL;
        size_t size = clip_utils_arg_unpack_hexarray(&data, arg.val_hexarray);
        return arg.type == CLIP_ARG_TYPE_HEXARRAY && memcmp(p->data(), data, size) == 0;

    } else {
        return false;
    }
}

TEST_F(ClipE2ETest, e2e__cmdArgsParsing)
{
    char buf[128];
    void *callCtx = (void*)12345678;

    std::vector<std::tuple<std::string, std::string, std::string, std::vector<CmdArg>>> testCases = {
        {
            "cmd1 abc a1 a11 arg1 arg2 \"hello world\" 3",
            "a11",
            "arg1 arg2 \"hello world\" 3",
            { CmdArg {"arg1"}, CmdArg {"arg2"}, CmdArg {"hello world"}, CmdArg {"3"} }
        },
        {
            "cmd2 abc test 0 -1 2 1.23 DEADBEEF",
            "abc",
            "test 0 -1 2 1.23 DEADBEEF",
            { CmdArg {"test"}, CmdArg {false}, CmdArg {(int32_t)-1}, CmdArg {(uint32_t)2}, CmdArg {(float)1.23}, CmdArg {std::vector<uint8_t>{0xDE, 0xAD, 0xBE, 0xEF}} }
        },
        {
            "cmd2 xyz -1 2 1.23 DEADBEEF",
            "xyz",
            "-1 2 1.23 DEADBEEF",
            { CmdArg {(int32_t)-1}, CmdArg {"2"}, CmdArg {"1.23"}, CmdArg {"DEADBEEF"} }
        },
        {
            "cmd1 abc a1 a11 ?",
            "a11",
            "?",
            { CmdArg {"?"} }
        },
    };

    for (auto ts : testCases) {

        auto cmd_line = std::get<0>(ts).c_str();
        auto cmd_name = std::get<1>(ts).c_str();
        auto args_line = std::get<2>(ts).c_str();
        auto args = std::get<3>(ts);

        std::vector<IsArgMatcherP<CmdArg>> argsMatchers;
        for (auto arg : args)
            argsMatchers.push_back(IsArg(arg));
            
        EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
            &g_clip,
            CLIP_EVENT_CALL_COMMAND_CALLBACK,
            IsEventArg_CallCommandCallback(cmd_name, args_line),
            callCtx)
        );
        EXPECT_CALL(*ClipCommandCallback_Mock::get(), clip_command_callback(
            &g_clip,
            IsCommand_Name(cmd_name),
            _,
            _,
            callCtx)
        ).With(Args<3, 2>(ElementsAreArray(argsMatchers)));

        strcpy(buf, cmd_line);
        clip_cmd_parse_line(&g_clip, NULL, buf, callCtx);
    }
}

TEST_F(ClipE2ETest, e2e__cmdArgsParsing__noCallback)
{
    char buf[128];
    void *callCtx = (void*)12345678;

    auto cmd_line = "cmd1 no abc test";
    auto cmd_name = "no";
    auto args_line = "abc test";

    EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
        &g_clip,
        CLIP_EVENT_CALL_COMMAND_CALLBACK,
        IsEventArg_CallCommandCallback(cmd_name, args_line),
        callCtx)
    );

    strcpy(buf, cmd_line);
    clip_cmd_parse_line(&g_clip, NULL, buf, callCtx);
}

TEST_F(ClipE2ETest, e2e__cmdHelp)
{
    char buf[128];
    void *callCtx = (void*)12345678;

    std::vector<std::tuple<std::string, std::optional<std::string>, std::vector<std::string>>> testCases = {
        {
            "?",
            std::nullopt,
            { "cmd1", "cmd2" }
        },
        {
            "cmd1 ?",
            "cmd1",
            { "abc", "def", "xyz", "no" }
        },
        {
            "cmd1 abc ?",
            "abc",
            { "a1", "a2", "a3" }
        },
        {
            "cmd1 abc a1 ?",
            "a1",
            { "a11", "a12" }
        },
        {
            "cmd2 ?",
            "cmd2",
            { "abc", "xyz" }
        },
    };

    for (auto ts : testCases) {

        auto cmd_line = std::get<0>(ts).c_str();
        auto cmd_name = std::get<1>(ts).has_value() ? std::get<1>(ts).value().c_str() : nullptr;
        auto commands_names_list = std::get<2>(ts);

        EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
            &g_clip,
            CLIP_EVENT_HELP,
            IsEventArg_Help(cmd_name, commands_names_list),
            callCtx)
        );

        strcpy(buf, cmd_line);
        clip_cmd_parse_line(&g_clip, NULL, buf, callCtx);
    }
}

TEST_F(ClipE2ETest, e2e__cmdNotFound)
{
    char buf[128];
    void *callCtx = (void*)12345678;

    std::vector<std::tuple<std::string, std::optional<std::string>, std::string>> testCases = {
        {
            "cmd3",
            std::nullopt,
            "cmd3"
        },
        {
            "cmd11",
            std::nullopt,
            "cmd11"
        },
        {
            "cmd1 qwerty",
            "cmd1",
            "qwerty"
        },
        {
            "cmd1 abc a1 a13",
            "a1",
            "a13"
        },
        {
            "cmd2 abcd",
            "cmd2",
            "abcd"
        }
    };

    for (auto ts : testCases) {

        auto cmd_line = std::get<0>(ts).c_str();
        auto cmd_name = std::get<1>(ts).has_value() ? std::get<1>(ts).value().c_str() : nullptr;
        auto cmd_not_found_name = std::get<2>(ts).c_str();

        EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
            &g_clip,
            CLIP_EVENT_COMMAND_NOT_FOUND,
            IsEventArg_CommandNotFound(cmd_name, cmd_not_found_name),
            callCtx)
        );

        strcpy(buf, cmd_line);
        clip_cmd_parse_line(&g_clip, NULL, buf, callCtx);
    }
}

TEST_F(ClipE2ETest, e2e__cmdArgsError)
{
    char buf[128];
    void *callCtx = (void*)12345678;

    std::vector<std::tuple<std::string, std::string, std::string, clip_arg_error_t>> testCases = {
        {
            "cmd2 xyz test",
            "xyz",
            "test",
            CLIP_ARG_ERROR_PARSE_INT,
        },
        {
            "cmd2 xyz 1.0 test",
            "xyz",
            "1.0 test",
            CLIP_ARG_ERROR_PARSE_INT,
        },
        {
            "cmd2 abc test x -1 2 1.23 DEADBEEF",
            "abc",
            "test x -1 2 1.23 DEADBEEF",
            CLIP_ARG_ERROR_PARSE_BOOL,
        },
        {
            "cmd2 abc test 0 -1.0 2 1.23 DEADBEEF",
            "abc",
            "test 0 -1.0 2 1.23 DEADBEEF",
            CLIP_ARG_ERROR_PARSE_INT,
        },
        {
            "cmd2 abc test 1 -1 -2 1.23 DEADBEEF",
            "abc",
            "test 1 -1 -2 1.23 DEADBEEF",
            CLIP_ARG_ERROR_PARSE_UINT,
        },
        {
            "cmd2 abc test 0 -1 2 1..23 DEADBEEF",
            "abc",
            "test 0 -1 2 1..23 DEADBEEF",
            CLIP_ARG_ERROR_PARSE_FLOAT,
        },
        {
            "cmd2 abc test 0 -1 2 1.23 DEADBEEF0",
            "abc",
            "test 0 -1 2 1.23 DEADBEEF0",
            CLIP_ARG_ERROR_PARSE_HEXARRAY,
        },
        {
            "cmd2 abc test 0 -1 2 1.23",
            "abc",
            "test 0 -1 2 1.23",
            CLIP_ARG_ERROR_NOT_ENOUGH_ARGUMENTS,
        }
    };

    for (auto ts : testCases) {

        auto cmd_line = std::get<0>(ts).c_str();
        auto cmd_name = std::get<1>(ts).c_str();
        auto args_line = std::get<2>(ts).c_str();
        auto error_type = std::get<3>(ts);

        EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
            &g_clip,
            CLIP_EVENT_CALL_COMMAND_CALLBACK,
            IsEventArg_CallCommandCallback(cmd_name, args_line),
            callCtx)
        );
        EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
            &g_clip,
            CLIP_EVENT_ARGUMENTS_ERROR,
            IsEventArg_ArgumentsError(cmd_name, error_type),
            callCtx)
        );

        strcpy(buf, cmd_line);
        clip_cmd_parse_line(&g_clip, NULL, buf, callCtx);
    }
}

TEST_F(ClipE2ETest, e2e__context)
{
    EXPECT_EQ(g_clip.context, (void*)11223344);
}

TEST_F(ClipE2ETest, e2e__rootCommands)
{
    EXPECT_EQ(g_clip.commands[0], (const clip_command*)&g_cmd1);
    EXPECT_EQ(g_clip.commands[1], (const clip_command*)&g_cmd2);
    EXPECT_EQ(g_clip.commands[2], nullptr);
}

TEST_F(ClipE2ETest, e2e__eventCallback)
{
    EXPECT_EQ(g_clip.event_callback, test_clip_event_callback);
}
