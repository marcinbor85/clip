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

#ifndef CLIP_H
#define CLIP_H

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include "clip_types.h"
#include "clip_config.h"
#include "clip_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief           Main function for parsing command line.
 *                  This function can be called resursively internally.
 *                  First invoke of this function (from application context) should use NULL as a cmd pointer.
 *                  Internally this function will find command and its subcommands, parse arguments and fire events.
 * @param[in]       self
 *                  Pointer to main clip root handler.
 * @param[in]       cmd
 *                  Pointer to command which is called (must be NULL for root call).
 * @param[in/out]   cmd_line
 *                  Input command line which contains commands/subcommands and their arguments.
 *                  Data pointed by this pointer will be changed during function call.
 * @param[in]       context
 *                  Generic pointer which will be passed to events or callbacks.
*/
void clip_cmd_parse_line(const struct clip *self, const struct clip_command *cmd, char *cmd_line, void *context);

/**
 * @brief           Function called from "clip_cmd_parse_line" function.
 *                  It performs the last stage of parsing command.
 *                  At this stage command has been found already, and the arguments need to be parsed.
 *                  As a result of calling this function, user can expect such events:
 *                  1. CLIP_EVENT_CALL_COMMAND_CALLBACK - always
 *                  2. CLIP_EVENT_ARGUMENTS_ERROR - in case of arguments error
 *                  3. specific command callback - in case of arguments parsed successfully
 * @param[in]       self
 *                  Pointer to main clip root handler.
 * @param[in]       cmd
 *                  Pointer to command which is called.
 * @param[in/out]   cmd_line
 *                  Part of the input command line which contains command arguments.
 *                  Data pointed by this pointer will be changed during function call.
 * @param[in]       context
 *                  Generic pointer which will be passed to events or callbacks.
*/
void clip_cmd_call_command_callback(const struct clip *self, const struct clip_command *cmd, char *cmd_line, void *context);

/**
 * @brief           Function used to get first argument from input command line.
 *                  Input command line must be mutable, it will be modified after call this function.
 *                  Function support quotemarks, whitespaces and escape chars for arguments.
 *                  See unit tests for more details and examples.
 * @param[out]      arg
 *                  Pointer where pointer to argument will be stored.
 *                  It will point the memory block which is a part of input command line.
 * @param[in]       cmd_line
 *                  Pointer to input command line contains commands and arguments.
 *                  Data will be modified (mainly zero bytes added in proper places).
 * @return          Pointer to next argument (followed part of command line which was not modified yet).
*/
char* clip_utils_arg_get_first(char **arg, char *cmd_line);

/**
 * @brief           Function used to convert argument type enum to string.
 * @param[in]       type
 *                  Enum with argument value type to convert.
 * @return          Const pointer to enum value type string (const and static).
*/
const char* clip_utils_arg_get_type_string(clip_arg_type_t type);

/**
 * @brief           Function used to convert argument error enum to string.
 * @param[in]       type
 *                  Enum with argument error type to convert.
 * @return          Const pointer to enum value error string (const and static).
*/
const char* clip_utils_arg_get_error_string(clip_arg_error_t error);

/**
 * @brief           Helper function used to perform operation on buffer size and lengths.
 *                  Its useful for calculating length of resulting buffer using snprintf calls.
 * @param[in]       buf
 *                  Pointer to buffer.
 * @param[in/out]   buf_size
 *                  Current buffer size (how many bytes are still inside buffer).
 *                  This buffer size will be decreased by "size" until reach zero.
 * @param[in/out]   out_size
 *                  Current output size (total number of bytes processed by function).
 *                  This output size will be increased by "size".
 * @param[in]       size
 *                  Number of bytes to process.
 * @return          Pointer to buffer after process "size" bytes or pointer to '\0'.
*/
char* clip_utils_arg_update_buf(char *buf, size_t *buf_size, size_t *out_size, size_t size);

/**
 * @brief           Helper function for generate command usage string.
 *                  It bases on command and its arguments descriptors.
 *                  This is only internal implementation which works out-of-the-box.
 *                  User can implement his own usage string for commands.
 * @param[out]      buf
 *                  Pointer to buffer where usage string will be stored.
 * @param[in]       buf_size
 *                  How many chars can be stored in output buffer.
 * @param[in]       cmd
 *                  Pointer to command for which we want to generate usage string.
 * @return          How many chars was stored in output buffer (without zero byte at the end).
*/
size_t clip_utils_arg_get_command_usage_string(char *buf, size_t buf_size, const struct clip_command *cmd);

