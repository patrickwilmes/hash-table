#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

typedef struct {
    char *key;
    char *value;
} table_item;

typedef struct {
    unsigned int size;
    unsigned int count;
    unsigned int base_size;
    table_item **items;
} hash_table;

hash_table* hash_table_new();
void delete_hash_table(hash_table *ht);

void insert(hash_table *ht, const char *key, const char *value);
char* search(hash_table *ht, const char *key);
void delete_value(hash_table *ht, const char *key);

#endif //HASH_TABLE_HASH_TABLE_H
