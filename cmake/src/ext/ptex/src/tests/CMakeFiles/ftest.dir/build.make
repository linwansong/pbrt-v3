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
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend.make

# Include the progress variables for this target.
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/ptex/src/tests/CMakeFiles/ftest.dir/flags.make

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/flags.make
src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o: ../src/ext/ptex/src/tests/ftest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ftest.dir/ftest.cpp.o -c /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/ptex/src/tests/ftest.cpp

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ftest.dir/ftest.cpp.i"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/ptex/src/tests/ftest.cpp > CMakeFiles/ftest.dir/ftest.cpp.i

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ftest.dir/ftest.cpp.s"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/ptex/src/tests/ftest.cpp -o CMakeFiles/ftest.dir/ftest.cpp.s

# Object files for target ftest
ftest_OBJECTS = \
"CMakeFiles/ftest.dir/ftest.cpp.o"

# External object files for target ftest
ftest_EXTERNAL_OBJECTS =

src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/ftest.cpp.o
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build.make
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/ptex/libPtex.a
src/ext/ptex/src/tests/ftest: /usr/lib/x86_64-linux-gnu/libz.so
src/ext/ptex/src/tests/ftest: src/ext/ptex/src/tests/CMakeFiles/ftest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ftest"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build: src/ext/ptex/src/tests/ftest

.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/build

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/clean:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests && $(CMAKE_COMMAND) -P CMakeFiles/ftest.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/clean

src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/ptex/src/tests /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/src/tests/CMakeFiles/ftest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/src/tests/CMakeFiles/ftest.dir/depend

