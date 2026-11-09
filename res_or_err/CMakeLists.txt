cmake_minimum_required(VERSION 3.23)
add_library(ResOrErr)
target_sources(ResOrErr
    PRIVATE
        res_or_err.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            res_or_err.h
)
