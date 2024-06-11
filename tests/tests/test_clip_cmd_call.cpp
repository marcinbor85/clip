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
#include "mock_clip_utils_parse.hpp"
#include "mock_clip_command_callback.hpp"

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;

class ClipCmdCallTest : public Test
{
protected:
    virtual void SetUp()
    {
        ClipNotify_Mock::create();
        ClipUtilsArg_Mock::create();
        ClipUtilsParse_Mock::create();
        ClipCommandCallback_Mock::create();
    }

    virtual void TearDown()
    {
        ClipNotify_Mock::destroy();
        ClipUtilsArg_Mock::destroy();
        ClipUtilsParse_Mock::destroy();
        ClipCommandCallback_Mock::destroy();
    }
};

TEST_F(ClipCmdCallTest, clip_cmd_call_command_callback__noArgs)
{
    struct clip_command cmd = {};
    cmd.callback = test_clip_command_callback;
    const char *line = "test";

    EXPECT_CALL(*ClipCommandCallback_Mock::get(), clip_command_callback((struct clip*)123, &cmd, 0, _, (void*)11223344));
    EXPECT_CALL(*ClipNotify_Mock::get(), clip_notify_event_call_command_callback((struct clip*)123, (void*)11223344, &cmd, line));
    EXPECT_CALL(*ClipUtilsArg_Mock::get(), clip_utils_arg_get_first(_, (char*)line))
        .WillOnce(Invoke([](char **cmd_name, char *cmd_line)->char* {
            *cmd_name = (char*)"\0";
            return (char*)"\0";
        }));
    
    clip_cmd_call_command_callback((struct clip*)123, &cmd, (char*)line, (void*)11223344);
}
