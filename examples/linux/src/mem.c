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
#include "main.h"

static void mem_read_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    uint32_t addr = argv[0].val_uint;
    uint32_t size = argv[1].val_uint;

    print_mem_dump(addr, size, NULL);
}

static void mem_write_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    uint32_t addr = argv[0].val_uint;
    clip_hexarray_t hexarray = argv[1].val_hexarray;

    uint8_t *data = NULL;
    size_t size = clip_utils_arg_unpack_hexarray(&data, hexarray);

    print_mem_dump(addr, size, data);
}

CLIP_DEF_ROOT_COMMAND(g_mem_cmd, "mem", "memory driver", NULL)

    CLIP_DEF_COMMAND("write", "write data to memory", mem_write_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("address", "address to write", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_ARGUMENT("data", "binary data to write", CLIP_ARG_TYPE_HEXARRAY)
    CLIP_DEF_COMMAND_END()

    CLIP_DEF_COMMAND("read", "read data from memory", mem_read_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("address", "address to read", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_ARGUMENT("size", "number of bytes to read", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_COMMAND_END()

CLIP_DEF_ROOT_COMMAND_END()
