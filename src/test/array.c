#include "test/global.h"

int test_count = 0, pass_count = 0;

#define N 50

int data[N];

void generate_data() {
    for (int i = 0; i < N; ++i) {
        data[i] = rand();
    }
}

void test_append() {
    generate_data();
    Array *array = new_array(free);
    int *element, *random;
    for (int i = 0; i < N; ++i) {
        element = malloc(sizeof(int));
        *element = data[i];
        random = malloc(sizeof(int));
        *random = rand();
        array_append(array, random);
        CHECK_INT(i + 1, array->size);
        array_append(array, element);
        CHECK_INT(i + 2, array->size);
        array_remove(array, i);
        CHECK_INT(i + 1, array->size);
    }
    for (int i = 0; i < N; ++i) {
        CHECK_INT(data[i], *(int *)(array->data[i]));
    }
    delete_array(array);
}

void test_remove() {
    generate_data();
    Array *array = new_array(free);
    int *element;
    for (int i = 0; i < N; ++i) {
        element = malloc(sizeof(int));
        *element = data[i];
        array_append(array, element);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            CHECK_INT(data[j], *(int *)(array->data[j - i]));
        }
        array_remove(array, 0);
    }
    CHECK_INT(0, array->size);
    delete_array(array);
}

void test_reserve() {
    generate_data();
    Array *array = new_array(free);
    array_reserve(array, N - 1);
    int *element;
    for (int i = 0; i < N; ++i) {
        element = malloc(sizeof(int));
        *element = data[i];
        CHECK_INT(N - 1, array->capacity);
        array_append(array, element);
    }
    CHECK_INT((N - 1) * 2, array->capacity);
    delete_array(array);
}

int main() {
    test_append();
    test_remove();
    test_reserve();
    DISPLAY_TEST_RESULT();
}