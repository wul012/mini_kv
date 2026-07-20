cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED SOURCE_DIR OR NOT DEFINED MANIFEST_PATH)
    message(FATAL_ERROR "SOURCE_DIR and MANIFEST_PATH are required")
endif()

set(PROJECT_SOURCE_DIR "${SOURCE_DIR}")
include("${SOURCE_DIR}/cmake/MinikvTesting.cmake")
minikv_read_test_manifest("${MANIFEST_PATH}" manifest_rows)

list(LENGTH manifest_rows case_count)
if(NOT case_count EQUAL 342)
    message(FATAL_ERROR "unexpected validated test case count: ${case_count}")
endif()

message(STATUS "validated mini-kv test case manifest: ${case_count} cases")
