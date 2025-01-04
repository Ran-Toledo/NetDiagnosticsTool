# CMakeList.txt : CMake project for NetDiagnosticsTool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NetDiagnosticsTool")

# Set SQLite3 source files
set(SQLITE3_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/sqlite3/sqlite3.c)
set(SQLITE3_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/sqlite3/sqlite3.h)

# Set SOURCES to all .cpp files in the Source directory
file(GLOB SOURCES "Source/*.cpp")
file(GLOB HEADERS "Source/*.h")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/sqlite3)

# Add source to this project's executable.
add_executable (NetDiagnosticsTool ${SOURCES} ${HEADERS} ${SQLITE3_SOURCES} ${SQLITE3_HEADERS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NetDiagnosticsTool PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
