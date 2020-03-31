#ifndef CORE_GLOBAL_H_
#define CORE_GLOBAL_H_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 3000
#define ALPHABET_SIZE 59

#ifdef SUPPRESS_WARNINGS
#define WARN(...)
#else
#define WARN(...) fprintf(stderr, __VA_ARGS__)
#endif // SUPPRESS_WARNINGS

typedef void (*Destructor)(void *);

bool is_valid_key_char(char c, bool case_sensitive);
int char_to_index(char c);

#endif // CORE_GLOBAL_H_