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

# Utility rule file for NightlyMemoryCheck.

# Include the progress variables for this target.
include src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/progress.make

src/ext/ptex/CMakeFiles/NightlyMemoryCheck:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex && /usr/bin/ctest -D NightlyMemoryCheck

NightlyMemoryCheck: src/ext/ptex/CMakeFiles/NightlyMemoryCheck
NightlyMemoryCheck: src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/build.make

.PHONY : NightlyMemoryCheck

# Rule to build all files generated by this target.
src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/build: NightlyMemoryCheck

.PHONY : src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/build

src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/clean:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex && $(CMAKE_COMMAND) -P CMakeFiles/NightlyMemoryCheck.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/clean

src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/depend:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/ptex /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/CMakeFiles/NightlyMemoryCheck.dir/depend

