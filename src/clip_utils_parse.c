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
#include <stdlib.h>

bool clip_utils_parse_bool(struct clip_arg_value *argv, const char *arg)
{
    CLIP_CONFIG_ASSERT(argv != NULL);
    CLIP_CONFIG_ASSERT(arg != NULL);

    if (strlen(arg) != 1)
        return false;

    if (*arg != '0' && *arg != '1')
        return false;

    argv->type = CLIP_ARG_TYPE_BOOL;
    argv->val_bool = (*arg != '0') ? true : false;
    return true;
}

bool clip_utils_parse_int(struct clip_arg_value *argv, const char *arg)
{
    CLIP_CONFIG_ASSERT(argv != NULL);
    CLIP_CONFIG_ASSERT(arg != NULL);

    char *end_arg = NULL;
    int32_t val = strtol(arg, &end_arg, 0);
    if (*end_arg != '\0')
        return false;

    argv->type = CLIP_ARG_TYPE_INT;
    argv->val_int = val;
    return true;
}

bool clip_utils_parse_uint(struct clip_arg_value *argv, const char *arg)
{
    CLIP_CONFIG_ASSERT(argv != NULL);
    CLIP_CONFIG_ASSERT(arg != NULL);

    if (*arg == '-')
        return false;

    char *end_arg = NULL;
    int32_t val = strtoul(arg, &end_arg, 0);
    if (*end_arg != '\0')
        return false;

    argv->type = CLIP_ARG_TYPE_UINT;
    argv->val_uint = val;
    return true;
}

bool clip_utils_parse_float(struct clip_arg_value *argv, const char *arg)
{
    CLIP_CONFIG_ASSERT(argv != NULL);
    CLIP_CONFIG_ASSERT(arg != NULL);

    char *end_arg = NULL;
    float val = strtod(arg, &end_arg);
    if (*end_arg != '\0')
        return false;

    argv->type = CLIP_ARG_TYPE_FLOAT;
    argv->val_float = val;
    return true;
}

bool clip_utils_parse_hexarray(struct clip_arg_value *argv, char *arg)
{
    bool s;
    uint8_t tmp[sizeof(uint32_t)] = {0};
    size_t len = 0;
    uint8_t *header = NULL;
    uint8_t *data = NULL;

    CLIP_CONFIG_ASSERT(argv != NULL);
    CLIP_CONFIG_ASSERT(arg != NULL);

    size_t asciihex_len = strlen(arg);
    if (asciihex_len & 1)
        return false;
    
    header = (uint8_t*)&arg[0];
    len = asciihex_len >> 1;
    if (len == 0) {
        *header = 0x00;
    } else {
        for (size_t i = 0; i < sizeof(tmp) && i < len; i++) {
            s = clip_utils_hex_to_buf(&tmp[i], &arg[i << 1], 2);
            if (s == false)
                return false;
        }

        if (len <= sizeof(tmp)) {
            *header = len;
            data = (uint8_t*)&arg[1];
        } else {
            *header = 0x80 | sizeof(tmp);
            for (size_t i = 0; i < sizeof(tmp); i++)
                header[i + 1] = len >> (i << 3);
            data = (uint8_t*)&arg[sizeof(tmp) + 1];

            for (size_t i = sizeof(tmp); i < len; i++) {
                uint8_t b = 0;
                s = clip_utils_hex_to_buf(&b, &arg[i << 1], 2);
                if (s == false)
                    return false;
                data[i] = b;
            }
        }

        for (size_t i = 0; i < sizeof(tmp) && i < len; i++) {
            data[i] = tmp[i];
        }
    }

    argv->type = CLIP_ARG_TYPE_HEXARRAY;
    argv->val_hexarray = (clip_hexarray_t)&arg[0];
    return true;
}
