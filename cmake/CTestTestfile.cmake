# CMake generated Testfile for 
# Source directory: /home/linwan/Documents/URA/pbrt-v3-aac
# Build directory: /home/linwan/Documents/URA/pbrt-v3-aac/cmake
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pbrt_unit_test "pbrt_test")
set_tests_properties(pbrt_unit_test PROPERTIES  _BACKTRACE_TRIPLES "/home/linwan/Documents/URA/pbrt-v3-aac/CMakeLists.txt;559;ADD_TEST;/home/linwan/Documents/URA/pbrt-v3-aac/CMakeLists.txt;0;")
subdirs("src/ext/openexr")
subdirs("src/ext/glog")
subdirs("src/ext/ptex")
