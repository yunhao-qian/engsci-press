#ifndef CORE_DICT_ENTRY_H_
#define CORE_DICT_ENTRY_H_

#include "core/string.h"

typedef struct DictEntry {
    String *headword, *word_class, *definition;
} DictEntry;

DictEntry *new_dict_entry(const String *line);
void delete_dict_entry(void *entry);

bool confirm(bool default_yes, const char *message);
DictEntry *input_dict_entry(const String *headword);

void display_dict_entry(const DictEntry *entry);
void write_dict_entry(const DictEntry *entry, FILE *stream);

#endif // CORE_DICT_ENTRY_H_