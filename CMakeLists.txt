
cmake_minimum_required(VERSION 3.0.0)

set(FOO_PROJECT_NAME
    BoardGameEngineProject
)
set(FOO_SOURCE_DIR
    src
)
set(FOO_SOURCE
    ${FOO_SOURCE_DIR}/BoardGameUI.cpp
)
set(FOO_LIBRARIES_DIR
    libs
)
set(FOO_LIBRARIES 
    BoardGames
    BoardGameEngine
    Catch
)

project(${FOO_PROJECT_NAME})

if (WIN32)
	set(C17_COMPILE_FLAG "-Xclang -std=c++1z")
else()
	set(C17_COMPILE_FLAG "-std=c++1z")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C17_COMPILE_FLAG}")

#########
# Test #
#########
enable_testing()
add_subdirectory(test)


###########
# Project #
###########
add_executable(${FOO_PROJECT_NAME} ${FOO_SOURCE})

foreach(LIBRARY ${FOO_LIBRARIES})
    add_subdirectory("${FOO_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)
target_link_libraries(${FOO_PROJECT_NAME} ${FOO_LIBRARIES})
