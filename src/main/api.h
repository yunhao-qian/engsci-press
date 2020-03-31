#ifndef MAIN_API_H_
#define MAIN_API_H_

#include "core/array.h"
#include "core/dict_entry.h"
#include "core/matcher.h"
#include "core/string.h"
#include "core/trie.h"
#include "core/trie_node.h"

typedef enum EspMode {
    ESP_MODE_INTERACTIVE,
    ESP_MODE_BACKGROUND,
    ESP_MODE_COMMAND_LINE
} EspMode;

void esp_initialize(EspMode mode);
void esp_cleanup(EspMode mode);

bool esp_parse_arguments(Array *arguments, EspMode mode);

void esp_on_load(const Array *arguments, EspMode mode);
void esp_on_search(const Array *arguments, EspMode mode);
void esp_on_insert(const Array *arguments, EspMode mode);
void esp_on_remove(const Array *arguments, EspMode mode);
void esp_on_neighbour(const Array *arguments, EspMode mode);
void esp_on_prefix(const Array *arguments, EspMode mode);
void esp_on_match(const Array *arguments, EspMode mode);
void esp_on_size(const Array *arguments, EspMode mode);
void esp_on_save(const Array *arguments, EspMode mode);
bool esp_on_exit(const Array *arguments, EspMode mode);

#endif // MAIN_API_H_