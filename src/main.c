#include <stdio.h>

#include "hash_table.h"

int main() {
    hash_table *ht = hash_table_new();
    insert(ht, "test", "value");
    char * val = search(ht, "test");
    printf("found %s", val);
    delete_value(ht, "test");
    delete_hash_table(ht);
    return 0;
}
