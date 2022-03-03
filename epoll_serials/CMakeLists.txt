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
        es_debug.h
        rio.c
        rio.h
        priority_queue.c
        priority_queue.h
        timer.c
        timer.h
        serials_requset.c
        serials_requset.h
        util.c
        util.h
        list.h
        serial_common.c
        serial_common.h)

# for cross compiling toolchain
# set(CMAKE_SYSTEM_NAME Linux)
# set(CMAKE_SYSTEM_PROCESSOR arm)
# set(tools /usr/local/toolchain/arm-openwrt-linux-uclibcgnueabi/toolchain-arm_cortex-a5+neon_gcc-4.8-linaro_uClibc-0.9.33.2_eabi)

# set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gcc)
# set(CMAKE_CXX_COMPILER ${tools}/bin/arm-openwrt-linux-g++)

add_executable(epoll_serials ${SOURCE_FILES})
redefine_file_macro(epoll_serials)
# add_definitions(-Os -Wall -I.. -DSTAGING_DIR)
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(epoll_serials -lm -lpthread)

