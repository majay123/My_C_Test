cmake_minimum_required(VERSION 3.8)
project(epoll_serials)
include (utils.cmake)

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst  ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='"$(notdir $<)"'")
add_definitions(-Wno-builtin-macro-redefined)
set(SOURCE_FILES
        epoll.c
        epoll.h
        main.c
        threadpool.c
        threadpool.h
        es_debug.h)

add_executable(epoll_serials ${SOURCE_FILES})
redefine_file_macro(epoll_serials)

