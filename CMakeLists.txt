cmake_minimum_required(VERSION 3.15)
project(TinyThreadPool)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

option(ENABLE_TEST "Enabling tests!" OFF)

add_subdirectory(library)

if (ENABLE_TEST)
    add_subdirectory(tests)
endif()
