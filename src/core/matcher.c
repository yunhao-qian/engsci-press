#include "core/matcher.h"

Matcher *new_matcher(const String *pattern) {
    assert(pattern->size > 0 && "new_matcher: pattern is empty");
    Matcher *matcher = malloc(sizeof(Matcher));
    matcher->pattern = to_lower(pattern);
    matcher->distances = new_array(free);
    int *row = malloc((pattern->size + 1) * sizeof(int));
    for (int i = 0; i <= pattern->size; ++i) {
        row[i] = i;
    }
    array_append(matcher->distances, row);
    matcher->min_distance = 0;
    return matcher;
}

void delete_matcher(void *p) {
    Matcher *matcher = p;
    delete_string(matcher->pattern);
    delete_array(matcher->distances);
    free(matcher);
}

static inline int int_min(int a, int b) {
    return a < b ? a : b;
}

void matcher_push(Matcher *matcher, char letter) {
    int *previous = matcher->distances->data[matcher->distances->size - 1];
    int *current = malloc((matcher->pattern->size + 1) * sizeof(int));
    matcher->min_distance = current[0] = previous[0] + 1;
    int d1, d2, d3;
    for (int i = 1; i <= matcher->pattern->size; ++i) {
        d1 = current[i - 1] + 1;
        d2 = previous[i] + 1;
        d3 = previous[i - 1] + (letter != matcher->pattern->text[i - 1]);
        current[i] = int_min(d1, int_min(d2, d3));
        if (current[i] < matcher->min_distance) {
            matcher->min_distance = current[i];
        }
    }
    array_append(matcher->distances, current);
}

void reset_matcher(Matcher *matcher) {
    assert(matcher->distances->size > 0 &&
           "reset_matcher: distance array is empty");
    for (int i = matcher->distances->size - 1; i > 0; --i) {
        array_remove(matcher->distances, i);
    }
}

static String *matcher_match_node(Matcher *matcher, const TrieNode *node,
                                  int distance) {
    if (matcher->min_distance > distance) {
        return NULL;
    }
    int *last_row = matcher->distances->data[matcher->distances->size - 1];
    if (last_row[matcher->pattern->size] == distance &&
        node->entries->size > 0) {
        DictEntry *entry = node->entries->data[0];
        return to_lower(entry->headword);
    }
    if (node->child_count > 0) {
        TrieNode *child;
        String *result;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            child = node->children[i];
            if (!child) {
                continue;
            }
            matcher_push(matcher, child->letter);
            result = matcher_match_node(matcher, child, distance);
            array_remove(matcher->distances, matcher->distances->size - 1);
            if (result) {
                return result;
            }
        }
    }
    return NULL;
}

String *trie_closest_match(const Trie *trie, const String *pattern,
                           int tolerance) {
    if (tolerance < 0) {
        tolerance = pattern->size;
    }
    Matcher *matcher = new_matcher(pattern);
    String *result;
    for (int i = 0; i <= tolerance; ++i) {
        result = matcher_match_node(matcher, trie->root, i);
        reset_matcher(matcher);
        if (result) {
            break;
        }
    }
    delete_matcher(matcher);
    return result;
}