#ifndef CORE_TRIE_NODE_H_
#define CORE_TRIE_NODE_H_

#include "core/dict_entry.h"

typedef struct TrieNode {
    char letter;
    Array *entries;
    struct TrieNode *parent, *children[ALPHABET_SIZE];
    int child_count;
} TrieNode;

TrieNode *new_trie_node(char letter, TrieNode *parent);
void delete_trie_node(void *node);

TrieNode *previous_trie_node(const TrieNode *node);
TrieNode *next_trie_node(const TrieNode *node);

TrieNode *trie_node_add_child(TrieNode *node, char letter);
void trie_node_remove_child(TrieNode *node, char letter);

#endif // CORE_TRIE_NODE_H_