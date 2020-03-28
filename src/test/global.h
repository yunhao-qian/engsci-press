#ifndef TEST_GLOBAL_H_
#define TEST_GLOBAL_H_

#include "core/array.h"
#include "core/string.h"

#define CHECK_BASE(equality, expect, actual, format)                           \
    do {                                                                       \
        ++test_count;                                                          \
        if (equality) {                                                        \
            ++pass_count;                                                      \
        } else {                                                               \
            fprintf(stderr,                                                    \
                    "%s: %d\nexpect: " format "\nactual: " format "\n",        \
                    __FILE__, __LINE__, expect, actual);                       \
        }                                                                      \
    } while (false)

#define DISPLAY_TEST_RESULT()                                                  \
    printf("%d/%d (%g%%) passed\n", pass_count, test_count,                    \
           pass_count * 100.0 / test_count)

#define CHECK_INT(expect, actual)                                              \
    CHECK_BASE((expect) == (actual), expect, actual, "%d")

#define CHECK_STRING(expect, actual)                                           \
    CHECK_BASE(!strcmp(expect, (actual)->text), expect, (actual)->text, "%s")

#endif // TEST_GLOBAL_H_