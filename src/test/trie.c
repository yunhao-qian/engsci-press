#include "test/global.h"

int test_count = 0, pass_count = 0;

Trie *dictionary;

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

void save_dictionary(const char *file_name) {
    Array *entries = traverse_trie(dictionary);
    CHECK_INT(dictionary->size, entries->size);
    FILE *stream = fopen(file_name, "wb");
    for (int i = 0; i < entries->size; ++i) {
        write_dict_entry((DictEntry *)(entries->data[i]), stream);
    }
    fclose(stream);
    delete_array(entries);
}

void test_search() {
    Array *entries = traverse_trie(dictionary), *results;
    DictEntry *entry;
    for (int i = 0; i < entries->size; ++i) {
        entry = entries->data[i];
        results = trie_search(dictionary, entry->headword, false);
        CHECK_BOOL(true, results->size > 0);
        delete_array(results);
    }
    delete_array(entries);
}

void test_predecessor() {
    Array *entries = traverse_trie(dictionary);
    String *s1, *s2;
    s1 = to_lower(((DictEntry *)(entries->data[entries->size - 1]))->headword);
    int entry_count = 0;
    Array *results;
    while (true) {
        results = trie_search(dictionary, s1, false);
        entry_count += results->size;
        delete_array(results);
        s2 = trie_predecessor(dictionary, s1);
        if (!s2) {
            delete_string(s1);
            break;
        }
        CHECK_BOOL(true, strcmp(s1->text, s2->text) > 0);
        delete_string(s1);
        s1 = s2;
    }
    delete_array(entries);
    CHECK_INT(dictionary->size, entry_count);
}

void test_successor() {
    Array *entries = traverse_trie(dictionary);
    String *s1, *s2;
    s1 = to_lower(((DictEntry *)(entries->data[0]))->headword);
    int entry_count = 0;
    Array *results;
    while (true) {
        results = trie_search(dictionary, s1, false);
        entry_count += results->size;
        delete_array(results);
        s2 = trie_successor(dictionary, s1);
        if (!s2) {
            delete_string(s1);
            break;
        }
        CHECK_BOOL(true, strcmp(s1->text, s2->text) < 0);
        delete_string(s1);
        s1 = s2;
    }
    delete_array(entries);
    CHECK_INT(dictionary->size, entry_count);
}

void test_remove() {
    Array *entries = traverse_trie(dictionary);
    Array *headwords = new_array(delete_string);
    String *headword;
    for (int i = 0; i < entries->size; ++i) {
        headword = ((DictEntry *)(entries->data[i]))->headword;
        array_append(headwords, new_string(headword->text, headword->size));
    }
    delete_array(entries);
    for (int i = 0; i < headwords->size; ++i) {
        trie_remove(dictionary, (String *)(headwords->data[i]), true);
    }
    CHECK_INT(0, dictionary->size);
    delete_array(headwords);
}

int main() {
    create_dictionary();
    save_dictionary("rewrite_dictionary.txt");
    test_search();
    test_predecessor();
    test_successor();
    test_remove();
    delete_trie(dictionary);
    DISPLAY_TEST_RESULT();
}