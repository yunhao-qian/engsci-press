#ifndef MAIN_API_H_
#define MAIN_API_H_

#include "core/array.h"
#include "core/dict_entry.h"
#include "core/matcher.h"
#include "core/string.h"
#include "core/trie.h"
#include "core/trie_node.h"

#ifndef ESP_RC_PATH
#define ESP_RC_PATH "../.esp_rc"
#endif // ESP_RC_PATH

typedef enum EspMode {
    ESP_MODE_INTERACTIVE,
    ESP_MODE_BACKGROUND,
    ESP_MODE_COMMAND_LINE
} EspMode;

void esp_initialize(EspMode mode);
void esp_cleanup(EspMode mode);

bool esp_parse_arguments(Array *arguments, EspMode mode);

void esp_on_load(Array *arguments, EspMode mode);
void esp_on_search(Array *arguments, EspMode mode);
void esp_on_insert(Array *arguments, EspMode mode);
void esp_on_remove(Array *arguments, EspMode mode);
void esp_on_neighbour(Array *arguments, EspMode mode);
void esp_on_prefix(Array *arguments, EspMode mode);
void esp_on_match(Array *arguments, EspMode mode);
void esp_on_size(Array *arguments, EspMode mode);
void esp_on_save(Array *arguments, EspMode mode);
bool esp_on_exit(Array *arguments, EspMode mode);

#endif // MAIN_API_H_