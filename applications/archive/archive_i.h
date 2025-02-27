#pragma once

#include "archive.h"
#include <stdint.h>
#include <furi.h>
#include <gui/gui_i.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/text_input.h>
#include <loader/loader.h>

#include <m-string.h>
#include <m-array.h>
#include <storage/storage.h>
#include "archive_views.h"
#include "applications.h"
#include "file-worker.h"

#define MAX_DEPTH 32
#define MAX_FILES 100 //temp
#define MAX_FILE_SIZE 128
#define ARCHIVE_FAV_PATH "/any/favorites.txt"
#define ARCHIVE_FAV_TEMP_PATH "/any/favorites.tmp"

typedef enum {
    ArchiveViewMain,
    ArchiveViewTextInput,
    ArchiveViewTotal,
} ArchiveViewEnum;

static const char* flipper_app_name[] = {
    [ArchiveFileTypeIButton] = "iButton",
    [ArchiveFileTypeNFC] = "NFC",
    [ArchiveFileTypeSubGhz] = "Sub-GHz",
    [ArchiveFileTypeLFRFID] = "125 kHz RFID",
    [ArchiveFileTypeIrda] = "Infrared",
};

static const char* known_ext[] = {
    [ArchiveFileTypeIButton] = ".ibtn",
    [ArchiveFileTypeNFC] = ".nfc",
    [ArchiveFileTypeSubGhz] = ".sub",
    [ArchiveFileTypeLFRFID] = ".rfid",
    [ArchiveFileTypeIrda] = ".ir",
};

static const char* tab_default_paths[] = {
    [ArchiveTabFavorites] = "/any/favorites",
    [ArchiveTabIButton] = "/any/ibutton",
    [ArchiveTabNFC] = "/any/nfc",
    [ArchiveTabSubGhz] = "/any/subghz/saved",
    [ArchiveTabLFRFID] = "/any/lfrfid",
    [ArchiveTabIrda] = "/any/irda",
    [ArchiveTabBrowser] = "/any",
};

static inline const char* get_tab_ext(ArchiveTabEnum tab) {
    switch(tab) {
    case ArchiveTabIButton:
        return known_ext[ArchiveFileTypeIButton];
    case ArchiveTabNFC:
        return known_ext[ArchiveFileTypeNFC];
    case ArchiveTabSubGhz:
        return known_ext[ArchiveFileTypeSubGhz];
    case ArchiveTabLFRFID:
        return known_ext[ArchiveFileTypeLFRFID];
    case ArchiveTabIrda:
        return known_ext[ArchiveFileTypeIrda];
    default:
        return "*";
    }
}

static inline const char* get_default_path(ArchiveFileTypeEnum type) {
    switch(type) {
    case ArchiveFileTypeIButton:
        return tab_default_paths[ArchiveTabIButton];
    case ArchiveFileTypeNFC:
        return tab_default_paths[ArchiveTabNFC];
    case ArchiveFileTypeSubGhz:
        return tab_default_paths[ArchiveTabSubGhz];
    case ArchiveFileTypeLFRFID:
        return tab_default_paths[ArchiveTabLFRFID];
    case ArchiveFileTypeIrda:
        return tab_default_paths[ArchiveTabIrda];
    default:
        return false;
    }
}

static inline const char* get_favorites_path() {
    return tab_default_paths[ArchiveTabFavorites];
}

typedef enum {
    EventTypeTick,
    EventTypeKey,
    EventTypeExit,
} EventType;

typedef struct {
    union {
        InputEvent input;
    } value;
    EventType type;
} AppEvent;

typedef enum {
    FavoritesCheck,
    FavoritesRead,
    FavoritesDelete,
    FavoritesRename,
} FavActionsEnum;

typedef struct {
    ArchiveTabEnum tab_id;
    string_t name;
    string_t path;
    char text_input_buffer[MAX_NAME_LEN];

    uint8_t depth;
    uint16_t last_idx[MAX_DEPTH];

    bool menu;
} ArchiveBrowser;

struct ArchiveApp {
    osMessageQueueId_t event_queue;
    FuriThread* app_thread;
    Loader* loader;
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    View* view_archive_main;
    TextInput* text_input;

    Storage* api;
    FileWorker* file_worker;
    ArchiveBrowser browser;
};
