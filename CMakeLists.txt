cmake_minimum_required(VERSION 3.10)

project(test C)

FILE(GLOB SOURCE ${PROJECT_SOURCE_DIR}/*.c)

include_directories(test ${PROJECT_SOURCE_DIR})
add_executable(test ${SOURCE})