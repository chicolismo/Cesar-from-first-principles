cmake_minimum_required(VERSION 3.7)
project(cesar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(ALL_SOURCE_FILES
    "source/alu.cpp"
    "source/constants.h"
    "source/cpu.cpp"
    "source/hardware.h"
    "source/gui/gui.h"
    "source/gui/main_window.cpp"
    "source/gui/side_windows.cpp"
    "source/gui/tables.cpp"
    )

set(TEST_SOURCE_FILES
    "source/alu.cpp"
    "source/constants.h"
    "source/cpu.cpp"
    "source/hardware.h"
    )

find_package(wxWidgets COMPONENTS all REQUIRED)
find_package(Threads REQUIRED)

include(ExternalProject)
ExternalProject_Add(gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
    )
ExternalProject_Get_Property(gtest source_dir binary_dir)
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
set_target_properties(libgtest PROPERTIES
    IMPORTED_LOCATION "gtest/src/gtest-build/lib/libgtest.a"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
    )
add_library(libgmock IMPORTED STATIC GLOBAL)
set_target_properties(libgmock PROPERTIES
    IMPORTED_LOCATION "gtest/src/gtest-build/lib/libgmock.a"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
    )
include_directories("${source_dir}/googletest/include" "${source_dir}/googlemock/include")

if (wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    add_executable(cesar "source/main.cpp" ${ALL_SOURCE_FILES})
    add_executable(run_tests "source/test.cpp" ${TEST_SOURCE_FILES})

    target_link_libraries(cesar ${wxWidgets_LIBRARIES} ${Threads_LIBRARIES})
    target_compile_options(cesar PRIVATE -g -Werror -Wall -Wextra)

    target_link_libraries(run_tests libgtest libgmock)
    target_compile_options(run_tests PRIVATE -g -Werror -Wall -Wextra)
endif()

