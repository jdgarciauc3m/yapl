  ExternalProject_Add(googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "release-1.8.0"
    PREFIX external
    BUILD_IN_SOURCE
    UPDATE_COMMAND ""
    CMAKE_CACHE_ARGS
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DBUILD_GMOCK:BOOL=OFF
      -DBUILD_GTEST:BOOL=ON
      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=debug-libs
      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=release-libs
  )  

# Get propperties
ExternalProject_Get_Property(googletest install_dir)

set(GTEST_ROOT ${install_dir} CACHE PATH "Path to googletest")
message("GTest root " ${GTEST_ROOT})
find_package(GTest)
if(NOT GTEST_LIBRARY)
  message(WARNING "gtest not found, call make first to build it and cmake again afterwards")
  return()
endif()
