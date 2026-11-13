cmake_minimum_required(VERSION 3.23)
add_library(Levenshtein)
target_sources(Levenshtein
    PRIVATE
        levenshtein.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            levenshtein.h
)
target_link_libraries(Levenshtein
PUBLIC
    ResOrErr
)

add_executable(TestLevenshtein)
target_sources(TestLevenshtein
PRIVATE
    test/main.c
)
target_link_libraries(TestLevenshtein
PUBLIC
    Levenshtein
)

add_subdirectory(res_or_err)
