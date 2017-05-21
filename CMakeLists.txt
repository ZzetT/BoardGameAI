
cmake_minimum_required(VERSION 3.0.0)

set(MASTER_PROJECT_NAME
    BoardGameAIMasterProject
)
set(MASTER_PROJECT_SOURCE_DIR
    src
)
set(MASTER_PROJECT_LIBRARIES_DIR
    libs
)
set(MASTER_PROJECT_LIBRARIES 
    BoardGames
    BoardGameAI
    Catch
)
set(MASTER_PROJECT_EXECUTABLES
	BoardGameConsoleApp
	BoardGameAIBenchmark
)


project(${MASTER_PROJECT_NAME})

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS-")	#disable security cookies
endif()


#########
# Test #
#########
enable_testing()

###########
# Project #
###########
foreach(EXECUTABLE ${MASTER_PROJECT_EXECUTABLES})
    add_subdirectory("${MASTER_PROJECT_SOURCE_DIR}/${EXECUTABLE}")
endforeach(EXECUTABLE)

foreach(LIBRARY ${MASTER_PROJECT_LIBRARIES})
    add_subdirectory("${MASTER_PROJECT_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)
