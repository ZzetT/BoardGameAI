cmake_minimum_required(VERSION 3.0.0)

set(TEST_PROJECT_NAME
    TestBoardGames
)
set(LIBRARY_TESTS_SOURCE
    Connect4LogicTest.cpp
    TestMain.cpp
)

project(${TEST_PROJECT_NAME})

enable_testing()

add_executable(${TEST_PROJECT_NAME} ${LIBRARY_TESTS_SOURCE})
target_link_libraries(${TEST_PROJECT_NAME} Catch)
target_link_libraries(${TEST_PROJECT_NAME} BoardGames)
target_compile_options(${TEST_PROJECT_NAME} PRIVATE $<$<AND:$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Windows>>:/EHsc>)



add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})