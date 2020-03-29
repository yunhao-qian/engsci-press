#include "core/string.h"

String *new_string(const char *text, int size) {
    if (size < 0) {
        size = strlen(text);
    } else {
        assert(size <= strlen(text) &&
               "new_string: size larger than text length");
    }
    String *string = malloc(sizeof(String));
    string->text = string->head = malloc((size + 1) * sizeof(char));
    memcpy(string->text, text, size * sizeof(char));
    string->text[size] = '\0';
    string->size = string->capacity = size;
    return string;
}

void delete_string(void *p) {
    String *string = p;
    free(string->head);
    free(string);
}

bool is_valid_key(const String *string, bool case_sensitive) {
    if (string->size <= 0) {
        return false;
    }
    for (int i = 0; i < string->size; ++i) {
        if (!is_valid_key_char(string->text[i], case_sensitive)) {
            return false;
        }
    }
    return true;
}

int string_index(const String *string, char value) {
    char *find = strchr(string->text, value);
    if (!find) {
        return -1;
    } else {
        return find - string->text;
    }
}

String *to_lower(const String *string) {
    String *lower = malloc(sizeof(String));
    lower->text = lower->head = malloc((string->size + 1) * sizeof(char));
    for (int i = 0; i <= string->size; ++i) {
        lower->text[i] = tolower(string->text[i]);
    }
    lower->size = lower->capacity = string->size;
    return lower;
}

void to_lower_in_place(String *string) {
    for (int i = 0; i < string->size; ++i) {
        string->text[i] = tolower(string->text[i]);
    }
}

static void shrink_string(String *string) {
    if (string->size > string->capacity / 2) {
        return;
    }
    size_t block_size = (string->size + 1) * sizeof(char);
    char *head;
    if (string->text == string->head) {
        head = realloc(string->head, block_size);
    } else {
        head = malloc(block_size);
        memcpy(head, string->text, block_size);
        free(string->head);
    }
    string->text = string->head = head;
    string->capacity = string->size;
}

String *substring(const String *string, int start, int end) {
    if (start >= end) {
        return new_string("", 0);
    }
    assert(start >= 0 && start < string->size &&
           "substring: start index out of range");
    assert(end >= 0 && end <= string->size &&
           "substring: end index out of range");
    return new_string(string->text + start, end - start);
}

void substring_in_place(String *string, int start, int end) {
    if (start == 0 && end == string->size) {
        return;
    }
    if (start >= end) {
        string->text = string->head;
        string->text[0] = '\0';
        string->size = 0;
        shrink_string(string);
        return;
    }
    assert(start >= 0 && start < string->size &&
           "substring_in_place: start index out of range");
    assert(end >= 0 && end <= string->size &&
           "substring_in_place: end index out of range");
    string->text += start;
    string->size = end - start;
    string->text[string->size] = '\0';
    shrink_string(string);
}

static void get_trim_indices(const String *string, int *start, int *end) {
    *end = 0;
    for (int i = string->size; i > 0; --i) {
        if (!isspace(string->text[i - 1])) {
            *end = i;
            break;
        }
    }
    *start = *end;
    for (int i = 0; i < *end; ++i) {
        if (!isspace(string->text[i])) {
            *start = i;
            break;
        }
    }
}

String *trim(const String *string) {
    int start, end;
    get_trim_indices(string, &start, &end);
    return substring(string, start, end);
}

void trim_in_place(String *string) {
    int start, end;
    get_trim_indices(string, &start, &end);
    substring_in_place(string, start, end);
}

String *get_line(FILE *stream) {
    static char buffer[MAX_LINE_SIZE + 1];
    if (!fgets(buffer, MAX_LINE_SIZE + 1, stream)) {
        return NULL;
    }
    int size = strlen(buffer);
    if (size > 0 && buffer[size - 1] == '\n') {
        --size;
    } else {
        int c;
        do {
            c = getc(stream);
        } while (c != '\n' && c != EOF);
    }
    if (size > 0 && buffer[size - 1] == '\r') {
        --size;
    }
    return new_string(buffer, size);
}

Array *split_string(const String *string) {
    Array *array = new_array(delete_string);
    int start;
    bool in_field = false;
    for (int i = 0; i < string->size; ++i) {
        if (isspace(string->text[i])) {
            if (in_field) {
                array_append(array, substring(string, start, i));
                in_field = false;
            }
        } else if (!in_field) {
            start = i;
            in_field = true;
        }
    }
    if (in_field) {
        array_append(array, substring(string, start, string->size));
    }
    return array;
}