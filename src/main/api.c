#include "main/api.h"

Trie *dictionary;

void esp_initialize(EspMode mode) {
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("===================="
               " Command-Line Dictionary by Yunhao Qian "
               "====================\n"
               "\nStarting...\n\n");
    }
    dictionary = new_trie();
    FILE *stream = fopen(".esp_rc", "r");
    if (stream) {
        String *line = get_line(stream);
        Array *arguments;
        while (line) {
            arguments = split_string(line);
            esp_parse_arguments(arguments, ESP_MODE_BACKGROUND);
            delete_array(arguments);
            delete_string(line);
        }
        fclose(stream);
    }
}

void esp_cleanup(EspMode mode) {
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("\nExiting...\n");
    }
}

bool esp_parse_arguments(Array *arguments, EspMode mode) {
    if (arguments->size < 0) {
        return true;
    }
    String *leading = to_lower(arguments->data[0]);
    array_remove(arguments, 0);
    bool returned = true;
    if (!strcmp(leading->text, "load")) {
        esp_on_load(arguments, mode);
    } else if (!strcmp(leading->text, "search")) {
        esp_on_search(arguments, mode);
    } else if (!strcmp(leading->text, "insert")) {
        esp_on_insert(arguments, mode);
    } else if (!strcmp(leading->text, "remove")) {
        esp_on_remove(arguments, mode);
    } else if (!strcmp(leading->text, "neighbour")) {
        esp_on_neighbour(arguments, mode);
    } else if (!strcmp(leading->text, "prefix")) {
        esp_on_prefix(arguments, mode);
    } else if (!strcmp(leading->text, "match")) {
        esp_on_match(arguments, mode);
    } else if (!strcmp(leading->text, "size")) {
        esp_on_size(arguments, mode);
    } else if (!strcmp(leading->text, "save")) {
        esp_on_save(arguments, mode);
    } else if (!strcmp(leading->text, "exit")) {
        returned = esp_on_exit(arguments, mode);
    } else {
        WARNING("Unknown leading argument: %s\n", leading->text);
    }
    delete_string(leading);
    return returned;
}

void esp_on_load(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_search(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_insert(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_remove(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_neighbour(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_prefix(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_match(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_size(const Array *arguments, EspMode mode) {
    ;
}

void esp_on_save(const Array *arguments, EspMode mode) {
    ;
}

bool esp_on_exit(const Array *arguments, EspMode mode) {
    return false;
}