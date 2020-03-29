#include "core/trie.h"

Trie *new_trie() {
    Trie *trie = malloc(sizeof(Trie));
    trie->root = new_trie_node('\0', NULL);
    trie->size = 0;
    return trie;
}

static void clear_trie_nodes(TrieNode *root) {
    assert(root && "clear_trie_nodes: root node is null");
    if (root->child_count > 0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (root->children[i]) {
                clear_trie_nodes(root->children[i]);
            }
        }
    }
    delete_trie_node(root);
}

void delete_trie(void *p) {
    Trie *trie = p;
    clear_trie_nodes(trie->root);
    free(trie);
}

static TrieNode *get_trie_node(const Trie *trie, const String *word) {
    TrieNode *node = trie->root;
    for (int i = 0; i < word->size; ++i) {
        int index = char_to_index(tolower(word->text[i]));
        node = node->children[index];
        if (!node) {
            return NULL;
        }
    }
    return node;
}

Array *trie_search(const Trie *trie, const String *word, bool case_sensitive) {
    Array *entries = new_array(NULL);
    if (!is_valid_key(word, false)) {
        WARNING("Invalid headword: %s\n", word->text);
        return entries;
    }
    TrieNode *node = get_trie_node(trie, word);
    if (!node || node->entries->size <= 0) {
        return entries;
    }
    if (case_sensitive) {
        DictEntry *entry;
        for (int i = 0; i < node->entries->size; ++i) {
            entry = node->entries->data[i];
            if (!strcmp(entry->headword->text, word->text)) {
                array_append(entries, entry);
            }
        }
    } else {
        for (int i = 0; i < node->entries->size; ++i) {
            array_append(entries, node->entries->data[i]);
        }
    }
    return entries;
}

void trie_insert(Trie *trie, DictEntry *entry) {
    assert(is_valid_key(entry->headword, false) &&
           "trie_insert: invalid entry headword");
    String *lowered = to_lower(entry->headword);
    TrieNode *node = trie->root;
    for (int i = 0; i < lowered->size; ++i) {
        int index = char_to_index(lowered->text[i]);
        if (!node->children[index]) {
            node = trie_node_add_child(node, lowered->text[i]);
        } else {
            node = node->children[index];
        }
    }
    array_append(node->entries, entry);
    ++trie->size;
}

void trie_remove(Trie *trie, const String *word, bool case_sensitive) {
    assert(is_valid_key(word, false) && "trie_remove: invalid headword");
    TrieNode *node = get_trie_node(trie, word);
    assert(node && "trie_remove: trie node does not exist");
    if (case_sensitive) {
        int i = 0;
        DictEntry *entry;
        while (i < node->entries->size) {
            entry = node->entries->data[i];
            if (!strcmp(entry->headword->text, word->text)) {
                array_remove(node->entries, i);
                --trie->size;
            } else {
                ++i;
            }
        }
    } else {
        for (int i = node->entries->size - 1; i >= 0; --i) {
            array_remove(node->entries, i);
            --trie->size;
        }
    }
    char letter;
    while (node->entries->size <= 0 && node->child_count <= 0 && node->parent) {
        letter = node->letter;
        node = node->parent;
        trie_node_remove_child(node, letter);
    }
}

String *trie_predecessor(const Trie *trie, const String *word) {
    TrieNode *node = get_trie_node(trie, word);
    if (!node) {
        WARNING("Cannot find word: %s\n", word->text);
        return NULL;
    }
    node = previous_trie_node(node);
    while (node) {
        if (node->entries->size > 0) {
            return to_lower((String *)(node->entries->data[0]));
        }
        node = previous_trie_node(node);
    }
    return NULL;
}

String *trie_successor(const Trie *trie, const String *word) {
    TrieNode *node = get_trie_node(trie, word);
    if (!node) {
        WARNING("Cannot find word: %s\n", word->text);
        return NULL;
    }
    node = next_trie_node(node);
    while (node) {
        if (node->entries->size > 0) {
            return to_lower((String *)(node->entries->data[0]));
        }
        node = next_trie_node(node);
    }
    return NULL;
}

static void transverse_trie_nodes(const TrieNode *root, Array *entries) {
    for (int i = 0; i < root->entries->size; ++i) {
        array_append(entries, root->entries->data[i]);
    }
    if (root->child_count > 0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (root->children[i]) {
                transverse_trie_nodes(root->children[i], entries);
            }
        }
    }
}

Array *transverse_trie(const Trie *trie) {
    Array *entries = new_array(NULL);
    transverse_trie_nodes(trie->root, entries);
    return entries;
}