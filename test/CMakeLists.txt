#
# Generates the build env for the kestrel engine test code
#
project(test_kestrel)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

# need to include the main engine source 
aux_source_directory(../src SRC_LIST)
include_directories(../src)

# the kestrel.c file contains the main(), so need to exlcude it
list(REMOVE_ITEM SRC_LIST ../src/kestrel.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})

