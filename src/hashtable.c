#define _DEFAULT_SOURCE
#include "hashtable.h"

struct ht_entry {
	const char* key;  // key is NULL if this slot is empty
	void* value;
	ht_entry* next;
};

struct ht {
	ht_entry* entries;
	size_t capacity;
	size_t length;
};


#define INITIAL_CAPACITY 16

ht* ht_create(void){
	ht* table = malloc(sizeof(ht));
	if(table == NULL){
		return NULL;
	}

	table->length = 0;
	table->capacity = INITIAL_CAPACITY;

	table->entries = calloc(table->capacity, sizeof(ht_entry));
	if(table->entries == NULL){
		free(table);
		return NULL;
	}

	return table;
}

void ht_destroy(ht* table){
	for(size_t i = 0; i < table->capacity; ++i){
		if(table->entries[i].key != NULL){
			free((void*)table->entries[i].key);
			ht_entry *ent = table->entries[i].next;
			while(ent != NULL){
				ht_entry *old = ent;
				ent = ent->next;
				free(old);
			}
			free(ent);
		}
	}

	free(table->entries);
	free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char* key){
	uint64_t hash = FNV_OFFSET;
	for(const char* p = key; *p; p++){
		hash ^= (uint64_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
	}

	return hash;
}

ht_entry* ht_get(ht* table, const char* key){
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

	while(table->entries[index].key != NULL){
		if(strcmp(key, table->entries[index].key) == 0){
			return &table->entries[index];
		}

		index++;
		if(index >= table->capacity){
			index = 0;
		}
	}

	return NULL;
}

static const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* plength){
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

	while(entries[index].key != NULL){
		if(strcmp(key, entries[index].key) == 0){ //if value exit append to is tail
			// TODO: Fix error in the linking now strange max depth beaviour
            		ht_entry *ent = malloc(sizeof(ht_entry));
			ent->key = (char*)key;
			ent->value = value;
			ent->next = entries[index].next;
			entries[index].next = ent;
            		return entries[index].key;
        	}

		index++;
        	if (index >= capacity) {
            		index = 0;
        	}
	}

	if(plength != NULL){
        	key = strdup(key);
        	if (key == NULL) {
            		return NULL;
        	}
        	(*plength)++;
    	}

	entries[index].key = (char*)key;
    	entries[index].value = value;
    	entries[index].next = NULL;
	return key;
}

static int ht_expand(ht* table){
    	size_t new_capacity = table->capacity * 2;
    	if(new_capacity < table->capacity){
    		return 0;  // overflow (capacity would be too big)
    	}
    	ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    	if(new_entries == NULL){
    		return 0;
    	}

    	for(size_t i = 0; i < table->capacity; i++){
        	ht_entry entry = table->entries[i];
        	if(entry.key != NULL) {
        		ht_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
        	}
    	}

    	free(table->entries);
    	table->entries = new_entries;
    	table->capacity = new_capacity;
    	return 1;
}

const char* ht_set(ht* table, const char* key, void* value){
	assert(value != NULL);
	if(value == NULL){
		return NULL;
	}

	if(table->length >= table->capacity / 2){
		if(!ht_expand(table)) return NULL;
	}

	 return ht_set_entry(table->entries, table->capacity, key, value, &table->length);
}

size_t ht_length(ht* table){
	return table->length;
}

hti ht_iterator(ht* table){
	hti it;
	it._table = table;
	it._index = 0;
	return it;
}

int ht_next(hti* it){
	ht* table = it->_table;
    	while (it->_index < table->capacity) {
        	size_t i = it->_index;
        	it->_index++;
        	if (table->entries[i].key != NULL) {
            		ht_entry entry = table->entries[i];
            		it->key = entry.key;
            		it->value = entry.value;
			it->next = entry.next;
            		return 1;
        	}
    	}
    	return 0;
}
