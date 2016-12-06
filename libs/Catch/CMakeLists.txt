cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    CatchLibrary
)
set(LIBRARY_NAME
    Catch
)
set(LIBRARY_HEADERS_DIR
    include
)
set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/catch.hpp
)

project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE include)