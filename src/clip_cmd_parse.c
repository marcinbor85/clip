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

#include <string.h>

void clip_cmd_parse_line(const struct clip *self, const struct clip_command *cmd, char *cmd_line, void *context)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(cmd_line != NULL);

    if (cmd != NULL && (cmd->commands == NULL || *cmd->commands == NULL)) {
        clip_cmd_call_command_callback(self, cmd, cmd_line, context);
        return;
    }

    const struct clip_command* *commands = (cmd != NULL) ? cmd->commands : self->commands;

    char *cmd_name;
    cmd_line = clip_utils_arg_get_first(&cmd_name, cmd_line);

    if (strcmp(cmd_name, CLIP_CONFIG_HELP_COMMAND) == 0) {
        clip_notify_event_help(self, context, cmd, commands);
        return;
    }

    while (*commands != NULL) {
        if (strcmp(cmd_name, (*commands)->name) == 0) {
            clip_cmd_parse_line(self, *commands, cmd_line, context);
            return;
        }
        commands++;
    }

    clip_notify_event_command_not_found(self, context, cmd, cmd_name);
}
