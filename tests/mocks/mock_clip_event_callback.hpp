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

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clip.h"

#include "mock.hpp"

struct ClipEventCallback_Mock : public Mock<ClipEventCallback_Mock>
{
    MOCK_METHOD(void, clip_event_callback, (const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context), ());
};

MATCHER_P2(IsEqualClipEventArg_Help, cmd, commands, "Equality matcher for clip_event_arg.help") {
    return std::tie(arg->help.cmd, arg->help.commands) == std::tie(cmd, commands);
}

MATCHER_P2(IsEqualClipEventArg_CommandNotFound, cmd, cmd_name, "Equality matcher for clip_event_arg.command_not_found") {
    return std::tie(arg->command_not_found.cmd, arg->command_not_found.cmd_name) == std::tie(cmd, cmd_name);
}

MATCHER_P2(IsEqualClipEventArg_ArgumentsError, cmd, error, "Equality matcher for clip_event_arg.arguments_error") {
    return std::tie(arg->arguments_error.cmd, arg->arguments_error.error) == std::tie(cmd, error);
}

MATCHER_P2(IsEqualClipEventArg_CallCommandCallback, cmd, cmd_line, "Equality matcher for clip_event_arg.call_command_callback") {
    return std::tie(arg->call_command_callback.cmd, arg->call_command_callback.cmd_line) == std::tie(cmd, cmd_line);
}

extern "C" {

void test_clip_event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context)
{
    return ClipEventCallback_Mock::get()->clip_event_callback(self, event, event_arg, context);
}

}
