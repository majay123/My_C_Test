cmake_minimum_required(VERSION 3.8)
project(regex_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        cJSON.c
        cJSON.h
        main.c)

add_executable(regex_test ${SOURCE_FILES})
target_link_libraries(regex_test -lm)