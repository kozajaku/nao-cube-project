# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/student/git/nao-cube-project/cube

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/git/nao-cube-project/cube/build-host

# Include any dependencies generated for this target.
include CMakeFiles/cube.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cube.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cube.dir/flags.make

CMakeFiles/cube.dir/main.cpp.o: CMakeFiles/cube.dir/flags.make
CMakeFiles/cube.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/student/git/nao-cube-project/cube/build-host/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cube.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cube.dir/main.cpp.o -c /home/student/git/nao-cube-project/cube/main.cpp

CMakeFiles/cube.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cube.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/student/git/nao-cube-project/cube/main.cpp > CMakeFiles/cube.dir/main.cpp.i

CMakeFiles/cube.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cube.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/student/git/nao-cube-project/cube/main.cpp -o CMakeFiles/cube.dir/main.cpp.s

CMakeFiles/cube.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/cube.dir/main.cpp.o.requires

CMakeFiles/cube.dir/main.cpp.o.provides: CMakeFiles/cube.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/cube.dir/build.make CMakeFiles/cube.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/cube.dir/main.cpp.o.provides

CMakeFiles/cube.dir/main.cpp.o.provides.build: CMakeFiles/cube.dir/main.cpp.o

# Object files for target cube
cube_OBJECTS = \
"CMakeFiles/cube.dir/main.cpp.o"

# External object files for target cube
cube_EXTERNAL_OBJECTS =

sdk/bin/cube: CMakeFiles/cube.dir/main.cpp.o
sdk/bin/cube: CMakeFiles/cube.dir/build.make
sdk/bin/cube: CMakeFiles/cube.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sdk/bin/cube"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cube.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cube.dir/build: sdk/bin/cube
.PHONY : CMakeFiles/cube.dir/build

CMakeFiles/cube.dir/requires: CMakeFiles/cube.dir/main.cpp.o.requires
.PHONY : CMakeFiles/cube.dir/requires

CMakeFiles/cube.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cube.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cube.dir/clean

CMakeFiles/cube.dir/depend:
	cd /home/student/git/nao-cube-project/cube/build-host && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/git/nao-cube-project/cube /home/student/git/nao-cube-project/cube /home/student/git/nao-cube-project/cube/build-host /home/student/git/nao-cube-project/cube/build-host /home/student/git/nao-cube-project/cube/build-host/CMakeFiles/cube.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cube.dir/depend

