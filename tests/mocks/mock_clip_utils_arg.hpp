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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clip.h"

#include "mock.hpp"

struct ClipUtilsArg_Mock : public Mock<ClipUtilsArg_Mock>
{
    MOCK_METHOD(char*, clip_utils_arg_get_first, (char **arg, char *cmd_line), ());
    MOCK_METHOD(const char*, clip_utils_arg_get_type_string, (clip_arg_type_t type), ());
    MOCK_METHOD(char*, clip_utils_arg_update_buf, (char *buf, size_t *buf_size, size_t *out_size, size_t size), ());
    MOCK_METHOD(size_t, clip_utils_arg_get_command_usage_string, (char *buf, size_t buf_size, const struct clip_command *cmd), ());
    MOCK_METHOD(size_t, clip_utils_arg_unpack_hexarray, (uint8_t **data, clip_hexarray_t hex_array), ());
};

extern "C" {

char* clip_utils_arg_get_first(char **arg, char *cmd_line)
{
    return ClipUtilsArg_Mock::get()->clip_utils_arg_get_first(arg, cmd_line);
}

const char* clip_utils_arg_get_type_string(clip_arg_type_t type)
{
    return ClipUtilsArg_Mock::get()->clip_utils_arg_get_type_string(type);
}

char* clip_utils_arg_update_buf(char *buf, size_t *buf_size, size_t *out_size, size_t size)
{
    return ClipUtilsArg_Mock::get()->clip_utils_arg_update_buf(buf, buf_size, out_size, size);
}

size_t clip_utils_arg_get_command_usage_string(char *buf, size_t buf_size, const struct clip_command *cmd)
{
    return ClipUtilsArg_Mock::get()->clip_utils_arg_get_command_usage_string(buf, buf_size, cmd);
}

size_t clip_utils_arg_unpack_hexarray(uint8_t **data, clip_hexarray_t hex_array)
{
    return ClipUtilsArg_Mock::get()->clip_utils_arg_unpack_hexarray(data, hex_array);
}

}
