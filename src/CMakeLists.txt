cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${TASKSH_INCLUDE_DIRS})

set (tasksh_SRCS diag.cpp
                 help.cpp
                 prompt.cpp
                 review.cpp
                 Color.cpp Color.h
                 FS.cpp FS.h
                 text.cpp text.h
                 util.cpp util.h)

add_executable (tasksh_executable main.cpp ${tasksh_SRCS})
target_link_libraries (tasksh_executable ${TASKSH_LIBRARIES})

set_property (TARGET tasksh_executable PROPERTY OUTPUT_NAME "tasksh")

install (TARGETS tasksh_executable DESTINATION ${TASKSH_BINDIR})

