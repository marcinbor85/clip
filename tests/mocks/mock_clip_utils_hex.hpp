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

struct ClipUtilsHex_Mock : public Mock<ClipUtilsHex_Mock>
{    
    MOCK_METHOD(bool, clip_utils_hex_nibble_to_char, (char *ch, const uint8_t n), ());
    MOCK_METHOD(bool, clip_utils_hex_char_to_nibble, (uint8_t *n, const char ch), ());
    MOCK_METHOD(bool, clip_utils_hex_from_buf, (char *hex, const uint8_t *buf, size_t buf_size), ());
    MOCK_METHOD(bool, clip_utils_hex_to_buf, (uint8_t *buf, const char *hex, size_t hex_size), ());
};

extern "C" {

bool clip_utils_hex_nibble_to_char(char *ch, const uint8_t n)
{
    return ClipUtilsHex_Mock::get()->clip_utils_hex_nibble_to_char(ch, n);
}

bool clip_utils_hex_char_to_nibble(uint8_t *n, const char ch)
{
    return ClipUtilsHex_Mock::get()->clip_utils_hex_char_to_nibble(n, ch);
}

bool clip_utils_hex_from_buf(char *hex, const uint8_t *buf, size_t buf_size)
{
    return ClipUtilsHex_Mock::get()->clip_utils_hex_from_buf(hex, buf, buf_size);
}

bool clip_utils_hex_to_buf(uint8_t *buf, const char *hex, size_t hex_size)
{
    return ClipUtilsHex_Mock::get()->clip_utils_hex_to_buf(buf, hex, hex_size);
}

}
