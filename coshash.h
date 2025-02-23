#ifndef __COSHASH_H
#define __COSHASH_H

#include <string.h>
#include <libdragon.h>

#define FNV1A_32_OFFSET 0x811c9dc5
#define FNV1A_32_PRIME 0x01000193

// Hash a key string using the FNV-1a hash - https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
unsigned int hash_fnv1a(const char * key);

// Add a pointer to a Hash Table of pointers.
// Returns True if successful.
bool hash_add_pointer(void** hash_table, int hash_table_size, const char* key, void* item, int index_offset);
// Get a pointer from a Hash Table of pointers.
// Returns the slot within the hash_table, -1 if not found.
int hash_get_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset);
// Get a pointer from a Hash Table of pointers and remove it from the Hash Table.
// Returns a void* pointer that may be converted into the intended pointer type.
void* hash_pop_pointer(void** hash_table, int hash_table_size, const char* key, int key_offset, int index_offset);

// Get a free string slot a Hash Table of strings.
// Returns -1 if no slots are available.
int hash_get_string_slot(char** hash_table, int hash_table_size, const char* key);
// Get a string's position from a Hash Table of strings.
// Returns -1 if it doesn't exist.
int hash_get_string(char** hash_table, int hash_table_size, const char* key);

#endif