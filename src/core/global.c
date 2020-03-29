#include "core/global.h"

bool is_valid_key_char(char c, bool case_sensitive) {
    if (!case_sensitive && c >= 65 && c <= 90) {
        return true;
    }
    return c >= 32 && c <= 64 || c >= 97 && c <= 122;
}

int char_to_index(char c) {
    assert(is_valid_key_char(c, true) && "char_to_index: invalid character");
    if (c < 96) {
        return c - 32;
    } else {
        return c - 64;
    }
}