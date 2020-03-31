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
    FILE *stream = fopen(ESP_RC_PATH, "r");
    if (stream) {
        String *line = get_line(stream);
        Array *arguments;
        while (line) {
            arguments = split_string(line);
            esp_parse_arguments(arguments, ESP_MODE_BACKGROUND);
            delete_array(arguments);
            delete_string(line);
            line = get_line(stream);
        }
        fclose(stream);
    }
}

void esp_cleanup(EspMode mode) {
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("\nExiting...\n");
    }
    delete_trie(dictionary);
}

bool esp_parse_arguments(Array *arguments, EspMode mode) {
    if (arguments->size <= 0) {
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

#define WARNING_NOT_SUPPORTED(argument, mode)                                  \
    WARNING("Does not support \"%s\" in %s mode.\n", argument, mode);

#define WARNING_MISSING(expected)                                              \
    WARNING("Missing argument: %s expected.\n", expected)

#define WARNING_REDUNDANT(arguments, start_index)                              \
    WARNING("Redundant arguments: ignore arguments since \"%s\".\n",           \
            ((String *)(arguments)->data[start_index])->text)

void esp_on_load(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARNING_NOT_SUPPORTED("load", "command-line");
        return;
    }
    if (arguments->size < 1) {
        WARNING_MISSING("file name");
        return;
    }
    if (arguments->size > 1) {
        WARNING_REDUNDANT(arguments, 1);
    }
    const char *file_name = ((String *)arguments->data[0])->text;
    FILE *stream = fopen(file_name, "r");
    if (!stream) {
        WARNING("Cannot open file: %s\nDo nothing.\n", file_name);
        return;
    }
    String *line = get_line(stream);
    DictEntry *entry;
    int count = 0;
    while (line) {
        if (line->size > 0) {
            entry = new_dict_entry(line);
            if (!entry) {
                WARNING("Failed to parse the following line in %s:\n%s\n",
                        file_name, line->text);
            } else {
                trie_insert(dictionary, entry);
                ++count;
            }
        }
        delete_string(line);
        line = get_line(stream);
    }
    fclose(stream);
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("%d entries loaded from %s\n", count, file_name);
    }
}

void esp_on_search(Array *arguments, EspMode mode) {
    ;
}

void esp_on_insert(Array *arguments, EspMode mode) {
    ;
}

void esp_on_remove(Array *arguments, EspMode mode) {
    ;
}

void esp_on_neighbour(Array *arguments, EspMode mode) {
    ;
}

void esp_on_prefix(Array *arguments, EspMode mode) {
    ;
}

void esp_on_match(Array *arguments, EspMode mode) {
    ;
}

void esp_on_size(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARNING_NOT_SUPPORTED("size", "background");
        return;
    }
    if (arguments->size > 0) {
        WARNING_REDUNDANT(arguments, 0);
    }
    printf("Dictionary size: %d\n", dictionary->size);
}

void esp_on_save(Array *arguments, EspMode mode) {
    if (arguments->size < 1) {
        WARNING_MISSING("file name");
        return;
    }
    if (arguments->size > 1) {
        WARNING_REDUNDANT(arguments, 1);
    }
    if (mode == ESP_MODE_INTERACTIVE && dictionary->size <= 0 &&
        !confirm(false, "The dictionary is empty. Continue?")) {
        printf("Do nothing.\n");
        return;
    }
    const char *file_name = ((String *)arguments->data[0])->text;
    FILE *stream = fopen(file_name, "wb");
    if (!stream) {
        WARNING("Cannot open file: %s\nDo nothing.\n", file_name);
        return;
    }
    Array *entries = traverse_trie(dictionary);
    for (int i = 0; i < entries->size; ++i) {
        write_dict_entry(entries->data[i], stream);
    }
    delete_array(entries);
    fclose(stream);
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("%d entries saved to %s.\n", dictionary->size, file_name);
    }
}

bool esp_on_exit(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARNING_NOT_SUPPORTED("exit", "background");
        return true;
    }
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARNING_NOT_SUPPORTED("exit", "command-line");
        return true;
    }
    if (arguments->size > 0) {
        WARNING_REDUNDANT(arguments, 0);
    }
    return !confirm(true, "Are you sure you want to exit?");
}