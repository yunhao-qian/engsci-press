#include "core/trie_node.h"

TrieNode *new_trie_node(char letter, TrieNode *parent) {
    assert((!parent || is_valid_key_char(letter, true)) &&
           "new_trie_node: invalid character");
    TrieNode *node = malloc(sizeof(TrieNode));
    node->letter = letter;
    node->entries = new_array(delete_dict_entry);
    node->parent = parent;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        node->children[i] = NULL;
    }
    node->child_count = 0;
    return node;
}

void delete_trie_node(void *p) {
    TrieNode *node = p;
    delete_array(node->entries);
    free(node);
}

static TrieNode *max_trie_leaf(TrieNode *node) {
    if (node->child_count <= 0) {
        return node;
    }
    for (int i = ALPHABET_SIZE - 1; i >= 0; --i) {
        if (node->children[i]) {
            return max_trie_leaf(node->children[i]);
        }
    }
    assert(false && "max_trie_leaf: unreachable code");
    return NULL;
}

TrieNode *previous_trie_node(const TrieNode *node) {
    TrieNode *parent = node->parent;
    if (!parent) {
        return NULL;
    }
    for (int i = char_to_index(node->letter) - 1; i >= 0; --i) {
        if (parent->children[i]) {
            return max_trie_leaf(parent->children[i]);
        }
    }
    return parent;
}

TrieNode *next_trie_node(const TrieNode *node) {
    if (node->child_count > 0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                return node->children[i];
            }
        }
    }
    TrieNode *parent = node->parent;
    int index;
    while (parent) {
        index = char_to_index(node->letter);
        for (int i = index + 1; i < ALPHABET_SIZE; ++i) {
            if (parent->children[i]) {
                return parent->children[i];
            }
        }
        node = parent;
        parent = node->parent;
    }
    return NULL;
}

TrieNode *trie_node_add_child(TrieNode *node, char letter) {
    int index = char_to_index(letter);
    assert(!node->children[index] &&
           "trie_node_add_child: child already exists");
    node->children[index] = new_trie_node(letter, node);
    ++node->child_count;
    return node->children[index];
}

void trie_node_remove_child(TrieNode *node, char letter) {
    int index = char_to_index(letter);
    assert(node->child_count > 0 && node->children[index] &&
           "trie_node_remove_child: child does not exist");
    assert(node->children[index]->child_count <= 0 &&
           "trie_node_remove_child: remove a non-leaf child");
    delete_trie_node(node->children[index]);
    node->children[index] = NULL;
    --node->child_count;
}