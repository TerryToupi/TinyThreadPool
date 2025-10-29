cmake_minimum_required(VERSION 3.15)

project(TinyThreadPool_Library)

# set the path for conviniance
set(LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# grab all the files recursivly. No need to add them one by one
file(GLOB_RECURSE LIBRARY_FILES CONFIGURE_DEPENDS "${LIBRARY_PATH}/**.h" "${LIBRARY_PATH}/**.cpp")

add_library(${PROJECT_NAME} STATIC)
add_library(Library::TinyThreadPool ALIAS ${PROJECT_NAME})

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
