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
# ============================================================
# Install rules
# ============================================================

install(TARGETS ${PROJECT_NAME}
    EXPORT TinyThreadPoolTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${LIBRARY_PATH}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(EXPORT TinyThreadPoolTargets
    FILE TinyThreadPoolTargets.cmake
    NAMESPACE TinyThreadPool::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyThreadPool
)