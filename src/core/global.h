#ifndef CORE_GLOBAL_H_
#define CORE_GLOBAL_H_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 3000

typedef void (*Destructor)(void *);

#endif // CORE_GLOBAL_H_