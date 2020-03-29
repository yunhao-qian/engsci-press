#include "test/global.h"

int test_count = 0, pass_count = 0;

#define N 15

const char *lines[N] = {
    " Apple pie  ( n. )  A kind of food that is made of apple. ",
    "\"Banana ((n.) A long and yellow fruit.\"",
    "California (n.) A state of USA.",
    "\" Drag  ( v. ))  To pull forcefully. \"",
    "Delta-Epsilon (n.) A mathematical language that defines derivatives.",
    "\"Fine adj. Good.\"",
    "Gay (adj.) Happy and joyful.",
    "\"Hike     (   v.   )     Travel on foot.\"",
    " University of Toronto  ( n. )  A university in Ontario. ",
    "\"Iceberg (n.) A big piece of ice that floats on the sea.\"",
    " \"a ( ( b ) ) c\"",
    "a ( ( b ) ( ) ))  c",
    "a ( ( ( b ) ) c",
    "\" a b c () \"",
    "a b () b c c () \""};

bool expect_success[N] = {
    true, false, true, true, true,  false, true, true,
    true, true,  true, true, false, true,  true,
};

const char *expect_entries[N][3] = {
    {"Apple pie", "n.", "A kind of food that is made of apple."},
    {},
    {"California", "n.", "A state of USA."},
    {"Drag", "v.", ")  To pull forcefully."},
    {"Delta-Epsilon", "n.",
     "A mathematical language that defines derivatives."},
    {},
    {"Gay", "adj.", "Happy and joyful."},
    {"Hike", "v.", "Travel on foot."},
    {"University of Toronto", "n.", "A university in Ontario."},
    {"Iceberg", "n.", "A big piece of ice that floats on the sea."},
    {"\"a", "( b )", "c\""},
    {"a", "( b ) ( )", ")  c"},
    {},
    {"a b c", "", ""},
    {"a b", "", "b c c () \""}};

void test_new_dict_entry() {
    DictEntry *entry;
    for (int i = 0; i < N; ++i) {
        String *line = new_string(lines[i], -1);
        DictEntry *entry = new_dict_entry(line);
        delete_string(line);
        CHECK_BOOL(expect_success[i], (bool)entry);
        if (expect_success[i] && entry) {
            CHECK_STRING(expect_entries[i][0], entry->headword);
            CHECK_STRING(expect_entries[i][1], entry->word_class);
            CHECK_STRING(expect_entries[i][2], entry->definition);
        }
        if (entry) {
            delete_dict_entry(entry);
        }
    }
}

int main() {
    test_new_dict_entry();
    DISPLAY_TEST_RESULT();
}