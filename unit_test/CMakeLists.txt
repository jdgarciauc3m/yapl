# Testing
find_package(GTest)
if(NOT GTEST_LIBRARY)
  message(WARNING "gtest not found, call make first to build it and cmake again afterwards")
  return()
endif()

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_unit_test)
file(GLOB TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} GTest::GTest GTest::Main)

add_test(utest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_TEST_NAME})
