cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME_STR yapl)
project (${PROJECT_NAME_STR})

# Local project include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

# Build binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-deprecated -Werror -pedantic-errors")

enable_testing()

#add_subdirectory(doxy)
#add_subdirectory(src)
add_subdirectory(unit_test)
#add_subdirectory(samples)

# Installer
FILE(GLOB headers "${CMAKE_SOURCE_DIR}/include/*.h")
INSTALL(FILES ${headers} DESTINATION include/${PROJECT_NAME_STR})
#INSTALL(FILES ${doc_files} DESTINATION ?share?/${PROJECT_NAME_STR})
