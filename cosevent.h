#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>

typedef struct EventSchedule EventSchedule;
typedef struct ScheduleEvent ScheduleEvent;

struct ScheduleEvent {
    void (*entry)(ScheduleEvent* event);
    void (*life)(ScheduleEvent* event, float deltaTime, uint32_t frame_buffer);
    bool (*param)(ScheduleEvent* event);
};

#endif