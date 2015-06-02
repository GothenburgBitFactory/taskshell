cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/test
                     ${TASKSH_INCLUDE_DIRS})

set (test_SRCS color.t fs.t)

message ("-- Configuring run_all")
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

set (TESTBLOB "./*.t")
if (CYGWIN)
  set (TESTBLOB "./*.t ./*.t.exe")
endif (CYGWIN)

else (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

set (TESTBLOB "${CMAKE_SOURCE_DIR}/test/*.t ${CMAKE_BINARY_DIR}/test/*.t")
if (CYGWIN)
  set (TESTBLOB "${CMAKE_SOURCE_DIR}/test/*.t ${CMAKE_BINARY_DIR}/test/*.t.exe")
endif (CYGWIN)

endif (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

configure_file (run_all.in run_all)

add_custom_target (test ./run_all --verbose
                        DEPENDS ${test_SRCS} tasksh_executable
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test)

add_custom_target (build_tests DEPENDS ${test_SRCS}
                               WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

foreach (src_FILE ${test_SRCS})
  add_executable (${src_FILE} "${src_FILE}.cpp"
                              ../src/Color.cpp
                              ../src/FS.cpp
                              ../src/text.cpp
                              ../src/utf8.cpp
                              ../src/wcwidth6.cpp
                              test.cpp)
  target_link_libraries (${src_FILE} ${TASKSH_LIBRARIES})
endforeach (src_FILE)

