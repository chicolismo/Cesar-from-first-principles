# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/chico/tcc/cesar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/chico/tcc/cesar/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cesar.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cesar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cesar.dir/flags.make

CMakeFiles/cesar.dir/main.cpp.o: CMakeFiles/cesar.dir/flags.make
CMakeFiles/cesar.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chico/tcc/cesar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cesar.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cesar.dir/main.cpp.o -c /Users/chico/tcc/cesar/main.cpp

CMakeFiles/cesar.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cesar.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chico/tcc/cesar/main.cpp > CMakeFiles/cesar.dir/main.cpp.i

CMakeFiles/cesar.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cesar.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chico/tcc/cesar/main.cpp -o CMakeFiles/cesar.dir/main.cpp.s

CMakeFiles/cesar.dir/Cpu.cpp.o: CMakeFiles/cesar.dir/flags.make
CMakeFiles/cesar.dir/Cpu.cpp.o: ../Cpu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/chico/tcc/cesar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/cesar.dir/Cpu.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cesar.dir/Cpu.cpp.o -c /Users/chico/tcc/cesar/Cpu.cpp

CMakeFiles/cesar.dir/Cpu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cesar.dir/Cpu.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/chico/tcc/cesar/Cpu.cpp > CMakeFiles/cesar.dir/Cpu.cpp.i

CMakeFiles/cesar.dir/Cpu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cesar.dir/Cpu.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/chico/tcc/cesar/Cpu.cpp -o CMakeFiles/cesar.dir/Cpu.cpp.s

# Object files for target cesar
cesar_OBJECTS = \
"CMakeFiles/cesar.dir/main.cpp.o" \
"CMakeFiles/cesar.dir/Cpu.cpp.o"

# External object files for target cesar
cesar_EXTERNAL_OBJECTS =

cesar: CMakeFiles/cesar.dir/main.cpp.o
cesar: CMakeFiles/cesar.dir/Cpu.cpp.o
cesar: CMakeFiles/cesar.dir/build.make
cesar: CMakeFiles/cesar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/chico/tcc/cesar/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable cesar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cesar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cesar.dir/build: cesar

.PHONY : CMakeFiles/cesar.dir/build

CMakeFiles/cesar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cesar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cesar.dir/clean

CMakeFiles/cesar.dir/depend:
	cd /Users/chico/tcc/cesar/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/chico/tcc/cesar /Users/chico/tcc/cesar /Users/chico/tcc/cesar/cmake-build-debug /Users/chico/tcc/cesar/cmake-build-debug /Users/chico/tcc/cesar/cmake-build-debug/CMakeFiles/cesar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cesar.dir/depend

