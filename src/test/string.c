#include "test/global.h"

int test_count = 0, pass_count = 0;

#define N 10

char *texts[N] = {
    "pen pineapple apple pie",
    "  banana nanana  ",
    "iPhone XS Max",
    "\tIt was the age of wisdom.\tIt was the age of foolishness.\t",
    "gcc -g string.c -o string.o",
    "Engineering Science",
    " University of Toronto ",
    " \t \t \t \t ",
    "Designed by Apple in California",
    " ESC190 Teaching Team"};

String *strings[N];

void create_strings() {
    for (int i = 0; i < N; ++i) {
        strings[i] = new_string(texts[i], -1);
    }
}

void discard_strings() {
    for (int i = 0; i < N; ++i) {
        delete_string(strings[i]);
    }
}

char find_chars[N] = {'a', 'b', 'x', 'F', 'e', 'S', 'g', 'h', 'i', 't'};

int expect_indices[N] = {8, 2, 12, -1, -1, 12, -1, -1, 3, -1};

void test_index() {
    int index;
    create_strings();
    for (int i = 0; i < N; ++i) {
        index = string_index(strings[i], find_chars[i]);
        CHECK_INT(expect_indices[i], index);
    }
    discard_strings();
}

char *lower_expect[N] = {
    "pen pineapple apple pie",
    "  banana nanana  ",
    "iphone xs max",
    "\tit was the age of wisdom.\tit was the age of foolishness.\t",
    "gcc -g string.c -o string.o",
    "engineering science",
    " university of toronto ",
    " \t \t \t \t ",
    "designed by apple in california",
    " esc190 teaching team"};

void test_to_lower() {
    String *lowered;
    create_strings();
    for (int i = 0; i < N; ++i) {
        lowered = to_lower(strings[i]);
        CHECK_STRING(lower_expect[i], lowered);
        delete_string(lowered);
        to_lower_in_place(strings[i]);
        CHECK_STRING(lower_expect[i], strings[i]);
    }
    discard_strings();
}

char *trim_expect[N] = {
    "pen pineapple apple pie",
    "banana nanana",
    "iPhone XS Max",
    "It was the age of wisdom.\tIt was the age of foolishness.",
    "gcc -g string.c -o string.o",
    "Engineering Science",
    "University of Toronto",
    "",
    "Designed by Apple in California",
    "ESC190 Teaching Team"};

void test_trim() {
    String *trimmed;
    create_strings();
    for (int i = 0; i < N; ++i) {
        trimmed = trim(strings[i]);
        CHECK_STRING(trim_expect[i], trimmed);
        delete_string(trimmed);
        trim_in_place(strings[i]);
        CHECK_STRING(trim_expect[i], strings[i]);
    }
    discard_strings();
}

int substring_indices[N][2] = {{4, 13}, {9, 15},  {7, 9}, {19, 25}, {7, 15},
                               {0, 11}, {15, 22}, {5, 8}, {9, 17},  {6, 16}};

char *substring_expect[N] = {"pineapple", "nanana",      "XS",      "wisdom",
                             "string.c",  "Engineering", "Toronto", "\t \t",
                             "by Apple",  "0 Teaching"};

void test_substring() {
    String *result;
    create_strings();
    for (int i = 0; i < N; ++i) {
        result = substring(strings[i], substring_indices[i][0],
                           substring_indices[i][1]);
        CHECK_STRING(substring_expect[i], result);
        delete_string(result);
        substring_in_place(strings[i], substring_indices[i][0],
                           substring_indices[i][1]);
        CHECK_STRING(substring_expect[i], strings[i]);
    }
    discard_strings();
}

int split_counts[N] = {4, 2, 3, 12, 5, 2, 3, 0, 5, 3};

char *split_fields[N][15] = {{"pen", "pineapple", "apple", "pie"},
                             {"banana", "nanana"},
                             {"iPhone", "XS", "Max"},
                             {"It", "was", "the", "age", "of", "wisdom.", "It",
                              "was", "the", "age", "of", "foolishness."},
                             {"gcc", "-g", "string.c", "-o", "string.o"},
                             {"Engineering", "Science"},
                             {"University", "of", "Toronto"},
                             {},
                             {"Designed", "by", "Apple", "in", "California"},
                             {"ESC190", "Teaching", "Team"}};

void test_split() {
    Array *array;
    create_strings();
    for (int i = 0; i < N; ++i) {
        array = split_string(strings[i]);
        CHECK_INT(split_counts[i], array->size);
        if (split_counts[i] != array->size) {
            continue;
        }
        for (int j = 0; j < split_counts[i]; ++j) {
            CHECK_STRING(split_fields[i][j], (String *)(array->data[j]));
        }
        delete_array(array);
    }
    discard_strings();
}

int join_counts[N] = {4, 2, 3, 12, 5, 2, 3, 1, 5, 3};

char *join_fields[N][15] = {{"pen", "pineapple", "apple", "pie"},
                            {"banana", "nanana"},
                            {"iPhone", "XS", "Max"},
                            {"It", "was", "the", "age", "of", "wisdom.", "It",
                             "was", "the", "age", "of", "foolishness."},
                            {"gcc", "-g", "string.c", "-o", "string.o"},
                            {"Engineering", "Science"},
                            {"University", "of", "Toronto"},
                            {"t"},
                            {"Designed", "by", "Apple", "in", "California"},
                            {"ESC190", "Teaching", "Team"}};

char *join_expect[N] = {
    "pen pineapple apple pie",
    "banana nanana",
    "iPhone XS Max",
    "It was the age of wisdom. It was the age of foolishness.",
    "gcc -g string.c -o string.o",
    "Engineering Science",
    "University of Toronto",
    "t",
    "Designed by Apple in California",
    "ESC190 Teaching Team"};

void test_join() {
    Array *strings;
    String *field, *joined;
    for (int i = 0; i < N; ++i) {
        strings = new_array(delete_string);
        for (int j = 0; j < join_counts[i]; ++j) {
            field = new_string(join_fields[i][j], -1);
            array_append(strings, field);
        }
        joined = join_strings(strings, ' ');
        CHECK_STRING(join_expect[i], joined);
        delete_string(joined);
        delete_array(strings);
    }
}

int main() {
    test_index();
    test_to_lower();
    test_substring();
    test_trim();
    test_split();
    test_join();
    DISPLAY_TEST_RESULT();
}