#include "coshash.h"
#include <libdragon.h>

unsigned int hash_fnv1a(const char * key) {
    unsigned int hash = FNV1A_32_OFFSET;
    unsigned int size = strlen(key);
    while (size--) {
        hash ^= key[size];
        hash *= FNV1A_32_PRIME;
    }
    return hash;
}

bool hash_add_pointer(void** hash_table, int hash_table_size, const char* key, void* item) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    bool slot_found = !(hash_table[base_slot]);
    while (!slot_found && i < hash_table_size) {
        i += 1;
        slot_found = !(hash_table[(base_slot+i)%hash_table_size]);
    }
    if (slot_found) {
        hash_table[(base_slot+i)%hash_table_size] = item;
        return true;
    } else {
        return false;
    }
}
int hash_get_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    void* found_item = hash_table[(base_slot)%hash_table_size];
    bool item_found = found_item && strcmp((char*)(found_item+key_offset), key) == 0;
    while (!item_found && i < hash_table_size) {
        i += 1;
        found_item = hash_table[(base_slot+i)%hash_table_size];
        item_found = found_item && strcmp((char*)(found_item+key_offset), key) == 0;
    }
    if (item_found) {
        return (base_slot)%hash_table_size;
    }
    return -1;
}
void* hash_pop_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset) {
    int found_slot = hash_get_pointer(hash_table, hash_table_size, key, key_offset);
    if (found_slot >= 0) {
        void* found_item = hash_table[found_slot];
        hash_table[found_slot] = NULL;
        return found_item;
    }

    return NULL;
}