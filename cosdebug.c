#include "cosdebug.h"
#include <libdragon.h>

rdpq_textparms_t debug_text_parms = (rdpq_textparms_t) {
    .wrap = WRAP_WORD,
    .width = 300,
    .height = 210,
    .valign = VALIGN_BOTTOM
};

int debug_log_count = 0;
char debug_log_text[DEBUG_LOG_TEXT_SIZE];
DebugLogEntry* first_entry;
DebugLogEntry* last_entry;

void debug_log_init() {
    first_entry = malloc(sizeof(DebugLogEntry));
    last_entry = first_entry;
    first_entry->next_log = first_entry;
    strcpy(first_entry->text, "Log Active!");
    debug_log_count = 1;
}
void debug_log_write(const char* text) {
    if (debug_log_count < DEBUG_LOG_ROWS) {
        DebugLogEntry* new_entry = malloc(sizeof(DebugLogEntry));
        strcpy(new_entry->text, text);
        last_entry->next_log = new_entry;
        new_entry->next_log = first_entry;
        last_entry = new_entry;
        debug_log_count += 1;
    } else {
        DebugLogEntry* new_entry = first_entry;
        first_entry = first_entry->next_log;
        last_entry = new_entry;
        strcpy(new_entry->text, text);
    }
    strcpy(debug_log_text, "DEBUG: ");
    strcat(debug_log_text, first_entry->text);
    DebugLogEntry* current_entry = first_entry->next_log;

    while(current_entry != first_entry) {
        strcat(debug_log_text, "\n");
        strcat(debug_log_text, "DEBUG: ");
        strcat(debug_log_text, current_entry->text);
        current_entry = current_entry->next_log;
    }
}
void debug_log_draw() {
    rdpq_text_print(&debug_text_parms, FONT_BUILTIN_DEBUG_MONO, 10, 10, debug_log_text);
}

int debug_log_get_count() {
    return debug_log_count;
}