/**
 * @brief           Function used to unpack hexarray argument value.
 *                  For avoid dynamic allocation, hex arrays are stored in place in command line input.
 *                  And also for further optimalisation, there is no "hex array size" in argument value.
 *                  Hex array is encoded as LV (Length Value) binary in command line buffer.
 *                  So for using hex_array argument value, user needs to call this function to get
 *                  raw binary data pointer and the raw data size.
 *                  This function doesnt perform any parsing. It only decode LV values and return them to user.
 * @param[in/out]   data
 *                  Pointer where raw data pointer will be stored.
 * @param[in]       hex_array
 *                  Pointer where LV encoded data are stored.
 * @return          Number of bytes pointed by "hex_array".
*/
size_t clip_utils_arg_unpack_hexarray(uint8_t **data, clip_hexarray_t hex_array);

/**
 * @brief           Function used to notify CLIP_EVENT_HELP event.
 *                  Its called internally by "clip_cmd_parse_line" function.
 * @param[in]       context
 *                  Generic pointer which will be passed to event callbacks.
 * @param[in]       cmd
 *                  Pointer to command which will be placed in event arguments.
 * @param[in]       commands
 *                  List of commands which will be placed in event arguments.
*/
void clip_notify_event_help(const struct clip *self, void *context, const struct clip_command *cmd, const struct clip_command **commands);

/**
 * @brief           Function used to notify CLIP_EVENT_COMMAND_NOT_FOUND event.
 *                  Its called internally by "clip_cmd_parse_line" function.
 * @param[in]       context
 *                  Generic pointer which will be passed to event callbacks.
 * @param[in]       cmd
 *                  Pointer to command which will be placed in event arguments.
 * @param[in]       cmd_name
 *                  Command name which was not found and will be places in event arguments.
*/
void clip_notify_event_command_not_found(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_name);

/**
 * @brief           Function used to notify CLIP_EVENT_ARGUMENTS_ERROR event.
 *                  Its called internally by "clip_cmd_parse_line" function.
 * @param[in]       context
 *                  Generic pointer which will be passed to event callbacks.
 * @param[in]       cmd
 *                  Pointer to command which will be placed in event arguments.
 * @param[in]       error
 *                  Type of argument error which will be placed in event arguments.
*/
void clip_notify_event_arguments_error(const struct clip *self, void *context, const struct clip_command *cmd, clip_arg_error_t error);

/**
 * @brief           Function used to notify CLIP_EVENT_CALL_COMMAND_CALLBACK event.
 *                  Its called internally by "clip_cmd_parse_line" function.
 * @param[in]       context
 *                  Generic pointer which will be passed to event callbacks.
 * @param[in]       cmd
 *                  Pointer to command which will be placed in event arguments.
 * @param[in]       cmd_line
 *                  Command line with arguments which will be places in event arguments.
*/
void clip_notify_event_call_command_callback(const struct clip *self, void *context, const struct clip_command *cmd, const char *cmd_line);

/**
 * @brief           Function used to convert single nibble byte (0-15) to char ('0'-'9', 'A'-'F')
 *                  It uses upper-case characters for encoding.
 * @param[out]      ch
 *                  Pointer to char where the converting result will be stored.
 * @param[in]       n
 *                  Nibble byte to convert to char
 * @return          Converting status. true - success, false - error
*/
bool clip_utils_hex_nibble_to_char(char *ch, const uint8_t n);

/**
 * @brief           Function used to convert single nibble char ('0'-'9', 'A'-'F') to nibble byte (0-15)
 *                  It supports lower- and upper-case characters.
 * @param[out]      n
 *                  Pointer to nibble byte where the converting result will be stored.
 * @param[in]       ch
 *                  Nibble chat to convert to byte
 * @return          Converting status. true - success, false - error
*/
bool clip_utils_hex_char_to_nibble(uint8_t *n, const char ch);

