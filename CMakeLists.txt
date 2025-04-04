cmake_minimum_required(VERSION 3.10)
project(MemoryOrderBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
    main.cpp
    counter_seq_cst.cpp
    counter_relaxed.cpp
    counter_acq_rel.cpp
)

add_executable(counter_test ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(counter_test Threads::Threads)
