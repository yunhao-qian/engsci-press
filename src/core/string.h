#ifndef CORE_STRING_H_
#define CORE_STRING_H_

#include "core/array.h"

typedef struct String {
    char *text, *head;
    int size, capacity;
} String;

String *new_string(const char *text, int size);
void delete_string(void *string);

bool is_valid_key(const String *string, bool case_sensitive);
int string_index(const String *string, char value);

String *to_lower(const String *string);
void to_lower_in_place(String *string);

String *substring(const String *string, int start, int end);
void substring_in_place(String *string, int start, int end);

String *trim(const String *string);
void trim_in_place(String *string);

String *get_line(FILE *stream);
Array *split_string(const String *string);
String *join_strings(const Array *strings, char c);

#endif // CORE_STRING_H_