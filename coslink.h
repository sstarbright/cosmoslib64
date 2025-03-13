#ifndef __COSLINK_H
#define __COSLINK_H

#include <libdragon.h>

// An object that can be added to a linked list.
typedef struct LinkedMember LinkedMember;

struct LinkedMember {
    LinkedMember* prev;
    LinkedMember* next;
};

// Add an item to a linked list.
void linked_add_to_list(void* last_item, void* first_item, void* item_to_add, int p_offset, int n_offset);
// Remove an item from a linked list.
// Returns True if other entries still remain in the list.
bool linked_pop_from_list(void* item_to_pop, int p_offset, int n_offset);
// Calls a provided kill function on every entry of a linked list.
void linked_kill_list(void* start_item, void (*kill_function)(void* item), int p_offset, int n_offset);

#endif