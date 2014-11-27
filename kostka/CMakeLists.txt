cmake_minimum_required(VERSION 2.8)
project(kostka)

find_package(qibuild)

# Create a executable named kostka
# with the source file: main.cpp
qi_create_lib(mymodule SHARED ${_srcs} SUBFOLDER naoqi)

qi_use_lib(kostka ALCOMMON ALPROXIES BOOST OPENCV2_CORE OPENCV2_HIGHGUI OPENCV2_IMGPROC)

