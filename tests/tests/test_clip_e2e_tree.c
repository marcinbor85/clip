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

extern void test_clip_event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context);
extern void test_clip_command_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context);

CLIP_DEF_ROOT_COMMAND(g_cmd1, "cmd1", "cmd2 description", NULL)
    CLIP_DEF_COMMAND("abc", "abc command", test_clip_command_callback)
        CLIP_DEF_COMMAND("a1", "a1 command", test_clip_command_callback)
            CLIP_DEF_COMMAND("a11", "a11 command", test_clip_command_callback) CLIP_DEF_COMMAND_END()
            CLIP_DEF_COMMAND("a12", "a12 command", test_clip_command_callback) CLIP_DEF_COMMAND_END()
        CLIP_DEF_COMMAND_END()
        CLIP_DEF_COMMAND("a2", "a2 command", test_clip_command_callback) CLIP_DEF_COMMAND_END()
        CLIP_DEF_COMMAND("a3", "a3 command", test_clip_command_callback) CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("def", "def command", test_clip_command_callback) CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("xyz", "xyz command", test_clip_command_callback) CLIP_DEF_WITH_ARGS()
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("no", "no command", NULL)
    CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

CLIP_DEF_ROOT_COMMAND(g_cmd2, "cmd2", "cmd2 description", NULL)
    CLIP_DEF_COMMAND("abc", "abc command", test_clip_command_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("a", "a argument", CLIP_ARG_TYPE_STRING)
        CLIP_DEF_ARGUMENT("b", "b argument", CLIP_ARG_TYPE_BOOL)
        CLIP_DEF_ARGUMENT("c", "c argument", CLIP_ARG_TYPE_INT)
        CLIP_DEF_ARGUMENT("d", "d argument", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_ARGUMENT("e", "e argument", CLIP_ARG_TYPE_FLOAT)
        CLIP_DEF_ARGUMENT("f", "f argument", CLIP_ARG_TYPE_HEXARRAY)
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("xyz", "xyz command", test_clip_command_callback) CLIP_DEF_WITH_ARGS()
        CLIP_DEF_ARGUMENT("a", "a argument", CLIP_ARG_TYPE_INT)
    CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

CLIP_DEF_ROOT(g_clip, (void*)11223344, test_clip_event_callback)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_cmd1)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_cmd2)
CLIP_DEF_ROOT_END()
