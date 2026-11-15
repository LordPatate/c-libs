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
target_compile_options(Levenshtein
PUBLIC
    -Wall
    -Wextra
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
target_compile_options(TestLevenshtein
PUBLIC
    -Wall
    -Wextra
    -g
    )
target_link_options(TestLevenshtein
PUBLIC
    -fsanitize=address
)

add_subdirectory(res_or_err)
