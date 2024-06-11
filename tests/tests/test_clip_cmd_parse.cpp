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

#include "mock_clip_notify.hpp"
#include "mock_clip_utils_arg.hpp"
#include "mock_clip_cmd_call.hpp"

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;

class ClipCmdParseTest : public Test
{
protected:
    virtual void SetUp()
    {
        ClipNotify_Mock::create();
        ClipUtilsArg_Mock::create();
        ClipCmdCall_Mock::create();
    }

    virtual void TearDown()
    {
        ClipNotify_Mock::destroy();
        ClipUtilsArg_Mock::destroy();
        ClipCmdCall_Mock::destroy();
    }
};

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__callCommandCallback)
{
    struct clip_command cmd = {};
    const struct clip_command *commands[1] = {nullptr};

    EXPECT_CALL(*ClipCmdCall_Mock::get(), clip_cmd_call_command_callback((struct clip*)123, &cmd, (char*)456, (void*)11223344))
        .Times(2);

    cmd.commands = nullptr;
    clip_cmd_parse_line((struct clip*)123, &cmd, (char*)456, (void*)11223344);

    cmd.commands = commands;
    clip_cmd_parse_line((struct clip*)123, &cmd, (char*)456, (void*)11223344);
}

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__callCommandCallback_nested)
{
    struct clip_command cmd = {};
    struct clip_command subcmd = {};
    subcmd.name = "test";
    const struct clip_command *commands[2] = {&subcmd, nullptr};
    cmd.commands = commands;
    const char *name = "test";
    const char *ret_name = "abcd";

    EXPECT_CALL(*ClipCmdCall_Mock::get(), clip_cmd_call_command_callback((struct clip*)123, &subcmd, (char*)ret_name, (void*)11223344));

    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)456))
        .WillOnce(Invoke([=](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = (char*)name;
            return (char*)ret_name;
        }));

    clip_cmd_parse_line((struct clip*)123, &cmd, (char*)456, (void*)11223344);
}

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__notifyEventHelp_notRoot)
{
    struct clip_command cmd = {};
    const struct clip_command subcmd = {};
    const struct clip_command *commands[2] = {&subcmd, nullptr};
    cmd.commands = commands;

    EXPECT_CALL(*ClipNotify_Mock::get(), clip_notify_event_help((struct clip*)123, (void*)11223344, &cmd, commands));
    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)456))
        .WillOnce(Invoke([](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = CLIP_CONFIG_HELP_COMMAND;
            return (char*)"\0";
        }));

    clip_cmd_parse_line((struct clip*)123, &cmd, (char*)456, (void*)11223344);
}

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__notifyEventHelp_root)
{
    struct clip self = {};
    const struct clip_command subcmd = {};
    const struct clip_command *commands[2] = {&subcmd, nullptr};
    self.commands = commands;

    EXPECT_CALL(*ClipNotify_Mock::get(), clip_notify_event_help(&self, (void*)11223344, nullptr, commands));
    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)456))
        .WillOnce(Invoke([](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = CLIP_CONFIG_HELP_COMMAND;
            return (char*)"\0";
        }));

    clip_cmd_parse_line(&self, nullptr, (char*)456, (void*)11223344);
}

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__notifyEventCommandNotFound_notRoot)
{
    struct clip_command cmd = {};
    struct clip_command subcmd = {};
    subcmd.name = "aaa";
    const struct clip_command *commands[2] = {&subcmd, nullptr};
    cmd.commands = commands;
    const char *name = "test";

    EXPECT_CALL(*ClipNotify_Mock::get(), clip_notify_event_command_not_found((struct clip*)123, (void*)11223344, &cmd, name));
    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)456))
        .WillOnce(Invoke([=](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = (char*)name;
            return (char*)"\0";
        }));

    clip_cmd_parse_line((struct clip*)123, &cmd, (char*)456, (void*)11223344);
}

TEST_F(ClipCmdParseTest, clip_cmd_parse_line__notifyEventCommandNotFound_root)
{
    struct clip self = {};
    struct clip_command subcmd = {};
    subcmd.name = "aaa";
    const struct clip_command *commands[2] = {&subcmd, nullptr};
    self.commands = commands;
    const char *name = "test";

    EXPECT_CALL(*ClipNotify_Mock::get(), clip_notify_event_command_not_found(&self, (void*)11223344, nullptr, name));
    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)456))
        .WillOnce(Invoke([=](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = (char*)name;
            return (char*)"\0";
        }));

    clip_cmd_parse_line(&self, nullptr, (char*)456, (void*)11223344);
}
