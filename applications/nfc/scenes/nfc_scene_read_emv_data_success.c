#include "../nfc_i.h"
#include "../helpers/nfc_emv_parser.h"

void nfc_scene_read_emv_data_success_widget_callback(GuiButtonType result, void* context) {
    Nfc* nfc = (Nfc*)context;

    view_dispatcher_send_custom_event(nfc->view_dispatcher, result);
}

void nfc_scene_read_emv_data_success_on_enter(void* context) {
    Nfc* nfc = (Nfc*)context;
    NfcEmvData* emv_data = &nfc->dev.dev_data.emv_data;
    NfcDeviceCommonData* nfc_data = &nfc->dev.dev_data.nfc_data;

    // Clear device name
    nfc_device_set_name(&nfc->dev, "");

    // Setup Custom Widget view
    // Add frame
    widget_add_frame_element(nfc->widget, 0, 0, 128, 64, 6);
    // Add buttons
    widget_add_button_element(
        nfc->widget,
        GuiButtonTypeLeft,
        "Back",
        nfc_scene_read_emv_data_success_widget_callback,
        nfc);
    widget_add_button_element(
        nfc->widget,
        GuiButtonTypeRight,
        "Save",
        nfc_scene_read_emv_data_success_widget_callback,
        nfc);
    // Add card name
    widget_add_string_element(
        nfc->widget, 64, 3, AlignCenter, AlignTop, FontSecondary, nfc->dev.dev_data.emv_data.name);
    // Add cad number
    char pan_str[32];
    snprintf(
        pan_str,
        sizeof(pan_str),
        "%02X%02X %02X%02X %02X%02X %02X%02X",
        emv_data->number[0],
        emv_data->number[1],
        emv_data->number[2],
        emv_data->number[3],
        emv_data->number[4],
        emv_data->number[5],
        emv_data->number[6],
        emv_data->number[7]);
    widget_add_string_element(nfc->widget, 64, 13, AlignCenter, AlignTop, FontSecondary, pan_str);
    // Parse country code
    string_t country_name;
    string_init(country_name);
    if((emv_data->country_code) &&
       nfc_emv_parser_get_country_name(emv_data->country_code, country_name)) {
        string_t disp_country;
        string_init_printf(disp_country, "Reg:%s", country_name);
        widget_add_string_element(
            nfc->widget, 7, 23, AlignLeft, AlignTop, FontSecondary, string_get_cstr(disp_country));
        string_clear(disp_country);
    }
    string_clear(country_name);
    // Parse currency code
    string_t currency_name;
    string_init(currency_name);
    if((emv_data->currency_code) &&
       nfc_emv_parser_get_currency_name(emv_data->currency_code, currency_name)) {
        string_t disp_currency;
        string_init_printf(disp_currency, "Cur:%s", currency_name);
        widget_add_string_element(
            nfc->widget,
            121,
            23,
            AlignRight,
            AlignTop,
            FontSecondary,
            string_get_cstr(disp_currency));
        string_clear(disp_currency);
    }
    string_clear(currency_name);
    // Add ATQA
    char atqa_str[16];
    snprintf(atqa_str, sizeof(atqa_str), "ATQA: %02X%02X", nfc_data->atqa[0], nfc_data->atqa[1]);
    widget_add_string_element(nfc->widget, 121, 32, AlignRight, AlignTop, FontSecondary, atqa_str);
    // Add UID
    char uid_str[32];
    snprintf(
        uid_str,
        sizeof(uid_str),
        "UID: %02X %02X %02X %02X",
        nfc_data->uid[0],
        nfc_data->uid[1],
        nfc_data->uid[2],
        nfc_data->uid[3]);
    widget_add_string_element(nfc->widget, 7, 42, AlignLeft, AlignTop, FontSecondary, uid_str);
    // Add SAK
    char sak_str[16];
    snprintf(sak_str, sizeof(sak_str), "SAK: %02X", nfc_data->sak);
    widget_add_string_element(nfc->widget, 121, 42, AlignRight, AlignTop, FontSecondary, sak_str);
    // Add expiration date
    if(emv_data->exp_mon) {
        char exp_str[16];
        snprintf(
            exp_str, sizeof(exp_str), "Exp: %02X/%02X", emv_data->exp_mon, emv_data->exp_year);
        widget_add_string_element(nfc->widget, 7, 32, AlignLeft, AlignTop, FontSecondary, exp_str);
    }

    view_dispatcher_switch_to_view(nfc->view_dispatcher, NfcViewWidget);
}

const bool nfc_scene_read_emv_data_success_on_event(void* context, SceneManagerEvent event) {
    Nfc* nfc = (Nfc*)context;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == GuiButtonTypeLeft) {
            return scene_manager_search_and_switch_to_previous_scene(
                nfc->scene_manager, NfcSceneReadEmvAppSuccess);
        } else if(event.event == GuiButtonTypeRight) {
            nfc->dev.format = NfcDeviceSaveFormatBankCard;
            scene_manager_next_scene(nfc->scene_manager, NfcSceneSaveName);
            return true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        return scene_manager_search_and_switch_to_previous_scene(
            nfc->scene_manager, NfcSceneReadEmvAppSuccess);
    }
    return false;
}

const void nfc_scene_read_emv_data_success_on_exit(void* context) {
    Nfc* nfc = (Nfc*)context;

    widget_clear(nfc->widget);
}
