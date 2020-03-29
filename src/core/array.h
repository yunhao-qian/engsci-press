#ifndef CORE_ARRAY_H_
#define CORE_ARRAY_H_

#include "core/global.h"

typedef struct Array {
    void **data, **head;
    int size, capacity;
    Destructor destructor;
} Array;

Array *new_array(Destructor destructor);
void delete_array(void *array);

void array_append(Array *array, void *value);
void array_remove(Array *array, int index);

void array_reserve(Array *array, int capacity);

#endif // CORE_ARRAY_H_