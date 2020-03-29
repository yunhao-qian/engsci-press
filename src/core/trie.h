#ifndef CORE_TRIE_H_
#define CORE_TRIE_H_

#include "core/trie_node.h"

typedef struct Trie {
    TrieNode *root;
    int size;
} Trie;

Trie *new_trie();
void delete_trie(void *trie);

Array *trie_search(const Trie *trie, const String *word, bool case_sensitive);
void trie_insert(Trie *trie, DictEntry *entry);
void trie_remove(Trie *trie, const String *word, bool case_sensitive);

String *trie_predecessor(const Trie *trie, const String *word);
String *trie_successor(const Trie *trie, const String *word);

Array *traverse_trie(const Trie *trie);

#endif // CORE_TRIE_H_