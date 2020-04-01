#include "main/api.h"
#include "main/utility.h"

Trie *dictionary;

void esp_initialize(EspMode mode) {
    if (mode == ESP_MODE_INTERACTIVE) {
        printf("               *=============================================\n"
               "               ||  EngSci Press Dictionary by Yunhao Qian  ||\n"
               "               *=============================================\n"
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
        WARN("Unknown leading argument: %s\n", leading->text);
    }
    delete_string(leading);
    return returned;
}

void esp_on_load(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARN_NOT_SUPPORTED("load", "command-line");
        return;
    }
    if (arguments->size < 1) {
        WARN_MISSING("file name");
        return;
    }
    if (arguments->size > 1) {
        WARN_REDUNDANT(arguments, 1);
    }
    const char *file_name = ((String *)arguments->data[0])->text;
    FILE *stream = fopen(file_name, "r");
    if (!stream) {
        WARN("Cannot open file: %s\nDo nothing.\n", file_name);
        return;
    }
    String *line = get_line(stream);
    DictEntry *entry;
    int count = 0;
    while (line) {
        if (line->size > 0) {
            entry = new_dict_entry(line);
            if (!entry) {
                WARN("Failed to parse the following line in %s:\n%s\n",
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
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("search", "background");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("headword");
        return;
    }
    String *word = join_strings(arguments, ' ');
    bool case_sensitive = false;
    for (int i = 0; i < word->size; ++i) {
        if (isupper(word->text[i])) {
            case_sensitive = true;
            break;
        }
    }
    Array *results = trie_search(dictionary, word, case_sensitive);
    if (results->size <= 0) {
        WARN("Find no entry named: %s\n", word->text);
        word_hint(word, dictionary, case_sensitive);
    } else {
        for (int i = 0; i < results->size; ++i) {
            putchar('\n');
            display_dict_entry(results->data[i]);
        }
        putchar('\n');
    }
    delete_array(results);
    delete_string(word);
}

void esp_on_insert(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("insert", "background");
        return;
    }
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARN_NOT_SUPPORTED("insert", "command-line");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("headword");
        return;
    }
    String *headword = join_strings(arguments, ' ');
    DictEntry *entry = input_dict_entry(headword);
    if (entry) {
        trie_insert(dictionary, entry);
    }
    delete_string(headword);
}

void esp_on_remove(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARN_NOT_SUPPORTED("remove", "command-line");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("headword");
        return;
    }
    String *headword = join_strings(arguments, ' ');
    bool case_sensitive = false;
    for (int i = 0; i < headword->size; ++i) {
        if (isupper(headword->text[i])) {
            case_sensitive = true;
            break;
        }
    }
    Array *results = trie_search(dictionary, headword, case_sensitive);
    int remove_count = results->size;
    bool shall_remove = true;
    if (remove_count <= 0) {
        WARN("Find no entry named: %s\n", headword->text);
        word_hint(headword, dictionary, case_sensitive);
        shall_remove = false;
    } else if (mode == ESP_MODE_INTERACTIVE) {
        printf("The following entries will be removed:\n");
        for (int i = 0; i < remove_count; ++i) {
            putchar('\n');
            display_dict_entry(results->data[i]);
        }
        shall_remove = confirm(true, "\nWant to continue?");
    }
    delete_array(results);
    if (shall_remove) {
        trie_remove(dictionary, headword, case_sensitive);
        if (mode == ESP_MODE_INTERACTIVE) {
            printf("%d entries removed.\n", remove_count);
        }
    } else if (mode == ESP_MODE_INTERACTIVE) {
        printf("Do nothing.\n");
    }
}

void esp_on_neighbour(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("neighbour", "background");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("headword");
        return;
    }
    int radius = 10;
    if (string_start_with(arguments->data[0], "--")) {
        if (arguments->size <= 1) {
            WARN_MISSING("headword");
            return;
        }
        int number = parse_unsigned_int_flag(arguments->data[0]);
        if (number < 0) {
            WARN("Invalid flag: %s\n", ((String *)arguments->data[0])->text);
        } else {
            radius = number;
        }
        array_remove(arguments, 0);
    }
    String *word = join_strings(arguments, ' ');
    delete_string(word);
}

void esp_on_prefix(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("prefix", "background");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("prefix string");
        return;
    }
    int max_count = 10;
    if (string_start_with(arguments->data[0], "--")) {
        if (arguments->size <= 1) {
            WARN_MISSING("prefix string");
            return;
        }
        int number = parse_unsigned_int_flag(arguments->data[0]);
        if (number < 0) {
            WARN("Invalid flag: %s\n", ((String *)arguments->data[0])->text);
        } else {
            max_count = number;
        }
        array_remove(arguments, 0);
    }
    String *prefix = join_strings(arguments, ' ');
    delete_string(prefix);
}

void esp_on_match(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("match", "background");
        return;
    }
    if (arguments->size <= 0) {
        WARN_MISSING("headword");
        return;
    }
    int tolerance = -1;
    if (string_start_with(arguments->data[0], "--")) {
        if (arguments->size <= 1) {
            WARN_MISSING("headword");
            return;
        }
        int number = parse_unsigned_int_flag(arguments->data[0]);
        if (number < 0) {
            WARN("Invalid flag: %s\n", ((String *)arguments->data[0])->text);
        } else {
            tolerance = number;
        }
        array_remove(arguments, 0);
    }
    String *pattern = join_strings(arguments, ' ');
    String *matched = trie_closest_match(dictionary, pattern, tolerance);
    if (!matched) {
        WARN("Find no entry similar to: %s\n", pattern->text);
    } else {
        printf("%s\n", matched->text);
        delete_string(matched);
    }
    delete_string(pattern);
}

void esp_on_size(Array *arguments, EspMode mode) {
    if (mode == ESP_MODE_BACKGROUND) {
        WARN_NOT_SUPPORTED("size", "background");
        return;
    }
    if (arguments->size > 0) {
        WARN_REDUNDANT(arguments, 0);
    }
    printf("Dictionary size: %d\n", dictionary->size);
}

void esp_on_save(Array *arguments, EspMode mode) {
    if (arguments->size < 1) {
        WARN_MISSING("file name");
        return;
    }
    if (arguments->size > 1) {
        WARN_REDUNDANT(arguments, 1);
    }
    if (mode == ESP_MODE_INTERACTIVE && dictionary->size <= 0 &&
        !confirm(false, "The dictionary is empty. Continue?")) {
        printf("Do nothing.\n");
        return;
    }
    const char *file_name = ((String *)arguments->data[0])->text;
    FILE *stream = fopen(file_name, "wb");
    if (!stream) {
        WARN("Cannot open file: %s\nDo nothing.\n", file_name);
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
        WARN_NOT_SUPPORTED("exit", "background");
        return true;
    }
    if (mode == ESP_MODE_COMMAND_LINE) {
        WARN_NOT_SUPPORTED("exit", "command-line");
        return true;
    }
    if (arguments->size > 0) {
        WARN_REDUNDANT(arguments, 0);
    }
    return !confirm(true, "Are you sure you want to exit?");
}