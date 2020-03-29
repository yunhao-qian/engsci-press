#include "core/dict_entry.h"

DictEntry *new_dict_entry(const String *line) {
    int open_index = string_index(line, '(');
    if (open_index < 0) {
        return NULL;
    }
    int close_index = -1;
    for (int i = open_index + 1, depth = 1; i < line->size; ++i) {
        if (line->text[i] == '(') {
            ++depth;
        } else if (line->text[i] == ')' && --depth <= 0) {
            close_index = i;
            break;
        }
    }
    if (close_index < 0) {
        return NULL;
    }
    DictEntry *entry = malloc(sizeof(DictEntry));
    int begin_index = 0, end_index = line->size;
    if (line->text[0] == '"' && line->text[line->size - 1] == '"') {
        ++begin_index;
        --end_index;
    }
    entry->headword = substring(line, begin_index, open_index);
    trim_in_place(entry->headword);
    if (!is_valid_key(entry->headword, false)) {
        WARNING("Headword \"%s\"contains invalid characters.\n",
                entry->headword->text);
        delete_string(entry->headword);
        free(entry);
        return NULL;
    }
    entry->word_class = substring(line, open_index + 1, close_index);
    entry->definition = substring(line, close_index + 1, end_index);
    trim_in_place(entry->word_class);
    trim_in_place(entry->definition);
    return entry;
}

void delete_dict_entry(void *p) {
    DictEntry *entry = p;
    delete_string(entry->headword);
    delete_string(entry->word_class);
    delete_string(entry->definition);
    free(entry);
}

bool confirm(bool default_yes, const char *message) {
    if (message) {
        printf("%s ", message);
    }
    if (default_yes) {
        printf("([y]/n) ");
    } else {
        printf("(y/[n]) ");
    }
    bool returned;
    String *line = get_line(stdin);
    trim_in_place(line);
    to_lower_in_place(line);
    if (!strcmp(line->text, "y") || !strcmp(line->text, "yes")) {
        returned = true;
    } else if (!strcmp(line->text, "n") || !strcmp(line->text, "no")) {
        returned = false;
    } else {
        returned = default_yes;
    }
    delete_string(line);
    return returned;
}

DictEntry *input_dict_entry(const String *headword) {
    if (!is_valid_key(headword, false)) {
        printf("Headword contains invalid characters.\nDo nothing.\n");
        return NULL;
    }
    DictEntry *entry = malloc(sizeof(DictEntry));
    entry->headword = trim(headword);
    printf("Word class: ");
    entry->word_class = get_line(stdin);
    printf("Definition: ");
    entry->definition = get_line(stdin);
    trim_in_place(entry->word_class);
    trim_in_place(entry->definition);
    printf("Will create the following entry:\n");
    display_dict_entry(entry);
    if (!confirm(true, "Continue?")) {
        delete_dict_entry(entry);
        printf("Do nothing.\n");
        return NULL;
    } else {
        return entry;
    }
}

void display_dict_entry(const DictEntry *entry) {
    printf("%s\n%s\n%s\n", entry->headword->text, entry->word_class->text,
           entry->definition->text);
}

void write_dict_entry(const DictEntry *entry, FILE *stream) {
    fprintf(stream, "%s (%s) %s\n", entry->headword->text,
            entry->word_class->text, entry->definition->text);
}