# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/linwan/Documents/URA/pbrt-v3-aac

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linwan/Documents/URA/pbrt-v3-aac/cmake

# Include any dependencies generated for this target.
include src/ext/glog/CMakeFiles/stl_logging_unittest.dir/depend.make

# Include the progress variables for this target.
include src/ext/glog/CMakeFiles/stl_logging_unittest.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/glog/CMakeFiles/stl_logging_unittest.dir/flags.make

src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o: src/ext/glog/CMakeFiles/stl_logging_unittest.dir/flags.make
src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o: ../src/ext/glog/src/stl_logging_unittest.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o -c /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/glog/src/stl_logging_unittest.cc

src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.i"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/glog/src/stl_logging_unittest.cc > CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.i

src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.s"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/glog/src/stl_logging_unittest.cc -o CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.s

# Object files for target stl_logging_unittest
stl_logging_unittest_OBJECTS = \
"CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o"

# External object files for target stl_logging_unittest
stl_logging_unittest_EXTERNAL_OBJECTS =

src/ext/glog/stl_logging_unittest: src/ext/glog/CMakeFiles/stl_logging_unittest.dir/src/stl_logging_unittest.cc.o
src/ext/glog/stl_logging_unittest: src/ext/glog/CMakeFiles/stl_logging_unittest.dir/build.make
src/ext/glog/stl_logging_unittest: src/ext/glog/libglog.a
src/ext/glog/stl_logging_unittest: src/ext/glog/CMakeFiles/stl_logging_unittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stl_logging_unittest"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stl_logging_unittest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/glog/CMakeFiles/stl_logging_unittest.dir/build: src/ext/glog/stl_logging_unittest

.PHONY : src/ext/glog/CMakeFiles/stl_logging_unittest.dir/build

src/ext/glog/CMakeFiles/stl_logging_unittest.dir/clean:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog && $(CMAKE_COMMAND) -P CMakeFiles/stl_logging_unittest.dir/cmake_clean.cmake
.PHONY : src/ext/glog/CMakeFiles/stl_logging_unittest.dir/clean

src/ext/glog/CMakeFiles/stl_logging_unittest.dir/depend:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/glog /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/glog/CMakeFiles/stl_logging_unittest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/glog/CMakeFiles/stl_logging_unittest.dir/depend

