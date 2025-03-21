#ifndef __COSSTATE_H
#define __COSSTATE_H

#include <libdragon.h>

// A structure that schedules a series of events that may be played in chronological order.
// INCOMPLETE - PLEASE FINISH
typedef struct EventSchedule EventSchedule;
// A structure that holds event data to be played by an EventSchedule.
// INCOMPLETE - PLEASE FINISH
typedef struct ScheduleEvent ScheduleEvent;

struct ScheduleEvent {
    // The function called when this event is entered.
    void (*entry)(ScheduleEvent* event);
    // The function called while this event is still active.
    void (*life)(ScheduleEvent* event, float deltaTime, uint32_t frame_buffer);
    // The function called to determine if this event is able to be transitioned to.
    bool (*param)(ScheduleEvent* event);
};

#endif