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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clip.h"

#include "mock_clip_utils_hex.hpp"

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;

class ClipUtilsParseTest : public Test
{
protected:
    virtual void SetUp()
    {
        ClipUtilsHex_Mock::create();
    }

    virtual void TearDown()
    {
        ClipUtilsHex_Mock::destroy();
    }
};

TEST_F(ClipUtilsParseTest, clip_utils_parse_bool)
{
    std::vector<std::tuple<std::string, std::optional<bool>>> test_cases = {
        {".123", std::nullopt},
        {"x", std::nullopt},
        {"A", std::nullopt},
        {"0x10xFF", std::nullopt},
        {"-1-2", std::nullopt},
        {"-1a", std::nullopt},
        {"-1", std::nullopt},
        {"0", false},
        {"1", true},
        {"2", std::nullopt},
        {"9999", std::nullopt},
    };
    
    for (auto t : test_cases) {
        clip_arg_value val {};
        bool success = clip_utils_parse_bool(&val, (char*)std::get<0>(t).c_str());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(val.type, CLIP_ARG_TYPE_BOOL);
            EXPECT_EQ(val.val_bool, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsParseTest, clip_utils_parse_int)
{
    std::vector<std::tuple<std::string, std::optional<int32_t>>> test_cases = {
        {".123", std::nullopt},
        {"x", std::nullopt},
        {"A", std::nullopt},
        {"0x10xFF", std::nullopt},
        {"-1-2", std::nullopt},
        {"-1a", std::nullopt},
        {"-1", -1},
        {"0", 0},
        {"9999", 9999},
        {"-2147483648", -2147483648},
        {"2147483647", 2147483647},
        {"2147483648", -2147483648},
        {"4294967295", -1},
        {"010", 8},
        {"0777", 511},
        {"0xA", 10},
        {"0x10", 16},
        {"-0x10", -16},
        {"0x7FFFFFFF", 2147483647},
        {"0x80000000", -2147483648},
        {"0xFFFFFFFF", -1},
        {"0xdeadbeef", 0xDEADBEEF},
    };
    
    for (auto t : test_cases) {
        clip_arg_value val {};
        bool success = clip_utils_parse_int(&val, (char*)std::get<0>(t).c_str());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(val.type, CLIP_ARG_TYPE_INT);
            EXPECT_EQ(val.val_int, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsParseTest, clip_utils_parse_uint)
{
    std::vector<std::tuple<std::string, std::optional<uint32_t>>> test_cases = {
        {".123", std::nullopt},
        {"x", std::nullopt},
        {"A", std::nullopt},
        {"-1", std::nullopt},
        {"-0x10", std::nullopt},
        {"0x10xFF", std::nullopt},
        {"0A", std::nullopt},
        {"0", 0},
        {"9999", 9999},
        {"2147483647", 2147483647},
        {"2147483648", 2147483648},
        {"4294967295", 4294967295},
        {"010", 8},
        {"0777", 511},
        {"0xA", 10},
        {"0x10", 16},
        {"0x7FFFFFFF", 2147483647},
        {"0x80000000", 2147483648},
        {"0xFFFFFFFF", 4294967295},
        {"0xdeadbeef", 0xDEADBEEF},
    };
    
    for (auto t : test_cases) {
        clip_arg_value val {};
        bool success = clip_utils_parse_uint(&val, (char*)std::get<0>(t).c_str());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(val.type, CLIP_ARG_TYPE_UINT);
            EXPECT_EQ(val.val_uint, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsParseTest, clip_utils_parse_float)
{
    std::vector<std::tuple<std::string, std::optional<float>>> test_cases = {
        {"0", 0.0f},
        {".123", 0.123f},
        {"9999", 9999.0f},
        {"1.23", 1.23f},
        {"-0.0001", -0.0001f},
        {"01", 1.0f},
        {"x", std::nullopt},
        {"1.2.3", std::nullopt},
    };
    
    for (auto t : test_cases) {
        clip_arg_value val {};
        bool success = clip_utils_parse_float(&val, (char*)std::get<0>(t).c_str());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(val.type, CLIP_ARG_TYPE_FLOAT);
            EXPECT_FLOAT_EQ(val.val_float, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsParseTest, clip_utils_parse_hexarray)
{
    EXPECT_CALL(*ClipUtilsHex_Mock::get(), clip_utils_hex_to_buf(_, _, _)).
        WillRepeatedly(Invoke([](uint8_t *buf, const char *hex, size_t hex_size)->bool {
            while (hex_size > 0) {
                const std::string s{hex, hex + 2};
                *buf++ = std::stoi(s.c_str(), 0, 16);
                hex_size -= 2;
            }
            return true;
        }));

    std::vector<std::tuple<std::string, std::optional<std::vector<uint8_t>>>> test_cases = {
        {"", std::vector<uint8_t> {0x00}},
        {"0", std::nullopt},
        {"01", std::vector<uint8_t> {0x01, 0x01}},
        {"012", std::nullopt},
        {"DEADBEEF", std::vector<uint8_t> {0x04, 0xDE, 0xAD, 0xBE, 0xEF}},
        {"01DEADBEEF", std::vector<uint8_t> {0x84, 0x05, 0x00, 0x00, 0x00, 0x01, 0xDE, 0xAD, 0xBE, 0xEF}},
    };
    
    for (auto t : test_cases) {
        clip_arg_value val {};
        bool success = clip_utils_parse_hexarray(&val, (char*)std::get<0>(t).c_str());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(val.type, CLIP_ARG_TYPE_HEXARRAY);
            EXPECT_EQ(std::vector<uint8_t>(val.val_hexarray, val.val_hexarray + std::get<1>(t).value().size()), std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}
