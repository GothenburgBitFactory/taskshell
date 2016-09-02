cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/libshared/src
                     ${TASKSH_INCLUDE_DIRS})

set (tasksh_SRCS diag.cpp
                 help.cpp
                 prompt.cpp
                 review.cpp
                 shell.cpp)

set (libshared_SRCS libshared/src/Color.cpp         libshared/src/Color.h
                    libshared/src/Datetime.cpp      libshared/src/Datetime.h
                    libshared/src/Duration.cpp      libshared/src/Duration.h
                    libshared/src/FS.cpp            libshared/src/FS.h
                    libshared/src/Lexer.cpp         libshared/src/Lexer.h
                    libshared/src/Pig.cpp           libshared/src/Pig.h
                    libshared/src/shared.cpp        libshared/src/shared.h
                    libshared/src/format.cpp        libshared/src/format.h
                    libshared/src/unicode.cpp       libshared/src/unicode.h
                    libshared/src/utf8.cpp          libshared/src/utf8.h
                    libshared/src/wcwidth6.cpp)

add_library (tasksh    STATIC ${tasksh_SRCS})
add_library (libshared STATIC ${libshared_SRCS})
add_executable (tasksh_executable main.cpp)

target_link_libraries (tasksh_executable tasksh libshared ${TASKSH_LIBRARIES})

set_property (TARGET tasksh_executable PROPERTY OUTPUT_NAME "tasksh")

install (TARGETS tasksh_executable DESTINATION ${TASKSH_BINDIR})

