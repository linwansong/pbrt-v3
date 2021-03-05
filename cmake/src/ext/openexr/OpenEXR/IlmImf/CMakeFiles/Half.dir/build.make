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
include src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/depend.make

# Include the progress variables for this target.
include src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/progress.make

# Include the compile flags for this target's objects.
include src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/flags.make

src/ext/openexr/IlmBase/Half/eLut.h: src/ext/openexr/IlmBase/Half/eLut
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating eLut.h"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half/eLut > /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half/eLut.h

src/ext/openexr/IlmBase/Half/toFloat.h: src/ext/openexr/IlmBase/Half/toFloat
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating toFloat.h"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half/toFloat > /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half/toFloat.h

src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o: src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/flags.make
src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o: ../src/ext/openexr/IlmBase/Half/half.cpp
src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o: src/ext/openexr/IlmBase/Half/eLut.h
src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o: src/ext/openexr/IlmBase/Half/toFloat.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Half.dir/half.cpp.o -c /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/openexr/IlmBase/Half/half.cpp

src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Half.dir/half.cpp.i"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/openexr/IlmBase/Half/half.cpp > CMakeFiles/Half.dir/half.cpp.i

src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Half.dir/half.cpp.s"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/openexr/IlmBase/Half/half.cpp -o CMakeFiles/Half.dir/half.cpp.s

# Object files for target Half
Half_OBJECTS = \
"CMakeFiles/Half.dir/half.cpp.o"

# External object files for target Half
Half_EXTERNAL_OBJECTS =

src/ext/openexr/IlmBase/Half/libHalf.a: src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/half.cpp.o
src/ext/openexr/IlmBase/Half/libHalf.a: src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/build.make
src/ext/openexr/IlmBase/Half/libHalf.a: src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libHalf.a"
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && $(CMAKE_COMMAND) -P CMakeFiles/Half.dir/cmake_clean_target.cmake
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Half.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/build: src/ext/openexr/IlmBase/Half/libHalf.a

.PHONY : src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/build

src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/clean:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half && $(CMAKE_COMMAND) -P CMakeFiles/Half.dir/cmake_clean.cmake
.PHONY : src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/clean

src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/depend: src/ext/openexr/IlmBase/Half/eLut.h
src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/depend: src/ext/openexr/IlmBase/Half/toFloat.h
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac/src/ext/openexr/IlmBase/Half /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half /home/linwan/Documents/URA/pbrt-v3-aac/cmake/src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/openexr/IlmBase/Half/CMakeFiles/Half.dir/depend

