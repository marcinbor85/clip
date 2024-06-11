# CLIP
CLIP is an abbreviation of Command Line Interface Parser. Implementation is platform- and interface- agnostic. Input data could comes from any source, it could be UART, USB, TCP stream, or even memory-mapped device. User application is responsible for feeding parser with proper command line arguments. The library doesn't care where the data comes from or how it gets to it. CLIP only expects that the complete data will be allocated in a buffer with the possibility of data modification. There is no internal working buffers or dynamic memory allocation. Thanks to that CLIP is blazing fast and extremely lightweight.

## Features
- support quotemarks (allow uses spaces in arguments)
- support escape chars (allow uses quotemarks in arguments)
- support command names with spaces
- support nesting commands (no deep limits)
- no dynamic memory allocation (no memory leaks, fully predictable)
- no internal buffers (all operations performed in-place on input buffer)
- automatic required commands arguments parsing
- optional configurable special "help" command
- event driven callback (there are no real asynchronous events, its rather a way of app notification)
- tiny memory footprint (in flash and ram)
- fully stateless (all internal structures and data are const and static)
- input source data agnostic (command line arguments could come from any source)
- hardware agnostic (no hardware dependencies)
- Arduino compatible
- pure C implementation
- fully covered with unit tests

## Usage

See working example on linux:

