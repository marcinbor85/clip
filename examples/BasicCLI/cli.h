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

#pragma once

#include <clip.h>

#ifdef __cplusplus
extern "C" {
#endif
  
///< root clip handler descriptor
extern const struct clip g_cli_clip;

void cli_gpio_set_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context);
void cli_gpio_get_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context);

void cli_echo_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context);

void cli_event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context);

#ifdef __cplusplus
}
#endif
