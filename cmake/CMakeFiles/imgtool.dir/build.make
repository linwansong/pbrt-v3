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
include CMakeFiles/imgtool.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/imgtool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imgtool.dir/flags.make

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o: CMakeFiles/imgtool.dir/flags.make
CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o: ../src/tools/imgtool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o -c /home/linwan/Documents/URA/pbrt-v3-aac/src/tools/imgtool.cpp

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linwan/Documents/URA/pbrt-v3-aac/src/tools/imgtool.cpp > CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.i

CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linwan/Documents/URA/pbrt-v3-aac/src/tools/imgtool.cpp -o CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.s

# Object files for target imgtool
imgtool_OBJECTS = \
"CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o"

# External object files for target imgtool
imgtool_EXTERNAL_OBJECTS =

imgtool: CMakeFiles/imgtool.dir/src/tools/imgtool.cpp.o
imgtool: CMakeFiles/imgtool.dir/build.make
imgtool: libpbrt.a
imgtool: src/ext/openexr/OpenEXR/IlmImf/libIlmImf.a
imgtool: src/ext/openexr/IlmBase/Imath/libImath.a
imgtool: src/ext/openexr/IlmBase/Half/libHalf.a
imgtool: src/ext/glog/libglog.a
imgtool: src/ext/ptex/src/ptex/libPtex.a
imgtool: /usr/lib/x86_64-linux-gnu/libz.so
imgtool: src/ext/openexr/IlmBase/IlmThread/libIlmThread.a
imgtool: src/ext/openexr/IlmBase/Iex/libIex.a
imgtool: /usr/lib/x86_64-linux-gnu/libz.so
imgtool: CMakeFiles/imgtool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable imgtool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgtool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imgtool.dir/build: imgtool

.PHONY : CMakeFiles/imgtool.dir/build

CMakeFiles/imgtool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imgtool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imgtool.dir/clean

CMakeFiles/imgtool.dir/depend:
	cd /home/linwan/Documents/URA/pbrt-v3-aac/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake /home/linwan/Documents/URA/pbrt-v3-aac/cmake/CMakeFiles/imgtool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imgtool.dir/depend

