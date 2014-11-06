cmake_minimum_required(VERSION 2.8)
project(kostka)

find_package(qibuild)

# Create a executable named kostka
# with the source file: main.cpp
qi_create_bin(kostka "main.cpp")
qi_use_lib(kostka ALCOMMON ALPROXIES OPENCV2_CORE OPENCV2_HIGHGUI OPENCV2_IMGPROC)

# Add a simple test:
enable_testing()
qi_create_test(test_kostka "test.cpp")
