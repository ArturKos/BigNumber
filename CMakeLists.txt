cmake_minimum_required(VERSION 3.14)

project(TestBigNumberLibrary)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests src/bignumber.cpp src/digit.cpp test.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
