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

using ::testing::_;
using ::testing::Test;
using ::testing::Invoke;
using ::testing::Return;

class ClipUtilsArgTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_first)
{
    char *arg;
    char *next_arg;

    std::vector<std::tuple<std::string, std::string, std::string>> test_cases = {
        {"test 123",        "test",             "123"},        
        {"123  ",           "123",              " "},
        {" 123 ",           "123",              ""},
        {"  123",           "123",              ""},

        {"",                "",                 ""},
        {"  ",              "",                 ""},
        {"test",            "test",             ""},

        {"  test 123  ",    "test",             "123  "},
        {"  test  123 ",    "test",             " 123 "},
        {"  test   123",    "test",             "  123"},
    };
    
    for (auto t : test_cases) {
        next_arg = clip_utils_arg_get_first(&arg, (char*)std::get<0>(t).c_str());
        EXPECT_STREQ(arg, std::get<1>(t).c_str());
        EXPECT_STREQ(next_arg, std::get<2>(t).c_str());
    }
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_first__withQuoteMark)
{
    char *arg;
    char *next_arg;

    std::vector<std::tuple<std::string, std::string, std::string>> test_cases = {
        {"\"test\" 123",                "test",             "123"},
        {"\"123\"  ",                   "123",              " "},
        {" \"123\" ",                   "123",              ""},
        {"  \"123\"",                   "123",              ""},

        {"\"\"",                        "",                 ""},
        {"\"test\"",                    "test",             ""},
        {"\" test \"",                  " test ",           ""},

        {"\"test 123\"abc",             "test 123abc",      ""},
        {"\"test 123\" abc",            "test 123",         "abc"},
        {"\"test 123\"  abc",           "test 123",         " abc"},
        {"abc\"test 123\"abc",          "abctest 123abc",   ""},
        {"abc\"test 123\" abc",         "abctest 123",      "abc"},
        {"abc\"test 123\"  abc",        "abctest 123",      " abc"},
        
        {"  \"test\" \"123 abc\"  ",    "test",             "\"123 abc\"  "},
        {"  \"test\"  \"123 abc\" ",    "test",             " \"123 abc\" "},
        {"  \"test\"   \"123 abc\"",    "test",             "  \"123 abc\""},
    };
    
    for (auto t : test_cases) {
        next_arg = clip_utils_arg_get_first(&arg, (char*)std::get<0>(t).c_str());
        EXPECT_STREQ(arg, std::get<1>(t).c_str());
        EXPECT_STREQ(next_arg, std::get<2>(t).c_str());
    }
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_first__withEscapeChar)
{
    char *arg;
    char *next_arg;

    std::vector<std::tuple<std::string, std::string, std::string>> test_cases = {
        {"\"\\\"test abc\\\"\" 123",    "\"test abc\"",       "123"},
        {"\"\\\"test\"abc\\\"\" 123",   "\"testabc\" 123",    ""},
        {"  \"test abc\\\"\" 123",      "test abc\"",         "123"},
        {"  test abc\\\"\" 123",        "test",               "abc\\\"\" 123"},
        {"  testabc\\\"\" 123",         "testabc\" 123",      ""},
    };
    
    for (auto t : test_cases) {
        next_arg = clip_utils_arg_get_first(&arg, (char*)std::get<0>(t).c_str());
        EXPECT_STREQ(arg, std::get<1>(t).c_str());
        EXPECT_STREQ(next_arg, std::get<2>(t).c_str());
    }
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_type_string)
{
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_STRING), "STRING");
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_BOOL), "BOOL");
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_INT), "INT");
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_UINT), "UINT");
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_FLOAT), "FLOAT");
    EXPECT_STREQ(clip_utils_arg_get_type_string(CLIP_ARG_TYPE_HEXARRAY), "HEXARRAY");
    EXPECT_STREQ(clip_utils_arg_get_type_string((clip_arg_type_t)(CLIP_ARG_TYPE_HEXARRAY + 1)), "UNKNOWN");
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_error_string)
{
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_NO_ERROR), "NO ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_NOT_ENOUGH_ARGUMENTS), "NOT ENOUGH ARGUMENTS");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_PARSE_BOOL), "BOOLEAN PARSING ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_PARSE_INT), "SIGNED INTEGER PARSING ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_PARSE_UINT), "UNSIGNED INTEGER PARSING ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_PARSE_FLOAT), "FLOAT NUMBER PARSING ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string(CLIP_ARG_ERROR_PARSE_HEXARRAY), "ASCII HEX ARRAY PARSING ERROR");
    EXPECT_STREQ(clip_utils_arg_get_error_string((clip_arg_error_t)(CLIP_ARG_ERROR_PARSE_HEXARRAY + 1)), "UNKNOWN");
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_update_buf)
{
    std::vector<std::tuple<char*, size_t, size_t, size_t, char*, size_t, size_t>> test_cases = {
        {(char*)1000, 1, 0, 1, (char*)1001, 0, 1},
        {(char*)1000, 1, 0, 10, (char*)1010, 0, 10},
        {(char*)1000, 1, 0, 19, (char*)1019, 0, 19},
        {(char*)1000, 10, 20, 1, (char*)1001, 9, 21},
        {(char*)1000, 10, 20, 10, (char*)1010, 0, 30},
        {(char*)1000, 10, 20, 19, (char*)1019, 0, 39},
    };
    
    for (auto t : test_cases) {
        char *buf_in = std::get<0>(t);
        size_t buf_size  = std::get<1>(t);
        size_t out_size  = std::get<2>(t);
        size_t size  = std::get<3>(t);
        char *buf_out = clip_utils_arg_update_buf(buf_in, &buf_size, &out_size, size);
        EXPECT_EQ(buf_out, std::get<4>(t));
        EXPECT_EQ(buf_size, std::get<5>(t));
        EXPECT_EQ(out_size, std::get<6>(t));
    }
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_get_command_usage_string)
{
    char buf[256];

    std::array<const struct clip_arg, 6> args = {
        clip_arg { "str", nullptr, CLIP_ARG_TYPE_STRING, false },
        clip_arg { "bool", nullptr, CLIP_ARG_TYPE_BOOL, false },
        clip_arg { "int", nullptr, CLIP_ARG_TYPE_INT, false },
        clip_arg { "uint", nullptr, CLIP_ARG_TYPE_UINT, false },
        clip_arg { "float", nullptr, CLIP_ARG_TYPE_FLOAT, true },
        clip_arg { "hex", nullptr, CLIP_ARG_TYPE_HEXARRAY, true },
        
    };

    const struct clip_command cmd = {
        "command",
        nullptr,
        nullptr,
        nullptr,
        (const struct clip_arg**)(std::array<const struct clip_arg*, 7> {
            &args.at(0),
            &args.at(1),
            &args.at(2),
            &args.at(3),
            &args.at(4),
            &args.at(5),
            nullptr
        }).data()
    };

    size_t size_out = clip_utils_arg_get_command_usage_string(buf, sizeof(buf), &cmd);
    EXPECT_EQ(std::string(buf, buf + size_out), "command <str:STRING> <bool:BOOL> <int:INT> <uint:UINT> [float:FLOAT] [hex:HEXARRAY]");
}

