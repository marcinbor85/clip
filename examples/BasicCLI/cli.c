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

#include "cli.h"

CLIP_DEF_ROOT_COMMAND(g_cli_gpio_cmd, "gpio", "control gpio", NULL)
  CLIP_DEF_COMMAND("get", "get pin state", cli_gpio_get_callback) CLIP_DEF_WITH_ARGS()
    CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
  CLIP_DEF_COMMAND_END()
  CLIP_DEF_COMMAND("set", "set pin state", cli_gpio_set_callback) CLIP_DEF_WITH_ARGS()
    CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_ARGUMENT("state", "pin state", CLIP_ARG_TYPE_UINT)
  CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

CLIP_DEF_ROOT_COMMAND(g_cli_echo_cmd, "echo", "echo command", cli_echo_callback) CLIP_DEF_WITH_ARGS()
  CLIP_DEF_ARGUMENT("text", "text to print", CLIP_ARG_TYPE_STRING)
CLIP_DEF_ROOT_COMMAND_END()

CLIP_DEF_ROOT(g_cli_clip, NULL, cli_event_callback)
  CLIP_DEF_ADD_ROOT_COMMAND(&g_cli_gpio_cmd)
  CLIP_DEF_ADD_ROOT_COMMAND(&g_cli_echo_cmd)
CLIP_DEF_ROOT_END()
