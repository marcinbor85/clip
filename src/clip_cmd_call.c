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

void clip_cmd_call_command_callback(const struct clip *self, const struct clip_command *cmd, char *cmd_line, void *context)
{
    CLIP_CONFIG_ASSERT(self != NULL);
    CLIP_CONFIG_ASSERT(cmd != NULL);
    CLIP_CONFIG_ASSERT(cmd_line != NULL);

    size_t argc = 0;
    char *arg = cmd_line;
    struct clip_arg_value argv[CLIP_CONFIG_ARGS_MAX_NUM] = {0};
    struct clip_arg_value *av = NULL;

    clip_notify_event_call_command_callback(self, context, cmd, cmd_line);

    clip_arg_error_t error = CLIP_ARG_ERROR_NO_ERROR;
    bool no_more_required_args = false;
    while (argc < (sizeof(argv) / sizeof(argv[0]))) {
        cmd_line = clip_utils_arg_get_first(&arg, cmd_line);
        if (*arg == '\0')
            break;

        av = &argv[argc];
        av->type = CLIP_ARG_TYPE_STRING;
        av->val_str = arg;

        const struct clip_arg *ca = NULL;
        if (no_more_required_args == false && cmd->args != NULL) {
            ca = cmd->args[argc];
            if (ca != NULL) {
                switch (ca->type) {
                case CLIP_ARG_TYPE_BOOL:
                    if (clip_utils_parse_bool(av, arg) == false)
                        error = CLIP_ARG_ERROR_PARSE_BOOL;
                    break;
                    
                case CLIP_ARG_TYPE_INT:
                    if (clip_utils_parse_int(av, arg) == false)
                        error = CLIP_ARG_ERROR_PARSE_INT;
                    break;

                case CLIP_ARG_TYPE_UINT:
                    if (clip_utils_parse_uint(av, arg) == false)
                        error = CLIP_ARG_ERROR_PARSE_UINT;
                    break;

                case CLIP_ARG_TYPE_FLOAT:
                    if (clip_utils_parse_float(av, arg) == false)
                        error = CLIP_ARG_ERROR_PARSE_FLOAT;
                    break;

                case CLIP_ARG_TYPE_HEXARRAY:
                    if (clip_utils_parse_hexarray(av, arg) == false)
                        error = CLIP_ARG_ERROR_PARSE_HEXARRAY;
                    break;

                case CLIP_ARG_TYPE_STRING:
                default:
                    av->type = CLIP_ARG_TYPE_STRING;
                    av->val_str = arg;
                    break;
                }
            } else {
                no_more_required_args = true;
            }
        }

        if (error != CLIP_ARG_ERROR_NO_ERROR)
            break;

        argc++;
    }

    if (error == CLIP_ARG_ERROR_NO_ERROR) {
        size_t required_args_count = 0;
        const struct clip_arg* *args = cmd->args;
        if (args != NULL) {
            while (*args != NULL) {
                required_args_count++;
                args++;
            }
        }
        if (required_args_count > argc) {
            error = CLIP_ARG_ERROR_NOT_ENOUGH_ARGUMENTS;
        }
    }

    if (error != CLIP_ARG_ERROR_NO_ERROR) {
        clip_notify_event_arguments_error(self, context, cmd, error);
    } else {
        if (cmd->callback != NULL)
            cmd->callback(self, cmd, argc, argv, context);
    }
}
