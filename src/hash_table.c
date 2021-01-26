#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "hash_table.h"
#include "prime.h"

static table_item DELETED_ITEM = {NULL, NULL};
static const int PRIME_1 = 151;
static const int PRIME_2 = 163;
static const int INITIAL_BASE_SIZE = 50;

static table_item* new_item(const char* key, const char* value) {
    table_item *i = malloc(sizeof(table_item));
    i->key = _strdup(key);
    i->value = _strdup(value);
    return i;
}

static void delete_item(table_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

static int hash(const char *string_to_hash, const int a, const int max) {
    unsigned long hash = 0;
    const unsigned int len_s = strlen(string_to_hash) & INT_MAX;
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * string_to_hash[i];
        hash = hash % max;
    }
    return (int)hash;
}

static int get_hash(const char *string_to_hash, const int num_buckets, const int attempt) {
    const int hash_a = hash(string_to_hash, PRIME_1, num_buckets);
    const int hash_b = hash(string_to_hash, PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

static hash_table * new_sized(const int base_size) {
    hash_table *ht = malloc(sizeof(hash_table));
    if (ht == NULL) {
        printf("Failed to allocate memory for resizing the hash table!");
        return NULL;
    }
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(table_item*));
    return ht;
}

static void resize(hash_table *ht, const int base_size) {
    if (base_size < INITIAL_BASE_SIZE) {
        return;
    }
    hash_table *new_ht = new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        table_item *item = ht->items[i];
        if (item != NULL && item != &DELETED_ITEM) {
            insert(new_ht, item->key, item->value);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;
    table_item **tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;
    delete_hash_table(new_ht);
}

static void resize_up(hash_table *ht) {
    const int new_size = ht->base_size * 2;
    resize(ht, new_size);
}

static void resize_down(hash_table *ht) {
    const int new_size = ht->base_size / 2;
    resize(ht, new_size);
}

hash_table* hash_table_new() {
    hash_table *ht = malloc(sizeof(hash_table));
    ht->base_size = INITIAL_BASE_SIZE;
    ht->count = 0;
    ht->size = 53;
    ht->items = calloc((size_t)ht->size, sizeof(table_item*));
    return ht;
}

void delete_hash_table(hash_table *ht) {
    for (int i = 0; i < ht->size; i++) {
        table_item* item = ht->items[i];
        if (item != NULL && item != &DELETED_ITEM) {
            delete_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

void insert(hash_table *ht, const char *key, const char *value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        resize_up(ht);
    }
    table_item *item = new_item(key, value);
    int index = get_hash(key, ht->size, 0);
    table_item *current = ht->items[index];
    int attempt = 1;
    while (current != NULL && current != &DELETED_ITEM) {
        index = get_hash(key, ht->size, attempt);
        current = ht->items[index];
        attempt++;
    }
    ht->items[index] = item;
    ht->count++;
}

char *search(hash_table *ht, const char *key) {
    int index = get_hash(key, ht->size, 0);
    table_item *current = ht->items[index];
    int attempt = 1;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        index = get_hash(key, ht->size, attempt);
        current = ht->items[index];
        attempt++;
    }
    return NULL;
}

void delete_value(hash_table *ht, const char *key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        resize_down(ht);
    }
    int index = get_hash(key, ht->size, 0);
    table_item *current = ht->items[index];
    int attempt = 1;
    while (current != NULL) {
        if (current != &DELETED_ITEM) {
            if (strcmp(current->key, key) == 0) {
                delete_item(current);
                ht->items[index] = &DELETED_ITEM;
                return;
            }
        }
        index = get_hash(key, ht->size, attempt);
        current = ht->items[index];
        attempt++;
    }
    ht->count--;
}
