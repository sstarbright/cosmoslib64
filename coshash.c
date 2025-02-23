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

bool hash_add_pointer(void** hash_table, int hash_table_size, const char* key, void* item, int index_offset) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    bool slot_found = !(hash_table[base_slot]);
    while (!slot_found && i < hash_table_size) {
        i += 1;
        slot_found = !(hash_table[(base_slot+i)%hash_table_size]);
    }
    if (slot_found) {
        hash_table[(base_slot+i)%hash_table_size] = item;
        *(bool*)(item+index_offset) = true;
        return true;
    } else {
        return false;
    }
}
int hash_get_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    void* found_item = hash_table[(base_slot)%hash_table_size];
    bool item_found = found_item && (char*)(found_item+key_offset) == key;
    while (!item_found && i < hash_table_size) {
        i += 1;
        found_item = hash_table[(base_slot+i)%hash_table_size];
        item_found = found_item && (char*)(found_item+key_offset) == key;
    }
    if (item_found) {
        return (base_slot)%hash_table_size;
    }
    return -1;
}
void* hash_pop_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset, int index_offset) {
    int found_slot = hash_get_pointer(hash_table, hash_table_size, key, key_offset);
    if (found_slot >= 0) {
        void* found_item = hash_table[found_slot];
        hash_table[found_slot] = NULL;
        *(bool*)(found_item+index_offset) = false;
        return found_item;
    }

    return NULL;
}

int hash_get_string_slot(char** hash_table, int hash_table_size, const char* key) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    bool slot_found = !(hash_table[base_slot]);
    while (!slot_found && i < hash_table_size) {
        i += 1;
        slot_found = !(hash_table[(base_slot+i)%hash_table_size]);
    }
    if (slot_found) {
        return (base_slot+i)%hash_table_size;
    }
    return -1;
}
int hash_get_string(char** hash_table, int hash_table_size, const char* key) {
    unsigned int base_slot = hash_fnv1a(key)%hash_table_size;
    int i = 0;
    bool string_found = hash_table[(base_slot+i)%hash_table_size] == key;
    while(!string_found && i < hash_table_size) {
        i += 1;
        string_found = hash_table[(base_slot+i)%hash_table_size] == key;
    }
    if (string_found) {
        return (base_slot+i)%hash_table_size;
    }
    return -1;
}