/**
 * @brief           Function used to convert binary buffer to ascii hex array.
 *                  It uses upper-case characters for encoding.
 *                  User needs to be sure that the "hex" buffer has enough space to store all input "buf".
 *                  Function uses 2 chars for every 1 input byte.
 *                  Encoded string will not be zero-ended.
 * @param[out]      hex
 *                  Pointer where encoded ascii hex array will be stored.
 * @param[in]       buf
 *                  Pointer to binary buffer to convert to ascii hex.
 * @param[in]       buf_size
 *                  Number of bytes to convert
 * @return          Converting status. true - success, false - error
*/
bool clip_utils_hex_from_buf(char *hex, const uint8_t *buf, size_t buf_size);

/**
 * @brief           Function used to convert ascii hex array to binary buffer.
 *                  It supports lower- and upper-case characters.
 *                  User needs to be sure that the "hex" buffer has enough space to store all input "buf".
 *                  Function uses 1 byte per every 2 input chars.
 *                  The length of the ascii hex array input buffer must be even.
 * @param[out]      buf
 *                  Pointer where decoded binary data will be stored.
 * @param[in]       hex
 *                  Pointer to ascii hex array to convert to binary data.
 * @param[in]       hex_size
 *                  Number of chars in ascii hex array.
 * @return          Converting status. true - success, false - error
*/
bool clip_utils_hex_to_buf(uint8_t *buf, const char *hex, size_t hex_size);

/**
 * @brief           Function used by "clip_cmd_call_command_callback" function.
 *                  Its used for parsing CLIP_ARG_TYPE_BOOL argument.
 *                  During parsing operation, string "arg" is converted to proper clip_arg_value value.
 *                  It supports only "0" (for false) and "1" (for true) string values.
 * @param[out]      argv
 *                  Argument value contains type and parsed data.
 * @param[in]       arg
 *                  Pointer to string representation of value which will be parsed.
 * @return          Parsing status. true - success, false - error
*/
bool clip_utils_parse_bool(struct clip_arg_value *argv, const char *arg);

/**
 * @brief           Function used by "clip_cmd_call_command_callback" function.
 *                  Its used for parsing CLIP_ARG_TYPE_INT argument.
 *                  During parsing operation, string "arg" is converted to proper clip_arg_value value.
 *                  It supports "0x" prefixes (for hex), "0" prefix (for oct) and no prefix (for dec).
 *                  Supported range is -2147483648..2147483647 (int32_t).
 * @param[out]      argv
 *                  Argument value contains type and parsed data.
 * @param[in]       arg
 *                  Pointer to string representation of value which will be parsed.
 * @return          Parsing status. true - success, false - error
*/
bool clip_utils_parse_int(struct clip_arg_value *argv, const char *arg);

/**
 * @brief           Function used by "clip_cmd_call_command_callback" function.
 *                  Its used for parsing CLIP_ARG_TYPE_UINT argument.
 *                  During parsing operation, string "arg" is converted to proper clip_arg_value value.
 *                  It supports "0x" prefixes (for hex), "0" prefix (for oct) and no prefix (for dec).
 *                  Supported range is 0..4294967295 (uint32_t).
 * @param[out]      argv
 *                  Argument value contains type and parsed data.
 * @param[in]       arg
 *                  Pointer to string representation of value which will be parsed.
 * @return          Parsing status. true - success, false - error
*/
bool clip_utils_parse_uint(struct clip_arg_value *argv, const char *arg);

/**
 * @brief           Function used by "clip_cmd_call_command_callback" function.
 *                  Its used for parsing CLIP_ARG_TYPE_FLOAT argument.
 *                  During parsing operation, string "arg" is converted to proper clip_arg_value value.
 *                  Supported range is aligned with 32-bit float number.
 * @param[out]      argv
 *                  Argument value contains type and parsed data.
 * @param[in]       arg
 *                  Pointer to string representation of value which will be parsed.
 * @return          Parsing status. true - success, false - error
*/
bool clip_utils_parse_float(struct clip_arg_value *argv, const char *arg);

/**
 * @brief           Function used by "clip_cmd_call_command_callback" function.
 *                  Its used for parsing CLIP_ARG_TYPE_HEXARRAY argument.
 *                  During parsing operation, string "arg" is converted to proper clip_arg_value value.
 *                  It supports lower- and upper-case characters.
 * @param[out]      argv
 *                  Argument value contains type and parsed data.
 * @param[in]       arg
 *                  Pointer to string representation of value which will be parsed.
 * @return          Parsing status. true - success, false - error
*/
bool clip_utils_parse_hexarray(struct clip_arg_value *argv, char *arg);

#ifdef __cplusplus
}
#endif

#endif /* CLIP_H */
