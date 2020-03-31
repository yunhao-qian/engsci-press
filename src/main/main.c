#include "main/api.h"

int main(int argc, const char **argv) {
    Array *arguments;
    if (argc > 1) {
        arguments = new_array(delete_string);
        for (int i = 1; i < argc; ++i) {
            array_append(arguments, new_string(argv[i], -1));
        }
        esp_initialize(ESP_MODE_COMMAND_LINE);
        esp_parse_arguments(arguments, ESP_MODE_COMMAND_LINE);
        delete_array(arguments);
        esp_cleanup(ESP_MODE_COMMAND_LINE);
        return 0;
    }
    esp_initialize(ESP_MODE_INTERACTIVE);
    String *line;
    bool shall_continue;
    do {
        printf(">>> ");
        line = get_line(stdin);
        if (!line) {
            break;
        }
        arguments = split_string(line);
        shall_continue = esp_parse_arguments(arguments, ESP_MODE_INTERACTIVE);
        delete_array(arguments);
        delete_string(line);
    } while (shall_continue);
    esp_cleanup(ESP_MODE_INTERACTIVE);
}