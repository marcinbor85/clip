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

#ifndef CLIP_CONFIG_H
#define CLIP_CONFIG_H

#ifdef CLIP_USE_USER_CONFIG
///< optional user-defined clip configuration
#include "clip_user_config.h"
#endif

#ifndef CLIP_CONFIG_ASSERT
///< optional macro for asserts (not needed in production code)
#define CLIP_CONFIG_ASSERT(cond)
#endif

#ifndef CLIP_CONFIG_ARGS_MAX_NUM
///< maximum number of supported command arguments
#define CLIP_CONFIG_ARGS_MAX_NUM 10
#endif

#ifndef CLIP_CONFIG_HELP_COMMAND
 ///< special subcommand for calling help event
#define CLIP_CONFIG_HELP_COMMAND "?"
#endif

#endif /* CLIP_CONFIG_H */
