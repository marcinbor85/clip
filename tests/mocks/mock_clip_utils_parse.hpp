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

struct ClipUtilsParse_Mock : public Mock<ClipUtilsParse_Mock>
{
    MOCK_METHOD(bool, clip_utils_parse_bool, (struct clip_arg_value *argv, const char *arg), ());
    MOCK_METHOD(bool, clip_utils_parse_int, (struct clip_arg_value *argv, const char *arg), ());
    MOCK_METHOD(bool, clip_utils_parse_uint, (struct clip_arg_value *argv, const char *arg), ());
    MOCK_METHOD(bool, clip_utils_parse_float, (struct clip_arg_value *argv, const char *arg), ());
    MOCK_METHOD(bool, clip_utils_parse_hexarray, (struct clip_arg_value *argv, char *arg), ());
};

extern "C" {

bool clip_utils_parse_bool(struct clip_arg_value *argv, const char *arg)
{
    return ClipUtilsParse_Mock::get()->clip_utils_parse_bool(argv, arg);
}

bool clip_utils_parse_int(struct clip_arg_value *argv, const char *arg)
{
    return ClipUtilsParse_Mock::get()->clip_utils_parse_int(argv, arg);
}

bool clip_utils_parse_uint(struct clip_arg_value *argv, const char *arg)
{
    return ClipUtilsParse_Mock::get()->clip_utils_parse_uint(argv, arg);
}

bool clip_utils_parse_float(struct clip_arg_value *argv, const char *arg)
{
    return ClipUtilsParse_Mock::get()->clip_utils_parse_float(argv, arg);
}

bool clip_utils_parse_hexarray(struct clip_arg_value *argv, char *arg)
{
    return ClipUtilsParse_Mock::get()->clip_utils_parse_hexarray(argv, arg);
}

}
