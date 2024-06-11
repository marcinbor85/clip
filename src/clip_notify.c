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

#include "clip.h"

void clip_notify_event_help(const struct clip *self, void *context, const struct clip_command *cmd, const struct clip_command **commands)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(commands != NULL);

    union clip_event_arg event_arg = {
        .help = {
            .cmd = cmd,
            .commands = commands,
        }
    };

    if (self->event_callback != NULL)
        self->event_callback(self, CLIP_EVENT_HELP, &event_arg, context);
}

void clip_notify_event_command_not_found(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_name)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(cmd_name != NULL);

    union clip_event_arg event_arg = {
        .command_not_found = {
            .cmd = cmd,
            .cmd_name = cmd_name,
        }
    };

    if (self->event_callback != NULL)
        self->event_callback(self, CLIP_EVENT_COMMAND_NOT_FOUND, &event_arg, context);
}

void clip_notify_event_arguments_error(const struct clip *self, void *context, const struct clip_command *cmd, clip_arg_error_t error)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(cmd != NULL);

    union clip_event_arg event_arg = {
        .arguments_error = {
            .cmd = cmd,
            .error = error,
        }
    };

    if (self->event_callback != NULL)
        self->event_callback(self, CLIP_EVENT_ARGUMENTS_ERROR, &event_arg, context);
}

void clip_notify_event_call_command_callback(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_line)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(cmd != NULL);
    CLIP_CONFIG_ASSERT(cmd_line != NULL);

    union clip_event_arg event_arg = {
        .call_command_callback = {
            .cmd = cmd,
            .cmd_line = cmd_line
        }
    };

    if (self->event_callback != NULL)
        self->event_callback(self, CLIP_EVENT_CALL_COMMAND_CALLBACK, &event_arg, context);
}
