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

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;

class ClipNotifyTest : public Test
{
protected:
    virtual void SetUp()
    {
        ClipEventCallback_Mock::create();
    }

    virtual void TearDown()
    {
        ClipEventCallback_Mock::destroy();
    }
};

TEST_F(ClipNotifyTest, clip_notify_event_help)
{
    struct clip self = {};
    self.event_callback = test_clip_event_callback;

    EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
        &self,
        CLIP_EVENT_HELP,
        IsEqualClipEventArg_Help((struct clip_command*)456, (struct clip_command **)789),
        (void*)123)
    );

    clip_notify_event_help(&self, (void*)123, (struct clip_command*)456, (const struct clip_command **)789);
}

TEST_F(ClipNotifyTest, clip_notify_event_command_not_found)
{
    struct clip self = {};
    self.event_callback = test_clip_event_callback;

    EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
        &self,
        CLIP_EVENT_COMMAND_NOT_FOUND,
        IsEqualClipEventArg_CommandNotFound((struct clip_command*)456, "test"),
        (void*)123)
    );

    clip_notify_event_command_not_found(&self, (void*)123, (struct clip_command*)456, "test");
}

TEST_F(ClipNotifyTest, clip_notify_event_arguments_error)
{
    struct clip self = {};
    self.event_callback = test_clip_event_callback;

    EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
        &self,
        CLIP_EVENT_ARGUMENTS_ERROR,
        IsEqualClipEventArg_ArgumentsError((struct clip_command*)456, (clip_arg_error_t)111),
        (void*)123)
    );

    clip_notify_event_arguments_error(&self, (void*)123, (struct clip_command*)456, (clip_arg_error_t)111);
}

TEST_F(ClipNotifyTest, clip_notify_event_call_command_callback)
{
    struct clip self = {};
    self.event_callback = test_clip_event_callback;

    EXPECT_CALL(*ClipEventCallback_Mock::get(), clip_event_callback(
        &self,
        CLIP_EVENT_CALL_COMMAND_CALLBACK,
        IsEqualClipEventArg_CallCommandCallback((struct clip_command*)456, "test"),
        (void*)123)
    );

    clip_notify_event_call_command_callback(&self, (void*)123, (struct clip_command*)456, "test");
}

TEST_F(ClipNotifyTest, clip_notify_event_help__null)
{
    struct clip self = {};

    clip_notify_event_help(&self, (void*)123, (struct clip_command*)456, (const struct clip_command **)789);
}

TEST_F(ClipNotifyTest, clip_notify_event_command_not_found__null)
{
    struct clip self = {};

    clip_notify_event_command_not_found(&self, (void*)123, (struct clip_command*)456, "test");
}

TEST_F(ClipNotifyTest, clip_notify_event_arguments_error__null)
{
    struct clip self = {};

    clip_notify_event_arguments_error(&self, (void*)123, (struct clip_command*)456, (clip_arg_error_t)111);
}

TEST_F(ClipNotifyTest, clip_notify_event_call_command_callback__null)
{
    struct clip self = {};

    clip_notify_event_call_command_callback(&self, (void*)123, (struct clip_command*)456, "test");
}
