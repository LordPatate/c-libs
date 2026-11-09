cmake_minimum_required(VERSION 3.23)
add_library(LinkedLists)
target_sources(LinkedLists
    PRIVATE
        linked_lists.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            linked_lists.h
)
