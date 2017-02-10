
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    BoardGameConsoleApp
)
set(PROJECT_SOURCE_DIR
    src
)
set(PROJECT_SOURCE
    ${PROJECT_SOURCE_DIR}/BoardGameConsoleApp.cpp
)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

target_link_libraries(${PROJECT_NAME} BoardGames)
target_link_libraries(${PROJECT_NAME} BoardGameAI)

