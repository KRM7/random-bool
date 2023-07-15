cmake_minimum_required(VERSION 3.21)

project(random_bool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

if(NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release|RelWithDebInfo)$")
    message(WARNING "The specified build type [${CMAKE_BUILD_TYPE}] is not recognized. Defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -permissive- -W4 -WX -diagnostics:caret")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -pedantic-errors -g")
endif()

if(CMAKE_BUILD_TYPE MATCHES "(Release|RelWithDebInfo)")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(random_bool ${SOURCES} ${HEADERS})

find_package(benchmark REQUIRED)
target_link_libraries(random_bool PRIVATE benchmark::benchmark_main)
target_compile_features(random_bool PUBLIC "cxx_std_20")
target_compile_options(random_bool PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-Zc:preprocessor>" "$<$<CXX_COMPILER_ID:MSVC>:-Zc:__cplusplus>")
