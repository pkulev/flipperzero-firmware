#pragma once
#include <furi.h>
#include <gui/view.h>

typedef enum {
    GuiButtonTypeLeft,
    GuiButtonTypeCenter,
    GuiButtonTypeRight,
} GuiButtonType;

typedef void (*ButtonCallback)(GuiButtonType result, void* context);

typedef struct WidgetElement WidgetElement;
typedef struct Widget Widget;

struct WidgetElement {
    // generic draw and input callbacks
    void (*draw)(Canvas* canvas, WidgetElement* element);
    bool (*input)(InputEvent* event, WidgetElement* element);

    // free callback
    void (*free)(WidgetElement* element);

    // generic model holder
    void* model;

    // pointer to widget that hold our element
    Widget* parent;
};

/* Create string element */
WidgetElement* widget_element_string_create(
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical,
    Font font,
    const char* text);

/* Create button element */
WidgetElement* widget_element_button_create(
    GuiButtonType button_type,
    const char* text,
    ButtonCallback callback,
    void* context);

/* Create icon element */
WidgetElement* widget_element_icon_create(uint8_t x, uint8_t y, const Icon* icon);

/* Create frame element */
WidgetElement* widget_element_frame_create(
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius);