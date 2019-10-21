# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/chico/tcc/cesar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chico/tcc/cesar

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running tests..."
	/usr/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test

.PHONY : test/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chico/tcc/cesar/CMakeFiles /home/chico/tcc/cesar/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chico/tcc/cesar/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named cesar

# Build rule for target.
cesar: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 cesar
.PHONY : cesar

# fast build rule for target.
cesar/fast:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/build
.PHONY : cesar/fast

#=============================================================================
# Target rules for targets named gtest

# Build rule for target.
gtest: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 gtest
.PHONY : gtest

# fast build rule for target.
gtest/fast:
	$(MAKE) -f CMakeFiles/gtest.dir/build.make CMakeFiles/gtest.dir/build
.PHONY : gtest/fast

source/alu.o: source/alu.cpp.o

.PHONY : source/alu.o

# target to build an object file
source/alu.cpp.o:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/alu.cpp.o
.PHONY : source/alu.cpp.o

source/alu.i: source/alu.cpp.i

.PHONY : source/alu.i

# target to preprocess a source file
source/alu.cpp.i:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/alu.cpp.i
.PHONY : source/alu.cpp.i

source/alu.s: source/alu.cpp.s

.PHONY : source/alu.s

# target to generate assembly for a file
source/alu.cpp.s:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/alu.cpp.s
.PHONY : source/alu.cpp.s

source/cpu.o: source/cpu.cpp.o

.PHONY : source/cpu.o

# target to build an object file
source/cpu.cpp.o:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/cpu.cpp.o
.PHONY : source/cpu.cpp.o

source/cpu.i: source/cpu.cpp.i

.PHONY : source/cpu.i

# target to preprocess a source file
source/cpu.cpp.i:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/cpu.cpp.i
.PHONY : source/cpu.cpp.i

source/cpu.s: source/cpu.cpp.s

.PHONY : source/cpu.s

# target to generate assembly for a file
source/cpu.cpp.s:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/cpu.cpp.s
.PHONY : source/cpu.cpp.s

source/main.o: source/main.cpp.o

.PHONY : source/main.o

# target to build an object file
source/main.cpp.o:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/main.cpp.o
.PHONY : source/main.cpp.o

source/main.i: source/main.cpp.i

.PHONY : source/main.i

# target to preprocess a source file
source/main.cpp.i:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/main.cpp.i
.PHONY : source/main.cpp.i

source/main.s: source/main.cpp.s

.PHONY : source/main.s

# target to generate assembly for a file
source/main.cpp.s:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/main.cpp.s
.PHONY : source/main.cpp.s

source/test.o: source/test.cpp.o

.PHONY : source/test.o

# target to build an object file
source/test.cpp.o:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/test.cpp.o
.PHONY : source/test.cpp.o

source/test.i: source/test.cpp.i

.PHONY : source/test.i

# target to preprocess a source file
source/test.cpp.i:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/test.cpp.i
.PHONY : source/test.cpp.i

source/test.s: source/test.cpp.s

.PHONY : source/test.s

# target to generate assembly for a file
source/test.cpp.s:
	$(MAKE) -f CMakeFiles/cesar.dir/build.make CMakeFiles/cesar.dir/source/test.cpp.s
.PHONY : source/test.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... cesar"
	@echo "... test"
	@echo "... gtest"
	@echo "... source/alu.o"
	@echo "... source/alu.i"
	@echo "... source/alu.s"
	@echo "... source/cpu.o"
	@echo "... source/cpu.i"
	@echo "... source/cpu.s"
	@echo "... source/main.o"
	@echo "... source/main.i"
	@echo "... source/main.s"
	@echo "... source/test.o"
	@echo "... source/test.i"
	@echo "... source/test.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

