cmake_minimum_required (VERSION 2.8)

project(general_pb2)

aux_source_directory(./src PB_FILES)



include_directories(./include)

find_library(GPIOD libgpiod.so.3 ./lib)

add_executable(tft_demo ${PB_FILES})

target_link_libraries(tft_demo ${GPIOD})
