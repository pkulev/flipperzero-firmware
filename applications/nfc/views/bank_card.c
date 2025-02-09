#include "bank_card.h"
#include "../helpers/nfc_emv_parser.h"
#include <m-string.h>

struct BankCard {
    Widget* widget;
};

BankCard* bank_card_alloc() {
    BankCard* bank_card = furi_alloc(sizeof(BankCard));
    bank_card->widget = widget_alloc();
    return bank_card;
}

void bank_card_free(BankCard* bank_card) {
    furi_assert(bank_card);
    widget_free(bank_card->widget);
    free(bank_card);
}

View* bank_card_get_view(BankCard* bank_card) {
    furi_assert(bank_card);
    return widget_get_view(bank_card->widget);
}

void bank_card_clear(BankCard* bank_card) {
    furi_assert(bank_card);
    widget_clear(bank_card->widget);
}

void bank_card_set_name(BankCard* bank_card, char* name) {
    furi_assert(bank_card);
    furi_assert(name);
    widget_add_string_element(
        bank_card->widget, 64, 6, AlignCenter, AlignTop, FontSecondary, name);
}

void bank_card_set_number(BankCard* bank_card, uint8_t* number) {
    furi_assert(bank_card);
    furi_assert(number);
    string_t num_str;
    string_init(num_str);
    for(uint8_t i = 0; i < 8; i += 2) {
        string_cat_printf(num_str, "%02X%02X ", number[i], number[i + 1]);
    }
    // Add number
    widget_add_string_element(
        bank_card->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, string_get_cstr(num_str));
    string_clear(num_str);
    // Add icon
    widget_add_icon_element(bank_card->widget, 8, 15, &I_Detailed_chip_17x13);
    // Add frame
    widget_add_frame_element(bank_card->widget, 0, 0, 128, 64, 6);
}

void bank_card_set_back_callback(BankCard* bank_card, ButtonCallback callback, void* context) {
    furi_assert(bank_card);
    furi_assert(callback);
    widget_add_button_element(bank_card->widget, GuiButtonTypeLeft, "Back", callback, context);
}

void bank_card_set_exp_date(BankCard* bank_card, uint8_t mon, uint8_t year) {
    furi_assert(bank_card);
    char exp_date_str[16];
    snprintf(exp_date_str, sizeof(exp_date_str), "Exp: %02X/%02X", mon, year);
    widget_add_string_element(
        bank_card->widget, 122, 54, AlignRight, AlignBottom, FontSecondary, exp_date_str);
}

void bank_card_set_country_name(BankCard* bank_card, uint16_t country_code) {
    furi_assert(bank_card);
    string_t country_name;
    string_init(country_name);
    if(nfc_emv_parser_get_country_name(country_code, country_name)) {
        string_t disp_country;
        string_init_printf(disp_country, "Reg:%s", country_name);
        widget_add_string_element(
            bank_card->widget,
            120,
            18,
            AlignRight,
            AlignTop,
            FontSecondary,
            string_get_cstr(disp_country));
        string_clear(disp_country);
    }
    string_clear(country_name);
}

void bank_card_set_currency_name(BankCard* bank_card, uint16_t currency_code) {
    furi_assert(bank_card);
    string_t currency_name;
    string_init(currency_name);
    if(nfc_emv_parser_get_currency_name(currency_code, currency_name)) {
        string_t disp_currency;
        string_init_printf(disp_currency, "Cur:%s", currency_name);
        widget_add_string_element(
            bank_card->widget,
            31,
            18,
            AlignLeft,
            AlignTop,
            FontSecondary,
            string_get_cstr(disp_currency));
        string_clear(disp_currency);
    }
    string_clear(currency_name);
}