```bash
cmake -B build .
cd build
make
make test
./example/linux/example
```
![alt text](https://github.com/marcinbor85/clip/blob/main/assets/linux_terminal.png?raw=true "Linux Terminal")

See working example on Arduino:

![alt text](https://github.com/marcinbor85/clip/blob/main/assets/arduino_serial_monitor.png?raw=true "Arduino Serial Monitor")

### Supported argument types

Argument values after parsing are stored in structure which contains value type and the value itself. Access to the correct value field must be performed by checking argument type. All fields except "val_hexarray" can be accessed directly. Hex arrays needs to be unpacked first (they are encoded in-place using lightweight Length-Value encoding for reducing stack usage). Unpacking is very lightweight operation, it only finds a raw data length and raw data pointer.

```c
///< structure contains parsed argument value in union format
struct clip_arg_value {
    clip_arg_type_t type;               ///< argument value type
    union {
        char *val_str;                  ///< pointer to value string (for CLIP_ARG_TYPE_STRING)
        bool val_bool;                  ///< bool value (for CLIP_ARG_TYPE_BOOL)
        int32_t val_int;                ///< signed integer value (for CLIP_ARG_TYPE_INT)
        uint32_t val_uint;              ///< unsigned integer value (for CLIP_ARG_TYPE_UINT)
        float val_float;                ///< float number value (for CLIP_ARG_TYPE_FLOAT)
        clip_hexarray_t val_hexarray;   ///< array of bytes (for CLIP_ARG_TYPE_HEXARRAY)
    };
};
```

| clip_arg_type_t        | field with value | access                                       |
|------------------------|------------------|----------------------------------------------|
| CLIP_ARG_TYPE_STRING   | val_str          | direct                                       |
| CLIP_ARG_TYPE_BOOL     | val_bool         | direct                                       |
| CLIP_ARG_TYPE_INT      | val_int          | direct                                       |
| CLIP_ARG_TYPE_UINT     | val_uint         | direct                                       |
| CLIP_ARG_TYPE_FLOAT    | val_float        | direct                                       |
| CLIP_ARG_TYPE_HEXARRAY | val_hexarray     | unpack with "clip_utils_arg_unpack_hexarray" | 

### Defining commands tree

Both, root commands and subcommands could be defined in one file or divided into separated modules. Thanks to that for simple apps everything could be defined in one single place, and for bigger projects, everything could be separated logically. This increases flexibility.

Each command supports any number of nested subcommand. Each subcommand also supports any number of
another nested subcommands. There is no software limitation. The only limitation may come from the memory used by the recursive call to the parsing function. However the stack usage per function call is not so big. There are stored only 2 local pointers and function arguments. In real use case this is not a major limitation.

Commands tree definition could be implemented manually by filling structures properly, or by helping macros. Second option is more readable and has no additional overheads.

Commands arguments are optional. If they will not be defined, then all arguments will be parsed as strings. If arguments will be defined explicitly, then parsing (and decoding) of these arguments will be processed automatically. In this case user will get already parsed values (integer, floats, or even raw binary data). Additional extra arguments which will be provided after required onces are parsed as strings.

```c

/* Definition of main clip handler */
CLIP_DEF_ROOT(g_clip, &g_app_context, event_callback)
    /* there will be 4 root commands: adc, gpio, mem, exit */
    CLIP_DEF_ADD_ROOT_COMMAND(&g_adc_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_gpio_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_mem_cmd)
    CLIP_DEF_ADD_ROOT_COMMAND(&g_exit_cmd)
CLIP_DEF_ROOT_END()

/* Definition of command: adc */
CLIP_DEF_ROOT_COMMAND(g_adc_cmd, "adc", "control adc driver", NULL)
    /* there will be 4 subcommands: read, set, start, stop */
    CLIP_DEF_COMMAND("read", "read input", adc_read_callback) CLIP_DEF_WITH_ARGS()
        /* there will be 1 required argument in "adc read" command */
        CLIP_DEF_ARGUMENT("channel", "channel number", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("set", "set reference", adc_set_callback) CLIP_DEF_WITH_ARGS()
        /* there will be 1 required argument in "adc set" command */
        CLIP_DEF_ARGUMENT("vref", "voltage reference", CLIP_ARG_TYPE_FLOAT)
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("start", "start sampling", adc_start_callback) CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("stop", "stop sampling", adc_stop_callback) CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

/* Definition of command: gpio */
CLIP_DEF_ROOT_COMMAND(g_gpio_cmd, "gpio", "control gpio", NULL)
    /* there will be 2 subcommands: get, set */
    CLIP_DEF_COMMAND("get", "get methods", NULL)
        /* there will be 2 subcommands: pin, reg */
        CLIP_DEF_COMMAND("pin", "get pin state", gpio_get_pin_callback) CLIP_DEF_WITH_ARGS()
            /* there will be 1 required argument in "gpio get pin" command */
            CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END()
        CLIP_DEF_COMMAND("reg", "get register value", gpio_get_reg_callback) CLIP_DEF_WITH_ARGS()
            /* there will be 1 required argument in "gpio get reg" command */
            CLIP_DEF_ARGUMENT("address", "register address", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("set", "set methods", NULL)
        /* there will be 2 subcommands: pin, reg */
        CLIP_DEF_COMMAND("pin", "set pin state", gpio_set_pin_callback) CLIP_DEF_WITH_ARGS()
            /* there will be 2 required arguments in "gpio set pin" command */
            CLIP_DEF_ARGUMENT("pin", "pin number", CLIP_ARG_TYPE_UINT)
            CLIP_DEF_ARGUMENT("state", "pin state", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END()
        CLIP_DEF_COMMAND("reg", "set register value", gpio_set_reg_callback) CLIP_DEF_WITH_ARGS()
            /* there will be 2 required arguments in "gpio set reg" command */
            CLIP_DEF_ARGUMENT("address", "register address", CLIP_ARG_TYPE_UINT)
            CLIP_DEF_ARGUMENT("value", "register value", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

/* Definition of command: mem */
CLIP_DEF_ROOT_COMMAND(g_mem_cmd, "mem", "memory driver", NULL)
    /* there will be 2 subcommands: write, read */
    CLIP_DEF_COMMAND("write", "write data to memory", mem_write_callback) CLIP_DEF_WITH_ARGS()
        /* there will be 2 required arguments in "mem write" command */
        CLIP_DEF_ARGUMENT("address", "address to write", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_ARGUMENT("data", "binary data to write", CLIP_ARG_TYPE_HEXARRAY)
    CLIP_DEF_COMMAND_END()
    CLIP_DEF_COMMAND("read", "read data from memory", mem_read_callback) CLIP_DEF_WITH_ARGS()
        /* there will be 2 required arguments in "mem read" command */
        CLIP_DEF_ARGUMENT("address", "address to read", CLIP_ARG_TYPE_UINT)
        CLIP_DEF_ARGUMENT("size", "number of bytes to read", CLIP_ARG_TYPE_UINT)
    CLIP_DEF_COMMAND_END()
CLIP_DEF_ROOT_COMMAND_END()

/* Definition of command: exit */
CLIP_DEF_ROOT_COMMAND(g_exit_cmd, "exit", "application exit", exit_callback)
CLIP_DEF_ROOT_COMMAND_END()

```

### Implementation of event callback

Event callback is used to notify the application about events occurring during parsing. They are called from "clip_cmd_parse_line" function context. And as long as "clip_cmd_parse_line" is synchronous, all events also are fully synchronous. Event callback is optional and if the application doesn't need special implementation, all of these events could be left unimplemented. Or even whole callback could be ommited (use NULL pointer).

```c
static void event_callback(const struct clip *self, clip_event_t event, union clip_event_arg *event_arg, void *context)
{
    switch (event) {
    case CLIP_EVENT_HELP:
        // app is responsible for defining what should done here
        // e.g. print list of supported commands in app-specific format
        break;

    case CLIP_EVENT_COMMAND_NOT_FOUND:
        // app is responsible for defining what should done here
        // e.g. print error message
        break;
    
    case CLIP_EVENT_CALL_COMMAND_CALLBACK:
        // app is responsible for defining what should done here
        // this is an optional callback which aggregate all commands calls
        // its usefull for manual parsing special commands and arguments or implement custom dispatcher
        break;
    
    case CLIP_EVENT_ARGUMENTS_ERROR:
        // app is responsible for defining what should done here
        // e.g. print error message with command usage hints
        break;
    }
}
```

### Feeding parser with command line

Command line passed to CLIP parser must be completed (no chunks, no parts) and must be allocated in RAM (no matter where, it could be heap, stack or global data space). It's important, because this input buffer with command line content will be modified during parsing (parser will change its content, e.g. for finding commands or subcommands, parsing arguments, or decoding hex arrays from ascii hex to binary data). So if the application needs to keep the content, then it needs to be copied and the application is responsible for it. There is no risk of buffer overflow. Parser will not modify data outside this buffer (it needs to be zero-ended).

```c
int main(int argc, char *argv[])
{
    /* allocate buffer for command line */
    char buf[128];

    /* main loop */
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
```
