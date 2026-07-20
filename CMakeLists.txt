cmake_minimum_required(VERSION 3.20)

project(mini_kv VERSION 0.102.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MINIKV_CURRENT_ARCHIVE_VERSION
    "c/102/"
    CACHE STRING "Archive path hint for the frozen current runtime fixture exposed in generated version.hpp"
)

option(MINIKV_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer for GCC/Clang Debug checks" OFF)
option(MINIKV_COVERAGE "Enable gcov-compatible coverage instrumentation for GCC/Clang Debug checks" OFF)

set(_minikv_bundle_tests_default ON)
if(MINIKV_SANITIZE OR MINIKV_COVERAGE)
    set(_minikv_bundle_tests_default OFF)
endif()
option(MINIKV_BUNDLE_TESTS "Link ordinary tests through stable shared runners" ${_minikv_bundle_tests_default})
unset(_minikv_bundle_tests_default)
set(MINIKV_TEST_SHARD_COUNT 8)

if(MINIKV_SANITIZE AND MINIKV_COVERAGE)
    message(FATAL_ERROR "MINIKV_SANITIZE and MINIKV_COVERAGE are separate CI lanes; enable only one at a time")
endif()

if((MINIKV_SANITIZE OR MINIKV_COVERAGE) AND MINIKV_BUNDLE_TESTS)
    message(FATAL_ERROR "instrumented lanes require MINIKV_BUNDLE_TESTS=OFF to preserve per-test gcov/sanitizer isolation")
endif()

if(MINIKV_SANITIZE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message(FATAL_ERROR "MINIKV_SANITIZE requires ASan/UBSan runtime libraries; the bundled Windows MinGW toolchain does not provide -lasan/-lubsan. Use the Ubuntu sanitizer CI job.")
        endif()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    else()
        message(FATAL_ERROR "MINIKV_SANITIZE requires a GCC or Clang compatible compiler")
    endif()
endif()

if(MINIKV_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message(FATAL_ERROR "MINIKV_COVERAGE writes gcov sidecar files whose paths exceed the bundled Windows MinGW limit in this repository. Use the Ubuntu coverage CI job.")
        endif()
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(FATAL_ERROR "MINIKV_COVERAGE requires a GCC or Clang compatible compiler")
    endif()
endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/include/minikv/version.hpp.in
    ${PROJECT_BINARY_DIR}/generated/include/minikv/version.hpp
    @ONLY
)

include(cmake/MinikvSources.cmake)

minikv_read_product_sources(
    "${PROJECT_SOURCE_DIR}/config/product-sources.txt"
    "${PROJECT_SOURCE_DIR}"
    MINIKV_PRODUCT_SOURCES
)

add_library(minikv ${MINIKV_PRODUCT_SOURCES})

target_include_directories(minikv
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/generated/include
)

if(MSVC)
    target_compile_options(minikv PRIVATE /W4 /permissive-)
else()
    target_compile_options(minikv PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    target_compile_definitions(minikv PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(minikv PUBLIC ws2_32)
endif()

add_executable(minikv_cli
    src/main.cpp
)

target_link_libraries(minikv_cli PRIVATE minikv)

add_executable(minikv_server
    src/server_main.cpp
)

target_link_libraries(minikv_server PRIVATE minikv)

add_executable(minikv_client
    src/client_main.cpp
)

target_link_libraries(minikv_client PRIVATE minikv)

add_executable(minikv_benchmark
    src/benchmark_main.cpp
)

target_link_libraries(minikv_benchmark PRIVATE minikv)

add_executable(minikv_osfs
    src/osfs_main.cpp
)

target_link_libraries(minikv_osfs PRIVATE minikv)

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(minikv_osfs PRIVATE -municode)
endif()

include(cmake/MinikvTesting.cmake)

enable_testing()
include(cmake/MinikvTestSuite.cmake)
