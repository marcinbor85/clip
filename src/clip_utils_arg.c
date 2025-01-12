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
#include <stdio.h>

char* clip_utils_arg_get_first(char **arg, char *cmd_line)
{
    CLIP_CONFIG_ASSERT(arg != NULL);
    CLIP_CONFIG_ASSERT(cmd_line != NULL);

    int escape = 0;
    int quotemark = 0;
    size_t arg_pos = 0;
    char *ch = cmd_line;

    while (*ch == ' ')
        ch++;
    *arg = ch;

    while (*ch != '\0') {
        if (quotemark == 0 && *ch == ' ') {
            ch++;
            break;
        }

        if (escape == 0 && *ch == '\\') {
            escape = 1;
            ch++;
            continue;
        }

        if (escape == 0 && *ch == '\"') {
            quotemark = !quotemark;
            ch++;
            continue;
        }

        escape = 0;
        (*arg)[arg_pos] = *ch;
        arg_pos++;    
        ch++;
    }
    (*arg)[arg_pos] = '\0';

    return ch;
}

const char* clip_utils_arg_get_type_string(clip_arg_type_t type)
{
    switch (type) {
    case CLIP_ARG_TYPE_STRING: return "STRING";
    case CLIP_ARG_TYPE_BOOL: return "BOOL";
    case CLIP_ARG_TYPE_INT: return "INT";
    case CLIP_ARG_TYPE_UINT: return "UINT";
    case CLIP_ARG_TYPE_FLOAT: return "FLOAT";
    case CLIP_ARG_TYPE_HEXARRAY: return "HEXARRAY";
    default: return "UNKNOWN";
    }
}

const char* clip_utils_arg_get_error_string(clip_arg_error_t error)
{
    switch (error) {
    case CLIP_ARG_ERROR_NO_ERROR: return "NO ERROR";
    case CLIP_ARG_ERROR_NOT_ENOUGH_ARGUMENTS: return "NOT ENOUGH ARGUMENTS";
    case CLIP_ARG_ERROR_PARSE_BOOL: return "BOOLEAN PARSING ERROR";
    case CLIP_ARG_ERROR_PARSE_INT: return "SIGNED INTEGER PARSING ERROR";
    case CLIP_ARG_ERROR_PARSE_UINT: return "UNSIGNED INTEGER PARSING ERROR";
    case CLIP_ARG_ERROR_PARSE_FLOAT: return "FLOAT NUMBER PARSING ERROR";
    case CLIP_ARG_ERROR_PARSE_HEXARRAY: return "ASCII HEX ARRAY PARSING ERROR";
    default: return "UNKNOWN";
    }
}

char* clip_utils_arg_update_buf(char *buf, size_t *buf_size, size_t *out_size, size_t size)
{
    CLIP_CONFIG_ASSERT(buf != NULL);
    CLIP_CONFIG_ASSERT(buf_size != NULL);
    CLIP_CONFIG_ASSERT(out_size != NULL);

    *out_size += size;
    buf += size;
    *buf_size -= (size < *buf_size) ? size : *buf_size;
    return buf;
}

size_t clip_utils_arg_get_command_usage_string(char *buf, size_t buf_size, const struct clip_command *cmd)
{
    size_t out_size = 0;
    size_t size = 0;

    CLIP_CONFIG_ASSERT(cmd != NULL);

    size = snprintf(buf, buf_size, "%s ", cmd->name);
    buf = clip_utils_arg_update_buf(buf, &buf_size, &out_size, size);

    const struct clip_arg* *args = cmd->args;
    if (args != NULL) {
        while (*args != NULL) {
            if (args != cmd->args) {
                strncat(buf, " ", buf_size);
                buf = clip_utils_arg_update_buf(buf, &buf_size, &out_size, 1);
            }
            size = snprintf(buf, buf_size, "[%s:%s]", (*args)->name, clip_utils_arg_get_type_string((*args)->type));
            buf = clip_utils_arg_update_buf(buf, &buf_size, &out_size, size);
            args++;
        }
    }
    
    return out_size;
}

size_t clip_utils_arg_unpack_hexarray(uint8_t **data, clip_hexarray_t hex_array)
{
    CLIP_CONFIG_ASSERT(data != NULL);
    CLIP_CONFIG_ASSERT(hex_array != NULL);

    size_t len = 0;
    size_t len_size = 0;
    uint8_t header = hex_array[0];

    if (header & 0x80) {
        len_size = header & 0x0F;
        for (size_t i = 0; i < len_size; i++) {
            len |= hex_array[i + 1] << (i << 3);
        }
    } else {
        len = header;
    }
    *data = &hex_array[len_size + 1];
    return len;
}
