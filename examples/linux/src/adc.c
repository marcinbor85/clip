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

#include <time.h>
#include <stdlib.h>

static void adc_read_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    uint32_t val = rand() % 4096;

    printf("adc[%u] = %u\n", argv[0].val_uint, val);
}

static void adc_set_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    printf("vref = %.03f\n", argv[0].val_float);
}

static void adc_start_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    printf("started...\n");
}

static void adc_stop_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    printf("stopped...\n");
}

CLIP_DEF_ROOT_COMMAND(g_adc_cmd, "adc", "control adc driver", NULL)

    CLIP_DEF_COMMAND("read", "read input", adc_read_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("channel", "channel number", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_COMMAND_END()

    CLIP_DEF_COMMAND("set", "set reference", adc_set_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("vref", "voltage reference", CLIP_ARG_TYPE_FLOAT)
    CLIP_DEF_COMMAND_END()

    CLIP_DEF_COMMAND("start", "start sampling", adc_start_callback) CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("stop", "stop sampling", adc_stop_callback) CLIP_DEF_COMMAND_END()

CLIP_DEF_ROOT_COMMAND_END()
