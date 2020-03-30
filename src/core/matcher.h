#ifndef CORE_MATCHER_H_
#define CORE_MATCHER_H_

#include "core/trie.h"

typedef struct Matcher {
    String *pattern;
    Array *distances;
    int min_distance;
} Matcher;

Matcher *new_matcher(const String *pattern);
void delete_matcher(void *matcher);

void matcher_push(Matcher *matcher, char letter);
void reset_matcher(Matcher *matcher);

String *trie_closest_match(const Trie *trie, const String *pattern,
                           int tolerance);

#endif // CORE_MATCHER_H_