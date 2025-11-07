cmake_minimum_required(VERSION 3.15)

project(TinyThreadPool_Library)

# set the path for conviniance
set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# grab all the files recursivly. No need to add them one by one
file(GLOB_RECURSE LIBRARY_FILES CONFIGURE_DEPENDS "${LIBRARY_PATH}/**.h" "${LIBRARY_PATH}/**.cpp")

add_library(${PROJECT_NAME} STATIC)
add_library(TinyThreadPool::Library ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ${LIBRARY_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${LIBRARY_PATH}/include"
)

target_compile_features(${PROJECT_NAME}
    PUBLIC 
        cxx_std_17
)

source_group(
    TREE ${LIBRARY_PATH}
    PREFIX "Library"
    FILES ${LIBRARY_FILES}
)

# ============================================================
# Installation Configuration
# ============================================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the library
install(TARGETS ${PROJECT_NAME}
    EXPORT TinyThreadPoolTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(DIRECTORY ${LIBRARY_PATH}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install export targets
install(EXPORT TinyThreadPoolTargets
    FILE TinyThreadPoolTargets.cmake
    NAMESPACE TinyThreadPool::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)

# Create and install package config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)

# Create and install package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TinyThreadPoolConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)