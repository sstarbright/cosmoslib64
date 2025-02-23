#ifndef __COSDEBUG_H
#define __COSDEBUG_H

#ifndef DEBUG_LOG_ROWS
#define DEBUG_LOG_ROWS 20
#endif

#ifndef DEBUG_LOG_COLUMNS
#define DEBUG_LOG_COLUMNS 75
#endif

#define DEBUG_LOG_TEXT_SIZE (20*75)

#include <string.h>
#include <libdragon.h>

typedef struct DebugLogEntry DebugLogEntry;

struct DebugLogEntry {
    char text[DEBUG_LOG_COLUMNS];
    DebugLogEntry* next_log;
};

void debug_log_init();
void debug_log_write(const char* text);
void debug_log_draw();
int debug_log_get_count();

#endif