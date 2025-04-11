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

static void gpio_set_pin_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    printf("pin[%u] = %u\n", argv[0].val_uint, argv[1].val_uint);
}

static void gpio_set_reg_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    printf("reg[0x%08X] = 0x%08X\n", argv[0].val_uint, argv[1].val_uint);
}

static void gpio_get_pin_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    uint32_t val = rand() % 2;

    printf("pin[%u] = %u\n", argv[0].val_uint, val);
}

static void gpio_get_reg_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    uint32_t val = rand() % 0x100000000;

    printf("reg[0x%08X] = 0x%08X\n", argv[0].val_uint, val);
}

static void gpio_test_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);
}

CLIP_DEF_ROOT_COMMAND(g_gpio_cmd, "gpio", "control gpio", NULL)

    CLIP_DEF_COMMAND("get", "get methods", NULL)
        CLIP_DEF_COMMAND("pin", "get pin state", gpio_get_pin_callback) CLIP_DEF_WITH_ARGS()
            CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END_WITH_ARGS()

        CLIP_DEF_COMMAND("reg", "get register value", gpio_get_reg_callback) CLIP_DEF_WITH_ARGS()
            CLIP_DEF_ARGUMENT("address", "register address", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END_WITH_ARGS()
    CLIP_DEF_COMMAND_END()

    CLIP_DEF_COMMAND("set", "set methods", NULL)
        CLIP_DEF_COMMAND("pin", "set pin state", gpio_set_pin_callback) CLIP_DEF_WITH_ARGS()
            CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
            CLIP_DEF_ARGUMENT("state", "pin state", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END_WITH_ARGS()

        CLIP_DEF_COMMAND("reg", "set register value", gpio_set_reg_callback) CLIP_DEF_WITH_ARGS()
            CLIP_DEF_ARGUMENT("address", "register address", CLIP_ARG_TYPE_UINT)
            CLIP_DEF_ARGUMENT("value", "register value", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END_WITH_ARGS()

    CLIP_DEF_COMMAND_END()

    CLIP_DEF_COMMAND("test", "test method", gpio_test_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("type", "test type", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_OPT_ARGUMENT("mode", "optional test mode", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_COMMAND_END_WITH_ARGS()

CLIP_DEF_ROOT_COMMAND_END()
