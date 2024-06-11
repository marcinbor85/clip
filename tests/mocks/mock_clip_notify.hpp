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

struct ClipNotify_Mock : public Mock<ClipNotify_Mock>
{
    MOCK_METHOD(void, clip_notify_event_help, (const struct clip *self, void *context, const struct clip_command *cmd, const struct clip_command **commands), ());
    MOCK_METHOD(void, clip_notify_event_command_not_found, (const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_name), ());
    MOCK_METHOD(void, clip_notify_event_arguments_error, (const struct clip *self, void *context, const struct clip_command *cmd, clip_arg_error_t error), ());
    MOCK_METHOD(void, clip_notify_event_call_command_callback, (const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_line), ());
};

extern "C" {

void clip_notify_event_help(const struct clip *self, void *context, const struct clip_command *cmd, const struct clip_command **commands)
{
    ClipNotify_Mock::get()->clip_notify_event_help(self, context, cmd, commands);
}

void clip_notify_event_command_not_found(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_name)
{
    ClipNotify_Mock::get()->clip_notify_event_command_not_found(self, context, cmd, cmd_name);
}

void clip_notify_event_arguments_error(const struct clip *self, void *context, const struct clip_command *cmd, clip_arg_error_t error)
{
    ClipNotify_Mock::get()->clip_notify_event_arguments_error(self, context, cmd, error);
}

void clip_notify_event_call_command_callback(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_line)
{
    ClipNotify_Mock::get()->clip_notify_event_call_command_callback(self, context, cmd, cmd_line);
}

}
