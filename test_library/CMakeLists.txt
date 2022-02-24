cmake_minimum_required(VERSION VERSION 2.8.5)

project(test_so)

#Forces out of source builds just in case.
MACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD MSG)
    STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
"${CMAKE_BINARY_DIR}" insource)
    GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
    STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
"${PARENTDIR}" insourcesubdir)
    IF(insource OR insourcesubdir)
        MESSAGE(FATAL_ERROR "${MSG}")
    ENDIF(insource OR insourcesubdir)
ENDMACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX $(PWD)/install)
endif()

# for cross compiling toolchain
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(tools /home/ubuntu/Develop/toolchain/arm-openwrt-linux-uclibcgnueabi/toolchain-arm_cortex-a5+neon_gcc-4.8-linaro_uClibc-0.9.33.2_eabi)

set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-openwrt-linux-g++)


add_definitions(-Os -Wall -I.. -DSTAGING_DIR)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# add_library(test_so SHARED library.c)
add_executable(test_so SHARED library.c)


target_include_directories(test_so PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

find_library(libcjson libcjson.so ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(test_so ${libcjson})

