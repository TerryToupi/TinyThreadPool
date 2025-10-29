cmake_minimum_required(VERSION 3.15)

project(Tests)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        test1.cpp
)

target_compile_features(${PROJECT_NAME}
    PUBLIC 
        cxx_std_17
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Library::TinyThreadPool
)
