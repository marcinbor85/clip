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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#include "clip.h"
#include "main.h"

void print_args(const char *tag, size_t argc, struct clip_arg_value argv[])
{
    printf(COLOR_GREEN "%s, argc=%lu, argv=", tag, argc);
    for (size_t i = 0; i < argc; i++) {
        const char *type_name = clip_utils_arg_get_type_string(argv[i].type);
        switch (argv[i].type) {
        case CLIP_ARG_TYPE_STRING:
            printf("<%s:%s> ", type_name, argv[i].val_str);
            break;

        case CLIP_ARG_TYPE_INT:
            printf("<%s:%d> ", type_name, argv[i].val_int);
            break;

        case CLIP_ARG_TYPE_UINT:
            printf("<%s:%u> ", type_name, argv[i].val_uint);
            break;

        case CLIP_ARG_TYPE_FLOAT:
            printf("<%s:%f> ", type_name, argv[i].val_float);
            break;

        case CLIP_ARG_TYPE_HEXARRAY: {
            printf("<%s:", type_name);
            uint8_t *data = NULL;
            size_t size = clip_utils_arg_unpack_hexarray(&data, argv[i].val_hexarray);
            for (size_t n = 0; n < size; n++)
                printf("%02X", data[n]);
            printf("> ");
            break;
        }
        
        default:
            printf("<%s:%s> ", type_name, argv[i].val_str);
            break;
        }
    }
    printf(COLOR_RESET "\n");
}

void print_mem_dump(uint32_t addr, size_t size, uint8_t *data)
{
    const size_t max_align = 16;
    size_t align = 0;
    size_t buf_size = 0;
    uint8_t buf[max_align];

    while (size > 0 || align > 0) {
        if (align == 0)
            printf("0x%08X  ", addr);

        if (size > 0) {
            uint8_t val;
            if (data == NULL) {
                val = rand() % 256;
            } else {
                val = *data;
                data++;
            }
            printf("%02X ", val);            
            buf[buf_size++] = val;
        } else {
            printf("   ");
        }

        if (size > 0)
            size--;
        
        addr++;
        align++;
        align %= max_align;

        if (align == 0) {
            printf(" ");
            for (size_t i = 0; i < buf_size; i++) {
                char ch = (char)buf[i];
                printf("%c", isalnum(ch) ? ch : '.');
            }
            printf("\n");
            buf_size = 0;
        }
    }
}
