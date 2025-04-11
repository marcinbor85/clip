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

#ifndef CLIP_TYPES_H
#define CLIP_TYPES_H

#include <stdint.h>
#include <stddef.h>

///< forward declaration of main clip structure
struct clip;

///< forward declaration for clip command structure
struct clip_command;

///< type alias for hex array argument value type
typedef uint8_t* clip_hexarray_t;

///< enum contains event types
typedef enum {
    CLIP_EVENT_HELP,                    ///< event for notifing command HELP call
    CLIP_EVENT_COMMAND_NOT_FOUND,       ///< event for notifing unsupported command
    CLIP_EVENT_ARGUMENTS_ERROR,         ///< event for notifing command arguments error
    CLIP_EVENT_CALL_COMMAND_CALLBACK,   ///< event for notifing command call
} clip_event_t;

///< enum contains argument value types
typedef enum {
    CLIP_ARG_TYPE_STRING,               ///< string (without parsing)
    CLIP_ARG_TYPE_BOOL,                 ///< boolean (bool)
    CLIP_ARG_TYPE_INT,                  ///< signed integer (int32_t)
    CLIP_ARG_TYPE_UINT,                 ///< unsigned integer (uint32_t)
    CLIP_ARG_TYPE_FLOAT,                ///< float number (float)
    CLIP_ARG_TYPE_HEXARRAY,             ///< array of bytes (ascii hex encoded)
} clip_arg_type_t;

///< enum contains argument error type
typedef enum {
    CLIP_ARG_ERROR_NO_ERROR,                ///< no error
    CLIP_ARG_ERROR_NOT_ENOUGH_ARGUMENTS,    ///< not enough arguments
    CLIP_ARG_ERROR_PARSE_BOOL,              ///< bool parsing error
    CLIP_ARG_ERROR_PARSE_INT,               ///< signed integer parsing error
    CLIP_ARG_ERROR_PARSE_UINT,              ///< unsigned integer parsing error
    CLIP_ARG_ERROR_PARSE_FLOAT,             ///< float number parsing error
    CLIP_ARG_ERROR_PARSE_HEXARRAY,          ///< ascii hex array parsing error
} clip_arg_error_t;

///< structure contains argument descriptor (may by const and static)
struct clip_arg {
    const char *name;                   ///< argument name
    const char *description;            ///< argument description (not used internally)
    clip_arg_type_t type;               ///< argument value type
    bool optional;                      ///< optional argument flag
};

///< structure contains parsed argument value in union format
struct clip_arg_value {
    clip_arg_type_t type;               ///< argument value type
    union {
        char *val_str;                  ///< pointer to value string (for CLIP_ARG_TYPE_STRING)
        bool val_bool;                  ///< bool value (for CLIP_ARG_TYPE_BOOL)
        int32_t val_int;                ///< signed integer value (for CLIP_ARG_TYPE_INT)
        uint32_t val_uint;              ///< unsigned integer value (for CLIP_ARG_TYPE_UINT)
        float val_float;                ///< float number value (for CLIP_ARG_TYPE_FLOAT)
        clip_hexarray_t val_hexarray;   ///< array of bytes (for CLIP_ARG_TYPE_HEXARRAY)
    };
};

///< union contains notified event arguments
union clip_event_arg {
    struct {
        const struct clip_command *cmd;             ///< pointer to command which invoke HELP (may be NULL for root)
        const struct clip_command **commands;       ///< list of supported subcommands (last item is NULL)
    } help;                                         ///< structure with arguments for CLIP_EVENT_HELP
    struct {
        const struct clip_command *cmd;             ///< pointer to command which refused command (may be NULL for root)
        const char *cmd_name;                       ///< command name which is not supported
    } command_not_found;                            ///< structure with arguments for CLIP_EVENT_COMMAND_NOT_FOUND
    struct {
        const struct clip_command *cmd;             ///< pointer to command which refused arguments
        clip_arg_error_t error;                     ///< argument parsing error type
    } arguments_error;                              ///< structure with arguments for CLIP_EVENT_ARGUMENTS_ERROR
    struct {
        const struct clip_command *cmd;             ///< pointer to command which was called
        const char *cmd_line;                       ///< command line used as a command arguments (not parsed yet)
    } call_command_callback;                        ///< structure with arguments for CLIP_EVENT_CALL_COMMAND_CALLBACK
};

///< alias for function pointer with command call callback (fired on successully parsed arguments)
typedef void (*clip_command_callback_t)(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context);

///< alias for function pointer with event call callback (fired on events)
typedef void (*clip_event_callback_t)(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context);

///< structure contains root clip handler descriptor (may be const and static)
struct clip {
    void *context;                          ///< generic pointer used as global context (accessible in all callbacks)
    clip_event_callback_t event_callback;   ///< pointer to event callback function
    const struct clip_command **commands;   ///< list of root commands (last item is NULL)
};

///< structure contains command/subcommand descriptor (may be const and static)
struct clip_command {
    const char *name;                       ///< command name
    const char *description;                ///< optional command description (not used internally)

    clip_command_callback_t callback;       ///< command call callback function pointer
    const struct clip_command **commands;   ///< list of optional subcommands (may be NULL or last item is NULL)
    const struct clip_arg **args;           ///< list of optional arguments (may be NULL or last item is NULL)
};

#endif /* CLIP_TYPES_H */
