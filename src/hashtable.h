#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct ht_entry ht_entry;
typedef struct ht ht;

ht* ht_create(void);
void ht_destroy(ht* table);

// use void* to make no type dipendent
ht_entry* ht_get(ht* table, const char* key);
const char* ht_set(ht* table, const char* key, void* value);
size_t ht_length(ht* table);

typedef struct {
    const char* key;
    void* value;
    ht_entry* next;

    ht* _table;
    size_t _index;
} hti;

hti ht_iterator(ht* table);
int ht_next(hti* it);

#endif //HASHTABLE_H_
