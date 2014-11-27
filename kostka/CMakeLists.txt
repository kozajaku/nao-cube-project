cmake_minimum_required(VERSION 2.8)
project(kostka)

find_package(qibuild)

# Create a executable named kostka
# with the source file: main.cpp
#qi_create_lib(kostka SHARED ${_srcs} SUBFOLDER naoqi)

#qi_use_lib(kostka ALCOMMON ALPROXIES BOOST OPENCV2_CORE OPENCV2_IMGPROC)

set (_srcs
     main.cpp
     recognition.cpp
     dieModule.hpp
     dieModule.cpp)

qi_create_bin(mybroker ${_srcs})

qi_use_lib(mybroker ALCOMMON ALPROXIES BOOST OPENCV2_CORE OPENCV2_IMGPROC)