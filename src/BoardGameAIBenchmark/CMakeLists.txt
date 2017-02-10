cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    BoardGameAIBenchmark
)
set(PROJECT_SOURCE_DIR
    src
)
set(PROJECT_SOURCE
    ${PROJECT_SOURCE_DIR}/Benchmark.cpp
)

set(PROJECT_HEADERS_DIR
    include
)

set(PROJECT_HEADERS
    ${PROJECT_HEADERS_DIR}/AbstractBenchmark.h
	${PROJECT_HEADERS_DIR}/LiveBenchmark.h
	${PROJECT_HEADERS_DIR}/BenchmarkManager.h
	${PROJECT_HEADERS_DIR}/CSVIterator.h
	${PROJECT_HEADERS_DIR}/CSVRow.h
	${PROJECT_HEADERS_DIR}/RecordedBenchmark.h
)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_HEADERS_DIR})
target_link_libraries(${PROJECT_NAME} BoardGames)
target_link_libraries(${PROJECT_NAME} BoardGameAI)
