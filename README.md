# Simple Hash Table

This is a simple implementation of a hash table in C.
The implementation uses double hashing as a trivial way to handle
collisions.

## Usage Example

```c
hash_table *ht = hash_table_new(); // create a new table
insert(ht, "test", "value"); // insert a value
char * val = search(ht, "test"); // search for the value
printf("found %s", val); // print the value to stdout
delete_value(ht, "test"); // delete the value
delete_hash_table(ht); // delete the table and all of its items
```

## Building

**Requirements:**
- CMake
- C Compiler (Clang, MSVC, GCC or similar)

***Nixes:**
```bash
mkdir build
cd build
cmake ..
make
```

**Windows:**
_(make sure to use the visual studio command prompt)_
```bash
mkdir build
cd build
cmake ..
msbuild.exe hash-table.sln
```
