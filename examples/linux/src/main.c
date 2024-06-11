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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "clip.h"
#include "main.h"

static void event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context)
{
    switch (event) {
    case CLIP_EVENT_HELP:
        printf(COLOR_YELLOW "supported commands for <%s>:\n", (event_arg->help.cmd != NULL) ? event_arg->help.cmd->name : ".");
        while (*event_arg->help.commands != NULL) {
            printf("%s - %s\n", (*event_arg->help.commands)->name, (*event_arg->help.commands)->description);
            event_arg->help.commands++;
        }
        printf(COLOR_RESET);
        break;

    case CLIP_EVENT_COMMAND_NOT_FOUND:        
        printf(COLOR_RED);
        if (*event_arg->command_not_found.cmd_name == '\0') {
            if (event_arg->command_not_found.cmd) {
                printf("missing subcommand, <%s> needs subcommand! (use \"%s\" for more details)", event_arg->command_not_found.cmd->name, CLIP_CONFIG_HELP_COMMAND);
            } else {
                printf("command required!");
            }
        } else {
            printf("<%s> not found!", event_arg->command_not_found.cmd_name);
        }
        printf(COLOR_RESET "\n");
        break;
    
    case CLIP_EVENT_CALL_COMMAND_CALLBACK:
        printf("call command callback for <%s> with args <%s>\n", event_arg->call_command_callback.cmd->name, event_arg->call_command_callback.cmd_line);
        break;
    
    case CLIP_EVENT_ARGUMENTS_ERROR: {
        char buf[128];
        clip_utils_arg_get_command_usage_string(buf, sizeof(buf), event_arg->arguments_error.cmd);
        printf(COLOR_RED "error: %s\n" COLOR_RESET, clip_utils_arg_get_error_string(event_arg->arguments_error.error));
        printf(COLOR_YELLOW "usage: %s\n" COLOR_RESET, buf);
        break;
    }

    default:
        printf(COLOR_RED "unsupported event: %d\n" COLOR_RESET, (int)event);
        break;
    }
}
extern const struct clip_command g_adc_cmd;
extern const struct clip_command g_gpio_cmd;
extern const struct clip_command g_mem_cmd;
extern const struct clip_command g_exit_cmd;

static struct app_context g_app_context;

CLIP_DEF_ROOT(g_clip, &g_app_context, event_callback)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_adc_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_gpio_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_mem_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_exit_cmd)
CLIP_DEF_ROOT_END()

int main(int argc, char *argv[])
{
    /* allocate buffer for command line */
    char buf[128];

    /* init random */
    srand(time(0));

    do {
        /* print prompt */
        printf("> ");
        /* read line from stdin */
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;
        /* remove new line character at the end, ensure it will be zero-ended */
        buf[strlen(buf) - 1] = '\0';
        /* do not feed clip with empty command line */
        if (strlen(buf) == 0)
            continue;

        /* parse command line */
        clip_cmd_parse_line(&g_clip, NULL, buf, NULL);

        /* repeat all above until application exit */
    } while (g_app_context.exit_app == 0);
    
    /* exit without error code */
    return 0;
}
