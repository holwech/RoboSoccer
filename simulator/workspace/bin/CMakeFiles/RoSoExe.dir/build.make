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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniel/dev/RoSo_Simulator/trunk/workspace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin

# Include any dependencies generated for this target.
include CMakeFiles/RoSoExe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RoSoExe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RoSoExe.dir/flags.make

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o: CMakeFiles/RoSoExe.dir/flags.make
CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o: ../src/soccer_sample.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o -c /home/daniel/dev/RoSo_Simulator/trunk/workspace/src/soccer_sample.cpp

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/daniel/dev/RoSo_Simulator/trunk/workspace/src/soccer_sample.cpp > CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.i

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/daniel/dev/RoSo_Simulator/trunk/workspace/src/soccer_sample.cpp -o CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.s

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.requires:
.PHONY : CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.requires

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.provides: CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.requires
	$(MAKE) -f CMakeFiles/RoSoExe.dir/build.make CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.provides.build
.PHONY : CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.provides

CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.provides.build: CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o

# Object files for target RoSoExe
RoSoExe_OBJECTS = \
"CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o"

# External object files for target RoSoExe
RoSoExe_EXTERNAL_OBJECTS =

RoSoExe: CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o
RoSoExe: CMakeFiles/RoSoExe.dir/build.make
RoSoExe: CMakeFiles/RoSoExe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable RoSoExe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RoSoExe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RoSoExe.dir/build: RoSoExe
.PHONY : CMakeFiles/RoSoExe.dir/build

CMakeFiles/RoSoExe.dir/requires: CMakeFiles/RoSoExe.dir/src/soccer_sample.cpp.o.requires
.PHONY : CMakeFiles/RoSoExe.dir/requires

CMakeFiles/RoSoExe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RoSoExe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RoSoExe.dir/clean

CMakeFiles/RoSoExe.dir/depend:
	cd /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/dev/RoSo_Simulator/trunk/workspace /home/daniel/dev/RoSo_Simulator/trunk/workspace /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin /home/daniel/dev/RoSo_Simulator/trunk/workspace/bin/CMakeFiles/RoSoExe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RoSoExe.dir/depend

