#include "main/utility.h"

int parse_unsigned_int_flag(const String *string) {
    assert(string_start_with(string, "--") &&
           "parse_unsigned_int_flag: not a flag");
    if (string->size <= 2) {
        return -1;
    }
    String *flag = substring(string, 2, string->size);
    for (int i = 0; i < flag->size; ++i) {
        if (!isdigit(flag->text[i])) {
            delete_string(flag);
            return -1;
        }
    }
    int number = atoi(flag->text);
    delete_string(flag);
    return number;
}

void word_hint(const String *string, const Trie *dictionary,
               bool case_sensitive) {
    String *matched = trie_closest_match(dictionary, string, -1);
    if (!matched) {
        return;
    }
    if (case_sensitive) {
        String *lowered = to_lower(string);
        if (!strcmp(matched->text, lowered->text)) {
            printf("Tip: use lower-case word for case-insensitive "
                   "search/remove.\n");
            delete_string(lowered);
            return;
        }
        delete_string(lowered);
    }
    printf("Did you mean: %s\n", matched->text);
    delete_string(matched);
}