#include "coslink.h"
#include <libdragon.h>

void linked_add_to_list(void* last_item, void* first_item, void* item_to_add, int p_offset, int n_offset) {
    *(void**)(first_item+p_offset) = item_to_add;
    *(void**)(last_item+n_offset) = item_to_add;
    *(void**)(item_to_add+p_offset) = last_item;
    *(void**)(item_to_add+n_offset) = first_item;
}

bool linked_pop_from_list(void* item_to_pop, int p_offset, int n_offset) {
    void* previous_item = *(void**)(item_to_pop+p_offset);
    void* next_item = *(void**)(item_to_pop+n_offset);

    if (previous_item != item_to_pop) {
        *(void**)(previous_item+n_offset) = next_item;
        *(void**)(next_item+p_offset) = previous_item;
        *(void**)(item_to_pop+p_offset) = item_to_pop;
        *(void**)(item_to_pop+n_offset) = item_to_pop;
        return true;
    }
    return false;
}

void linked_kill_list(void* start_item, void (*kill_function)(void* item), int p_offset, int n_offset) {
    void* prev_item = start_item;
    *(void**)((*(void**)(start_item+p_offset))+n_offset) = NULL;
    void* next_item;

    while(prev_item) {
        next_item = *(void**)(prev_item+n_offset);
        kill_function(prev_item);
        prev_item = next_item;
    }
}