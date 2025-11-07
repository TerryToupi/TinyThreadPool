cmake_minimum_required(VERSION 3.15)

project(TinyThreadPool_Library VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

# Set up library path and gather source files
set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE LIBRARY_FILES CONFIGURE_DEPENDS
    "${LIBRARY_PATH}/**.h"
    "${LIBRARY_PATH}/**.hpp"
    "${LIBRARY_PATH}/**.cpp"
)

# Create the library target
add_library(${PROJECT_NAME} STATIC)
add_library(TinyThreadPool::Library ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${LIBRARY_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

source_group(
    TREE ${LIBRARY_PATH}
    PREFIX "Library"
    FILES ${LIBRARY_FILES}
)

# ===============================
# Installation and Export Section
# ===============================

# Install the library and headers
install(TARGETS ${PROJECT_NAME}
    EXPORT TinyThreadPoolTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export target for find_package()
install(EXPORT TinyThreadPoolTargets
    FILE TinyThreadPoolTargets.cmake
    NAMESPACE TinyThreadPool::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)

# Create a Config file for find_package()
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TinyThreadPoolConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)
