#ifndef MAIN_UTILITY_H_
#define MAIN_UTILITY_H_

#include "main/api.h"

#define WARN_NOT_SUPPORTED(argument, mode)                                     \
    WARN("Does not support \"%s\" in %s mode.\n", argument, mode);

#define WARN_MISSING(expected)                                                 \
    WARN("Missing argument: %s expected.\n", expected)

#define WARN_REDUNDANT(arguments, start_index)                                 \
    WARN("Redundant arguments: ignore arguments since \"%s\".\n",              \
         ((String *)(arguments)->data[start_index])->text)

int parse_unsigned_int_flag(const String *string);

void word_hint(const String *string, const Trie *dictionary,
               bool case_sensitive);

#endif // MAIN_UTILITY_H_