cmake_minimum_required (VERSION 2.8)
message ("-- Configuring man pages")
set (man_FILES tasksh.1)
foreach (man_FILE ${man_FILES})
  configure_file (
    man/${man_FILE}.in
    man/${man_FILE})
endforeach (man_FILE)

install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man/   DESTINATION ${TASK_MAN1DIR}
                                                      FILES_MATCHING PATTERN "*.1")

