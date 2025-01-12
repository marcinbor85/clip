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

#ifndef CLIP_DEFS_H
#define CLIP_DEFS_H

#include "clip_types.h"

///< internal macro for defining subcommands
#define _CLIP_DEF_SUBCOMMANDS()\
    .commands = (const struct clip_command**)&(const struct clip_command*[]) {\

///< internal macro for defining arguments
#define _CLIP_DEF_ARGUMENTS()\
    .args = (const struct clip_arg**)&(const struct clip_arg*[]) {\

///< internal macro for defining commands
#define _CLIP_DEF_COMMAND(cmd_name, cmd_description, callback_func)\
    .name = cmd_name,\
    .description = cmd_description,\
    .callback = callback_func,\
    _CLIP_DEF_SUBCOMMANDS()\

///< public macro for defining root command
#define CLIP_DEF_ROOT_COMMAND(var_name, cmd_name, cmd_description, callback_func)\
const struct clip_command var_name = {\
    _CLIP_DEF_COMMAND(cmd_name, cmd_description, callback_func)\

///< public macro for finishing root command definition
#define CLIP_DEF_ROOT_COMMAND_END_WITH_ARGS()\
        NULL,\
    }\
};\

///< public macro for finishing root command without args definition
#define CLIP_DEF_ROOT_COMMAND_END()\
        NULL,\
    },\
    .args = NULL\
};\

///< public macro for defining command
#define CLIP_DEF_COMMAND(cmd_name, cmd_description, callback_func)\
    &(const struct clip_command) {\
        _CLIP_DEF_COMMAND(cmd_name, cmd_description, callback_func)

///< public macro for defining argument
#define CLIP_DEF_ARGUMENT(arg_name, arg_description, arg_type)\
    &(const struct clip_arg) {\
        .name = arg_name,\
        .description = arg_description,\
        .type = arg_type,\
    },\

///< public macro for finishing command definition
#define CLIP_DEF_COMMAND_END_WITH_ARGS()\
            NULL,\
        }\
    },\

///< public macro for finishing command without args definition
#define CLIP_DEF_COMMAND_END()\
            NULL,\
        },\
        .args = NULL\
    },\

///< public macro for defining arguments for subcommand
#define CLIP_DEF_WITH_ARGS()\
            NULL,\
        },\
        _CLIP_DEF_ARGUMENTS()\

///< public macro for defining root commands handler
#define CLIP_DEF_ROOT(var_name, usr_context, evt_callback)\
const struct clip var_name = {\
    .context = usr_context,\
    .event_callback = evt_callback,\
    .commands = (const struct clip_command**)&(const struct clip_command*[]) {\

///< public macro for adding subcommands to root
#define CLIP_DEF_ADD_ROOT_COMMAND(var_name)\
    var_name,\

///< public macro for finishing root definition
#define CLIP_DEF_ROOT_END()\
        NULL,\
    }\
};\

#endif /* CLIP_DEFS_H */
