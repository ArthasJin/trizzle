# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiaolei/work/trizzle/MyCppGame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaolei/work/trizzle/MyCppGame/linux-build

# Utility rule file for trizzle_PRE_BUILD.

# Include the progress variables for this target.
include CMakeFiles/trizzle_PRE_BUILD.dir/progress.make

CMakeFiles/trizzle_PRE_BUILD:

trizzle_PRE_BUILD: CMakeFiles/trizzle_PRE_BUILD
trizzle_PRE_BUILD: CMakeFiles/trizzle_PRE_BUILD.dir/build.make
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "trizzle_PRE_BUILD ..."
	/usr/bin/cmake -E remove_directory /home/xiaolei/work/trizzle/MyCppGame/linux-build/bin/Resources
	/usr/bin/cmake -E copy_directory /home/xiaolei/work/trizzle/MyCppGame/Resources /home/xiaolei/work/trizzle/MyCppGame/linux-build/bin/Resources
.PHONY : trizzle_PRE_BUILD

# Rule to build all files generated by this target.
CMakeFiles/trizzle_PRE_BUILD.dir/build: trizzle_PRE_BUILD
.PHONY : CMakeFiles/trizzle_PRE_BUILD.dir/build

CMakeFiles/trizzle_PRE_BUILD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/trizzle_PRE_BUILD.dir/cmake_clean.cmake
.PHONY : CMakeFiles/trizzle_PRE_BUILD.dir/clean

CMakeFiles/trizzle_PRE_BUILD.dir/depend:
	cd /home/xiaolei/work/trizzle/MyCppGame/linux-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaolei/work/trizzle/MyCppGame /home/xiaolei/work/trizzle/MyCppGame /home/xiaolei/work/trizzle/MyCppGame/linux-build /home/xiaolei/work/trizzle/MyCppGame/linux-build /home/xiaolei/work/trizzle/MyCppGame/linux-build/CMakeFiles/trizzle_PRE_BUILD.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/trizzle_PRE_BUILD.dir/depend

