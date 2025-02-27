#pragma once

#include <furi-hal-resources.h>

/* Input Types
 * Some of them are physical events and some logical
 */
typedef enum {
    InputTypePress, /* Press event, emitted after debounce */
    InputTypeRelease, /* Release event, emitted after debounce */
    InputTypeShort, /* Short event, emitted after InputTypeRelease done withing INPUT_LONG_PRESS interval */
    InputTypeLong, /* Long event, emmited after INPUT_LONG_PRESS interval, asynchronouse to InputTypeRelease  */
    InputTypeRepeat, /* Repeat event, emmited with INPUT_REPEATE_PRESS period after InputTypeLong event */
} InputType;

/* Input Event, dispatches with PubSub */
typedef struct {
    uint32_t sequence;
    InputKey key;
    InputType type;
} InputEvent;

/** Get human readable input key name
 * @param key - InputKey
 * @return string
 */
const char* input_get_key_name(InputKey key);

/** Get human readable input type name
 * @param type - InputType
 * @return string
 */
const char* input_get_type_name(InputType type);
