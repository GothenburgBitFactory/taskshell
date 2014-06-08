cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${TASKSH_INCLUDE_DIRS})

set (tasksh_SRCS Color.cpp Color.h)

add_executable (tasksh_executable main.cpp ${task_SRCS})

set_property (TARGET tasksh_executable PROPERTY OUTPUT_NAME "tasksh")

install (TARGETS tasksh_executable DESTINATION ${TASKSH_BINDIR})

