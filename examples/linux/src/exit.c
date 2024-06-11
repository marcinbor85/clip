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

static void exit_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
    print_args(__func__, argc, argv);

    struct app_context *ctx = (struct app_context *)self->context;
    ctx->exit_app = 1;
}

CLIP_DEF_ROOT_COMMAND(g_exit_cmd, "exit", "application exit", exit_callback)
CLIP_DEF_ROOT_COMMAND_END()
