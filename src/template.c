#include "../include/rtl_generator.h"

#define MAX_PORTS 16
#define MIN_WIDTH 1
#define MAX_WIDTH 128

static void fail_with_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

static int is_valid_module_name(const char *name) {
    size_t len = 0;

    if (name == NULL || name[0] == '\0') {
        return 0;
    }

    if (name[0] >= '0' && name[0] <= '9') {
        return 0;
    }

    for (len = 0; name[len] != '\0'; len++) {
        char c = name[len];
        int is_alpha = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        int is_digit = (c >= '0' && c <= '9');
        int is_underscore = (c == '_');

        if (!is_alpha && !is_digit && !is_underscore) {
            return 0;
        }

        if (len >= 255) {
            return 0;
        }
    }

    return 1;
}

static void parse_port_widths(const char *arg, int *widths, int *count, const char *option_name) {
    const char *p = arg;
    int parsed_count = 0;

    if (arg == NULL || arg[0] == '\0') {
        fail_with_error("Port width list is empty");
    }

    while (*p != '\0') {
        char *end = NULL;
        long width = 0;

        if (parsed_count >= MAX_PORTS) {
            char message[128];
            snprintf(message, sizeof(message), "%s supports at most %d ports", option_name, MAX_PORTS);
            fail_with_error(message);
        }

        if (*p < '0' || *p > '9') {
            char message[128];
            snprintf(message, sizeof(message), "Invalid width format in %s", option_name);
            fail_with_error(message);
        }

        width = strtol(p, &end, 10);
        if (end == p) {
            char message[128];
            snprintf(message, sizeof(message), "Invalid width in %s", option_name);
            fail_with_error(message);
        }

        if (width < MIN_WIDTH || width > MAX_WIDTH) {
            char message[128];
            snprintf(message, sizeof(message), "Port width in %s must be between %d and %d", option_name, MIN_WIDTH, MAX_WIDTH);
            fail_with_error(message);
        }

        widths[parsed_count++] = (int)width;

        if (*end == '\0') {
            break;
        }

        if (*end != ',') {
            char message[128];
            snprintf(message, sizeof(message), "Widths in %s must be comma-separated", option_name);
            fail_with_error(message);
        }

        p = end + 1;
        if (*p == '\0') {
            char message[128];
            snprintf(message, sizeof(message), "Trailing comma in %s", option_name);
            fail_with_error(message);
        }
    }

    if (parsed_count < 1) {
        char message[128];
        snprintf(message, sizeof(message), "%s requires at least one port width", option_name);
        fail_with_error(message);
    }

    *count = parsed_count;
}

void parse_arguments(int argc, char *argv[], ModuleConfig *config) {
    if (argc < 2) {
        print_help();
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(0);
        } else if (strcmp(argv[i], "--module") == 0) {
            if (i + 1 >= argc) {
                fail_with_error("--module requires a value");
            }

            if (!is_valid_module_name(argv[i + 1])) {
                fail_with_error("Invalid module name (use letters, numbers, underscore, and do not start with a digit)");
            }

            strncpy(config->module_name, argv[i + 1], sizeof(config->module_name) - 1);
            config->module_name[sizeof(config->module_name) - 1] = '\0';
            i++;
        } else if (strcmp(argv[i], "--inputs") == 0) {
            if (i + 1 >= argc) {
                fail_with_error("--inputs requires a value");
            }

            parse_port_widths(argv[i + 1], config->input_width, &config->input_count, "--inputs");
            i++;
        } else if (strcmp(argv[i], "--outputs") == 0) {
            if (i + 1 >= argc) {
                fail_with_error("--outputs requires a value");
            }

            parse_port_widths(argv[i + 1], config->output_width, &config->output_count, "--outputs");
            i++;
        } else {
            char message[256];
            snprintf(message, sizeof(message), "Unknown option: %s", argv[i]);
            fail_with_error(message);
        }
    }
}