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

class ClipUtilsHexTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ClipUtilsHexTest, clip_utils_hex_nibble_to_char)
{
    std::vector<std::tuple<uint8_t, std::optional<char>>> test_cases = {
        {0, '0'},
        {1, '1'},
        {2, '2'},
        {3, '3'},
        {4, '4'},
        {5, '5'},
        {6, '6'},
        {7, '7'},
        {8, '8'},
        {9, '9'},
        {10, 'A'},
        {11, 'B'},
        {12, 'C'},
        {13, 'D'},
        {14, 'E'},
        {15, 'F'},
        {16, std::nullopt},
        {255, std::nullopt},
    };
    
    for (auto t : test_cases) {
        char ch = '\0';
        bool success = clip_utils_hex_nibble_to_char(&ch, std::get<0>(t));
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(ch, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsHexTest, clip_utils_hex_char_to_nibble)
{
    std::vector<std::tuple<char, std::optional<uint8_t>>> test_cases = {
        {'0', 0},
        {'1', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'A', 10},
        {'B', 11},
        {'C', 12},
        {'D', 13},
        {'E', 14},
        {'F', 15},
        {'a', 10},
        {'b', 11},
        {'c', 12},
        {'d', 13},
        {'e', 14},
        {'f', 15},
        {'G', std::nullopt},
        {'H', std::nullopt},
        {'Z', std::nullopt},
        {'X', std::nullopt},        
        {'!', std::nullopt},
        {'\0', std::nullopt},
    };
    
    for (auto t : test_cases) {
        uint8_t n = 0;
        bool success = clip_utils_hex_char_to_nibble(&n, std::get<0>(t));
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(n, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsHexTest, clip_utils_hex_from_buf)
{
    std::vector<std::tuple<std::vector<uint8_t>, std::optional<std::string>>> test_cases = {
        {std::vector<uint8_t>{}, ""},
        {{0x01}, "01"},
        {{0x01, 0x02, 0x03}, "010203"},
        {{0xDE, 0xAD, 0xBE, 0xEF}, "DEADBEEF"},
        {{0xDE, 0xAD, 0xBE, 0xEF, 0xFF}, "DEADBEEFFF"},
    };
    
    for (auto t : test_cases) {
        char str[128] = {};
        bool success = clip_utils_hex_from_buf(str, std::get<0>(t).data(), std::get<0>(t).size());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(std::string{str}, std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}

TEST_F(ClipUtilsHexTest, clip_utils_hex_to_buf)
{
    std::vector<std::tuple<std::string, std::optional<std::vector<uint8_t>>>> test_cases = {
        {"", std::vector<uint8_t>{}},
        {"01", std::vector<uint8_t>{0x01}},
        {"010203", std::vector<uint8_t>{0x01, 0x02, 0x03}},
        {"DEADBEEF", std::vector<uint8_t>{0xDE, 0xAD, 0xBE, 0xEF}},
        {"deadbeef", std::vector<uint8_t>{0xDE, 0xAD, 0xBE, 0xEF}},
        {"DEADBEEFFF", std::vector<uint8_t>{0xDE, 0xAD, 0xBE, 0xEF, 0xFF}},
        {"0", std::nullopt},
        {"012", std::nullopt},
        {"DEADBEEG", std::nullopt},
    };
    
    for (auto t : test_cases) {
        uint8_t buf[128] = {};
        bool success = clip_utils_hex_to_buf(buf, (char*)std::get<0>(t).c_str(), std::get<0>(t).length());
        if (std::get<1>(t).has_value()) {
            EXPECT_TRUE(success);
            EXPECT_EQ(std::vector<uint8_t>(buf, buf + std::get<0>(t).length() / 2), std::get<1>(t).value());
        } else {
            EXPECT_FALSE(success);
        }
    }
}
