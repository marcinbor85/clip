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

#include <Arduino.h>

#include <clip.h>

#include "cli.h"

/**
 * @brief       Command handler for "gpio set" command.
 *              It receive 2 arguments:
 *              argv[0] is uint pin number
 *              argv[1] is uint pin state to set
 * @param[in]   self
 *              Pointer to root clip handler descriptor.
 * @param[in]   cmd
 *              Pointer to command which invoked this callback.
 * @param[in]   argc
 *              Number of arguments passed by command line.
 * @param[in]   argv
 *              List or already parsed arguments.
 * @param[in]   context
 *              Generic context pointer passed to "clip_cmd_parse_line" function.
*/
void cli_gpio_set_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
  // get argument 0 as uint
  uint8_t pin = argv[0].val_uint;
  // get argument 1 as uint
  uint8_t state = argv[1].val_uint;

  // set pin mode to output
  pinMode(pin, OUTPUT);
  // write pin state
  digitalWrite(pin, state);

  // print message
  Serial.print("SET pin[");
  Serial.print(pin);
  Serial.print("] = ");
  Serial.println(state);
}

/**
 * @brief       Command handler for "gpio get" command.
 *              It receive 1 arguments:
 *              argv[0] is uint pin number
 * @param[in]   self
 *              Pointer to root clip handler descriptor.
 * @param[in]   cmd
 *              Pointer to command which invoked this callback.
 * @param[in]   argc
 *              Number of arguments passed by command line.
 * @param[in]   argv
 *              List or already parsed arguments.
 * @param[in]   context
 *              Generic context pointer passed to "clip_cmd_parse_line" function.
*/
void cli_gpio_get_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
  // get argument 0 as uint
  uint8_t pin = argv[0].val_uint;

  // set pin mode to input
  pinMode(pin, INPUT);
  // read pin state
  uint8_t state = digitalRead(pin);

  // print message
  Serial.print("GET pin[");
  Serial.print(pin);
  Serial.print("] = ");
  Serial.println(state);
}

/**
 * @brief       Command handler for "echo" command.
 *              It receive 1 arguments:
 *              argv[0] is string text
 * @param[in]   self
 *              Pointer to root clip handler descriptor.
 * @param[in]   cmd
 *              Pointer to command which invoked this callback.
 * @param[in]   argc
 *              Number of arguments passed by command line.
 * @param[in]   argv
 *              List or already parsed arguments.
 * @param[in]   context
 *              Generic context pointer passed to "clip_cmd_parse_line" function.
*/
void cli_echo_callback(const struct clip *self, const struct clip_command *cmd, size_t argc, struct clip_arg_value argv[], void *context)
{
  // get argument 0 as string
  char *text = argv[0].val_str;

  // print echo message
  Serial.println(text);
}

/**
 * @brief       Event callback handler.
 * @param[in]   self
 *              Pointer to root clip handler descriptor.
 * @param[in]   event
 *              Event type
 * @param[in]   event_arg
 *              Event arguments
 * @param[in]   context
 *              Generic context pointer passed to "clip_cmd_parse_line" function.
*/
void cli_event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context)
{
  switch (event) {
  case CLIP_EVENT_HELP:
    // print help message. here its just list of supported commands
    Serial.println("supported commands:");
    while (*event_arg->help.commands != NULL) {
      Serial.println((*(event_arg->help.commands))->name);
      event_arg->help.commands++;
    }
    break;

  case CLIP_EVENT_COMMAND_NOT_FOUND:
    // print error when command was not found
    if (*event_arg->command_not_found.cmd_name == '\0') {
      if (event_arg->command_not_found.cmd) {
        Serial.println("missing subcommand!");
      } else {
        Serial.println("command required!");
      }
    } else {
      Serial.println("command not found!");
    }
    break;
  
  case CLIP_EVENT_CALL_COMMAND_CALLBACK:
    // generic call command callback. not used in this implementation.
    break;
  
  case CLIP_EVENT_ARGUMENTS_ERROR: {
    // error during parsing arguments. e.g. print command usage hint.
    static char usage[64];
    clip_utils_arg_get_command_usage_string(usage, sizeof(usage), event_arg->arguments_error.cmd);
    Serial.print("error: ");
    Serial.println(clip_utils_arg_get_error_string(event_arg->arguments_error.error));
    Serial.print("usage: ");
    Serial.println(usage);
    break;
  }

  default:
    break;
  }
}

/**
 * @brief       Function used to print prompt
*/
void show_prompt() {
  Serial.print("> ");
}

/**
 * @brief       Function used to read line (until new line char) from Serial.
 * @param[in]   on_read_line
 *              Pointer to function which will be called on new line.
*/
void read_line(void (*on_read_line)(char*)) {
  static String buf;
  char* ret = nullptr;
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') {
      Serial.println();
      if (buf.length() != 0) {
        on_read_line(buf.c_str());
        buf = "";
      }
      show_prompt();
    } else {
      Serial.print(ch);
      buf += ch;
    }
  }
}

/**
 * @brief       Setup function.
*/
void setup() {
  Serial.begin(9600);
  show_prompt();
}

/**
 * @brief       Main loop function.
*/
void loop() {
  // async read line from Serial 
  read_line([](char *cmd_line) {
    // feed CLIP with command line
    clip_cmd_parse_line(&g_cli_clip, NULL, cmd_line, NULL);
  });
}
