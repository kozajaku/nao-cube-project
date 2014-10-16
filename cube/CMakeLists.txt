cmake_minimum_required(VERSION 2.8)
project(cube)

find_package(qibuild)

# Create a executable named cube
# with the source file: main.cpp
qi_create_bin(cube "main.cpp")

# Add a simple test:
enable_testing()
qi_create_test(test_cube "test.cpp")
