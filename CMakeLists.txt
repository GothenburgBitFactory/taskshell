cmake_minimum_required (VERSION 2.8)
set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set (HAVE_CMAKE true)

project (tasksh)
include (CXXSniffer)

set (PROJECT_VERSION "1.2.0")

include (CheckFunctionExists)
include (CheckStructHasMember)
include (CheckCXXCompilerFlag)

message ("-- Looking for SHA1 references")
if (EXISTS ${CMAKE_SOURCE_DIR}/.git/index)
  set (HAVE_COMMIT true)
  execute_process (COMMAND git log -1 --pretty=format:%h
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   OUTPUT_VARIABLE COMMIT)
  configure_file ( ${CMAKE_SOURCE_DIR}/commit.h.in
                   ${CMAKE_SOURCE_DIR}/commit.h)
  message ("-- Found SHA1 reference: ${COMMIT}")
endif (EXISTS ${CMAKE_SOURCE_DIR}/.git/index)

set (PACKAGE "${PROJECT_NAME}")
set (VERSION "${PROJECT_VERSION}")
set (PACKAGE_BUGREPORT "support@taskwarrior.org")
set (PACKAGE_NAME "${PACKAGE}")
set (PACKAGE_TARNAME "${PACKAGE}")
set (PACKAGE_VERSION "${VERSION}")
set (PACKAGE_STRING "${PACKAGE} ${VERSION}")

if (FREEBSD)
SET (TASKSH_MAN1DIR man/man1            CACHE STRING "Installation directory for man pages, section 1")
else (FREEBSD)
SET (TASKSH_MAN1DIR share/man/man1      CACHE STRING "Installation directory for man pages, section 1")
endif (FREEBSD)
SET (TASKSH_DOCDIR  share/doc/tasksh    CACHE STRING "Installation directory for doc files")
SET (TASKSH_RCDIR "${TASKSH_DOCDIR}/rc" CACHE STRING "Installation directory for configuration files")
SET (TASKSH_BINDIR  bin                 CACHE STRING "Installation directory for the binary")

# include the readline library finder module
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# find readline
message ("-- Looking for GNU Readline")
find_package (Readline REQUIRED)
if (READLINE_FOUND)
  set (HAVE_READLINE true)
  set (TASKSH_INCLUDE_DIRS ${TASKSH_INCLUDE_DIRS} ${READLINE_INCLUDE_DIR})
  set (TASKSH_LIBRARIES    ${TASKSH_LIBRARIES}    ${READLINE_LIBRARIES})
endif (READLINE_FOUND)

message ("-- Configuring cmake.h")
configure_file (
  ${CMAKE_SOURCE_DIR}/cmake.h.in
  ${CMAKE_SOURCE_DIR}/cmake.h)

add_subdirectory (src)
add_subdirectory (doc)
if (EXISTS ${CMAKE_SOURCE_DIR}/test)
  add_subdirectory (test EXCLUDE_FROM_ALL)
endif (EXISTS ${CMAKE_SOURCE_DIR}/test)

set (doc_FILES NEWS ChangeLog README.md INSTALL AUTHORS COPYING)
foreach (doc_FILE ${doc_FILES})
  install (FILES ${doc_FILE}  DESTINATION ${TASKSH_DOCDIR})
endforeach (doc_FILE)

# ---

set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})
set (CPACK_SOURCE_IGNORE_FILES  "CMakeCache" "CMakeFiles" "CPackConfig" "CPackSourceConfig"
                                "_CPack_Packages" "cmake_install" "install_manifest" "Makefile$"
                                "test" "package-config" "misc/*" "src/tasksh$" "README.md"
                                "/\\\\.gitignore" "/\\\\.git/" "swp$")
include (CPack)
