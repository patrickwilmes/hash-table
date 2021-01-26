cmake_minimum_required(VERSION 3.17)
project(hash_table C)

set(CMAKE_C_STANDARD 99)

set(SRC
        src/hash_table.c
        src/hash_table.h

        src/prime.c
        src/prime.h

        src/main.c
        )

add_executable(hash_table ${SRC})