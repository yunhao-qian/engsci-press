#include "core/array.h"

Array *new_array(Destructor destructor) {
    Array *array = malloc(sizeof(Array));
    array->size = array->capacity = 0;
    array->destructor = destructor;
    return array;
}

void delete_array(void *p) {
    Array *array = p;
    if (array->size > 0) {
        if (array->destructor) {
            for (int i = 0; i < array->size; ++i) {
                array->destructor(array->data[i]);
            }
        }
        free(array->head);
    }
    free(array);
}

static void expand_array(Array *array) {
    if (array->capacity <= 0) {
        array->data = array->head = malloc(sizeof(void *));
        array->capacity = 1;
        return;
    }
    size_t block_size = array->size * sizeof(void *);
    if (array->size < array->capacity) {
        if (array->data + array->size >= array->head + array->capacity) {
            memmove(array->head, array->data, block_size);
        }
    } else {
        array->data = array->head = realloc(array->head, 2 * block_size);
        array->capacity *= 2;
    }
}

void array_append(Array *array, void *value) {
    expand_array(array);
    array->data[array->size] = value;
    ++array->size;
}

static void shrink_array(Array *array) {
    if (array->size <= 0) {
        free(array->head);
        array->capacity = 0;
        return;
    }
    if (array->size > array->capacity / 2) {
        return;
    }
    size_t block_size = array->size * sizeof(void *);
    void **head;
    if (array->data == array->head) {
        head = realloc(array->head, block_size);
    } else {
        head = malloc(block_size);
        memcpy(head, array->data, block_size);
        free(array->head);
    }
    array->data = array->head = head;
    array->capacity = array->size;
}

void array_remove(Array *array, int index) {
    assert(index >= 0 && index < array->size &&
           "array_remove: index out of range");
    if (array->destructor) {
        array->destructor(array->data[index]);
    }
    size_t move_size;
    if (index < array->size / 2) {
        move_size = index * sizeof(void *);
        memmove(array->data + 1, array->data, move_size);
        ++array->data;
    } else {
        move_size = (array->size - index - 1) * sizeof(void *);
        memmove(array->data + index, array->data + (index + 1), move_size);
    }
    --array->size;
    shrink_array(array);
}

void array_reserve(Array *array, int capacity) {
    assert(capacity >= array->size &&
           "array_reserve: capacity smaller than array size");
    if (capacity == array->capacity) {
        return;
    }
    size_t reserve_size = capacity * sizeof(void *);
    void **head;
    if (array->size <= 0) {
        if (array->capacity <= 0) {
            head = malloc(reserve_size);
        } else {
            head = realloc(array->head, reserve_size);
        }
    } else if (array->data == array->head) {
        head = realloc(array->head, reserve_size);
    } else {
        head = malloc(reserve_size);
        memcpy(head, array->data, array->size * sizeof(void *));
        free(array->head);
    }
    array->data = array->head = head;
    array->capacity = capacity;
}