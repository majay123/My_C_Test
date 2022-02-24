cmake_minimum_required(VERSION 3.8)
project(epoll_serials)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        epoll.c
        epoll.h
        main.c
        threadpool.c
        threadpool.h
        es_debug.h)

add_executable(epoll_serials ${SOURCE_FILES})