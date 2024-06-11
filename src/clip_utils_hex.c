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

bool clip_utils_hex_nibble_to_char(char *ch, const uint8_t n)
{
    CLIP_CONFIG_ASSERT(ch != NULL);

    if (n < 10) {
        *ch = '0' + n;
    } else if (n < 16) {
        *ch = 'A' + (n - 10);
    } else {
        return false;
    }
    return true;
}

bool clip_utils_hex_char_to_nibble(uint8_t *n, const char ch)
{
    CLIP_CONFIG_ASSERT(n != NULL);

    if (ch >= '0' && ch <= '9') {
        *n = ch - '0';
    } else if (ch >= 'A' && ch <= 'F') {
        *n = ch - 'A' + 10;
    } else if (ch >= 'a' && ch <= 'f') {
        *n = ch - 'a' + 10;
    } else {
        return false;
    }
    return true;
}

bool clip_utils_hex_from_buf(char *hex, const uint8_t *buf, size_t buf_size)
{
    bool s;

    while (buf_size-- > 0) {
        s = clip_utils_hex_nibble_to_char(hex++, *buf >> 4);
        if (s == false)
            return false;

        s = clip_utils_hex_nibble_to_char(hex++, *buf++ & 0x0F);
        if (s == false)
            return false;
    }

    return true;
}

bool clip_utils_hex_to_buf(uint8_t *buf, const char *hex, size_t hex_size)
{
    bool s;
    uint8_t b;
    
    if (hex_size & 1)
        return false;

    while (hex_size > 0) {
        s = clip_utils_hex_char_to_nibble(&b, *hex++);
        if (s == false)
            return false;
        *buf = b << 4;
        
        s = clip_utils_hex_char_to_nibble(&b, *hex++);
        if (s == false)
            return false;
        *buf++ |= (b & 0x0F);

        hex_size -= 2;
    }
    return true;
}
