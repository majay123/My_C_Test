cmake_minimum_required(VERSION VERSION 3.10)

project(fixed_count)

set(toolchain_dir /home/ubuntu/Develop/toolchain/arm-openwrt-linux-uclibcgnueabi/toolchain-arm_cortex-a5+neon_gcc-4.8-linaro_uClibc-0.9.33.2_eabi)

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX $(PWD)/install)
endif()



#[[
# for cross compiling toolchain
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(tools ${toolchain_dir})

set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-openwrt-linux-g++)
]]



add_definitions(-Os -Wall -I.. -DSTAGING_DIR -DAPP_NAME=${PROJECT_NAME})
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(PROTO_C_OUT   "${CMAKE_SOURCE_DIR}")
set(PROTO_PATH    "${CMAKE_SOURCE_DIR}")
file (GLOB PROTO_SOURCE_FILES "${CMAKE_SOURCE_DIR}/*.proto")

foreach(input_proto ${PROTO_SOURCE_FILES})
    get_filename_component(DIR ${input_proto} DIRECTORY)
    get_filename_component(FILE_NAME ${input_proto} NAME_WE)

    set(OUTPUT_C_HEADER     "${PROTO_C_OUT}/${FILE_NAME}.pb.h")
    set(OUTPUT_C_SOURCE     "${PROTO_C_OUT}/${FILE_NAME}.pb.c")
    list(APPEND OUTPUT_SOURCES_C
        ${OUTPUT_C_HEADER} ${OUTPUT_C_SOURCE})
endforeach()

add_executable(${PROJECT_NAME} main.c
 pb_common.c
 pb_decode.c
 pb_encode.c
 ${dmessage.pb-c}
 ${OUTPUT_SOURCES_C}
)

# for protobuf description file
# add_custom_command(
#     OUTPUT  ${OUTPUT_SOURCES_C}
# 	COMMAND python ${CMAKE_SOURCE_DIR}/../generator/nanopb_generator.py -D ${PROTO_C_OUT} -I ${PROTO_PATH} dmessage.proto
# 	DEPENDS ${CMAKE_SOURCE_DIR}/dmessage.proto
# 	VERBATIM
#     COMMENT "Generate C Protobuf Source Files"
# )
# add_custom_target(
# 	dmessage.pb-c
# 	ALL
# 	DEPENDS ${OUTPUT_SOURCES_C}
# 	COMMENT "Generate C Protobuf Target Files"
# )

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} pthread rt)

