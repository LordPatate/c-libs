cmake_minimum_required(VERSION 3.23)
add_library(StringBuilder)
target_sources(StringBuilder
    PRIVATE
        string_builder.c
        string_builder_internals.h
    PUBLIC
        FILE_SET HEADERS
        FILES
            string_builder.h
)
