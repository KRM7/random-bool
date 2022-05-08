cmake_minimum_required(VERSION 3.20)

project(bits LANGUAGES CXX)

add_executable(singlethread ${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark_singlethread.cpp)
add_executable(multithread ${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark_multithread.cpp)

set(WFLAGS -Wall -Wextra -pedantic)
set(OFLAGS -O2 -flto)

target_compile_features(singlethread PRIVATE cxx_std_20)
target_compile_options(singlethread PRIVATE -DNDEBUG ${WFLAGS} ${OFLAGS})
target_compile_features(multithread PRIVATE cxx_std_20)
target_compile_options(multithread PRIVATE -DNDEBUG ${WFLAGS} ${OFLAGS})

find_package(benchmark REQUIRED)
target_link_libraries(singlethread benchmark::benchmark)
target_link_libraries(multithread benchmark::benchmark)