#include "test/global.h"

int test_count = 0, pass_count = 0;

Trie *dictionary;

// Copy and paste from test/trie.c.
void create_dictionary() {
    dictionary = new_trie();
    char file_name[] = "../Dictionary-in-csv/*.csv";
    int index = strchr(file_name, '*') - file_name;
    FILE *stream;
    String *line;
    DictEntry *entry;
    for (char c = 'A'; c <= 'Z'; ++c) {
        file_name[index] = c;
        stream = fopen(file_name, "r");
        if (!stream) {
            WARNING("Cannot open file: %s\n", file_name);
            continue;
        }
        line = get_line(stream);
        while (line) {
            if (line->size > 0) {
                entry = new_dict_entry(line);
                if (!entry) {
                    WARNING("Failed to parse the following line in %s:\n%s\n",
                            file_name, line->text);
                } else {
                    trie_insert(dictionary, entry);
                }
            }
            delete_string(line);
            line = get_line(stream);
        }
        fclose(stream);
    }
}

void test_closest_match() {
    Array *entries = traverse_trie(dictionary);
    DictEntry *entry;
    String *lowered, *result;
    for (int i = 0; i < entries->size; ++i) {
        entry = entries->data[i];
        lowered = to_lower(entry->headword);
        result = trie_closest_match(dictionary, entry->headword, -1);
        CHECK_BOOL(true, result != NULL);
        if (result) {
            CHECK_STRING(lowered->text, result);
            delete_string(result);
        }
        delete_string(lowered);
    }
    delete_array(entries);
}

int main() {
    create_dictionary();
    test_closest_match();
    delete_trie(dictionary);
    DISPLAY_TEST_RESULT();
}