TEST_F(ClipUtilsArgTest, clip_utils_arg_unpack_hexarray)
{
    std::vector<std::tuple<std::vector<uint8_t>, size_t, std::vector<uint8_t>>> test_cases = {
        {{0x00}, 0, {}},
        {{0x01, 0xAA}, 1, {0xAA}},
        {{0x04, 0xDE, 0xAD, 0xBE, 0xEF}, 4, {0xDE, 0xAD, 0xBE, 0xEF}},
        {{0x05, 0x01, 0x02, 0x03, 0x04, 0x05}, 5, {0x01, 0x02, 0x03, 0x04, 0x05}},
        {{0x08, 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE}, 8, {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE}},
        {{0x81, 0x01, 0xAA}, 1, {0xAA}},
        {{0x82, 0x01, 0x00, 0xAA}, 1, {0xAA}},
        {{0x83, 0x01, 0x00, 0x00, 0xAA}, 1, {0xAA}},
        {{0x84, 0x01, 0x00, 0x00, 0x00, 0xAA}, 1, {0xAA}},
        {{0x84, 0x02, 0x00, 0x00, 0x00, 0xAA, 0xBB}, 2, {0xAA, 0xBB}},
    };
    
    for (auto t : test_cases) {
        uint8_t *data = nullptr; 
        size_t size = clip_utils_arg_unpack_hexarray(&data, (clip_hexarray_t)std::get<0>(t).data());
        EXPECT_EQ(size, std::get<1>(t));
        EXPECT_EQ(std::vector<uint8_t>(data, data + size), std::get<2>(t));
    }
}
