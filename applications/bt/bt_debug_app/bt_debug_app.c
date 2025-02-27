#include "bt_debug_app.h"

enum BtDebugSubmenuIndex {
    BtDebugSubmenuIndexCarrierTest,
    BtDebugSubmenuIndexPacketTest,
};

void bt_debug_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    BtDebugApp* app = context;
    if(index == BtDebugSubmenuIndexCarrierTest) {
        view_dispatcher_switch_to_view(app->view_dispatcher, BtDebugAppViewCarrierTest);
    } else if(index == BtDebugSubmenuIndexPacketTest) {
        view_dispatcher_switch_to_view(app->view_dispatcher, BtDebugAppViewPacketTest);
    }
}

uint32_t bt_debug_exit(void* context) {
    return VIEW_NONE;
}

uint32_t bt_debug_start_view(void* context) {
    return BtDebugAppViewSubmenu;
}

BtDebugApp* bt_debug_app_alloc() {
    BtDebugApp* app = furi_alloc(sizeof(BtDebugApp));
    // Gui
    app->gui = furi_record_open("gui");

    // View dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Views
    app->submenu = submenu_alloc();
    submenu_add_item(
        app->submenu,
        "Carrier test",
        BtDebugSubmenuIndexCarrierTest,
        bt_debug_submenu_callback,
        app);
    submenu_add_item(
        app->submenu, "Packet test", BtDebugSubmenuIndexPacketTest, bt_debug_submenu_callback, app);
    view_set_previous_callback(submenu_get_view(app->submenu), bt_debug_exit);
    view_dispatcher_add_view(
        app->view_dispatcher, BtDebugAppViewSubmenu, submenu_get_view(app->submenu));
    app->bt_carrier_test = bt_carrier_test_alloc();
    view_set_previous_callback(
        bt_carrier_test_get_view(app->bt_carrier_test), bt_debug_start_view);
    view_dispatcher_add_view(
        app->view_dispatcher,
        BtDebugAppViewCarrierTest,
        bt_carrier_test_get_view(app->bt_carrier_test));
    app->bt_packet_test = bt_packet_test_alloc();
    view_set_previous_callback(bt_packet_test_get_view(app->bt_packet_test), bt_debug_start_view);
    view_dispatcher_add_view(
        app->view_dispatcher,
        BtDebugAppViewPacketTest,
        bt_packet_test_get_view(app->bt_packet_test));

    // Switch to menu
    view_dispatcher_switch_to_view(app->view_dispatcher, BtDebugAppViewSubmenu);

    return app;
}

void bt_debug_app_free(BtDebugApp* app) {
    furi_assert(app);

    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, BtDebugAppViewSubmenu);
    submenu_free(app->submenu);
    view_dispatcher_remove_view(app->view_dispatcher, BtDebugAppViewCarrierTest);
    bt_carrier_test_free(app->bt_carrier_test);
    view_dispatcher_remove_view(app->view_dispatcher, BtDebugAppViewPacketTest);
    bt_packet_test_free(app->bt_packet_test);
    view_dispatcher_free(app->view_dispatcher);

    // Close gui record
    furi_record_close("gui");
    app->gui = NULL;

    // Free rest
    free(app);
}

int32_t bt_debug_app(void* p) {
    BtDebugApp* app = bt_debug_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    bt_debug_app_free(app);
    return 0;
}
