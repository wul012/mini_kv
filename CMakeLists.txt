cmake_minimum_required(VERSION 3.20)

project(mini_kv VERSION 0.30.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(minikv
    src/client_history.cpp
    src/command.cpp
    src/resp.cpp
    src/snapshot.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)

target_include_directories(minikv
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(minikv PRIVATE /W4 /permissive-)
else()
    target_compile_options(minikv PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
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

enable_testing()

add_executable(minikv_store_tests
    tests/store_tests.cpp
)

target_link_libraries(minikv_store_tests PRIVATE minikv)

add_test(NAME store_tests COMMAND minikv_store_tests)

add_executable(minikv_command_tests
    tests/command_tests.cpp
)

target_link_libraries(minikv_command_tests PRIVATE minikv)

add_test(NAME command_tests COMMAND minikv_command_tests)

add_executable(minikv_wal_tests
    tests/wal_tests.cpp
)

target_link_libraries(minikv_wal_tests PRIVATE minikv)

add_test(NAME wal_tests COMMAND minikv_wal_tests)

add_executable(minikv_snapshot_tests
    tests/snapshot_tests.cpp
)

target_link_libraries(minikv_snapshot_tests PRIVATE minikv)

add_test(NAME snapshot_tests COMMAND minikv_snapshot_tests)

add_executable(minikv_stress_tests
    tests/stress_tests.cpp
)

target_link_libraries(minikv_stress_tests PRIVATE minikv)

add_test(NAME stress_tests COMMAND minikv_stress_tests)

add_executable(minikv_resp_tests
    tests/resp_tests.cpp
)

target_link_libraries(minikv_resp_tests PRIVATE minikv)

add_test(NAME resp_tests COMMAND minikv_resp_tests)

add_executable(minikv_tcp_server_tests
    tests/tcp_server_tests.cpp
)

target_link_libraries(minikv_tcp_server_tests PRIVATE minikv)

add_test(NAME tcp_server_tests COMMAND minikv_tcp_server_tests)

add_executable(minikv_tcp_resp_tests
    tests/tcp_resp_tests.cpp
)

target_link_libraries(minikv_tcp_resp_tests PRIVATE minikv)

add_test(NAME tcp_resp_tests COMMAND minikv_tcp_resp_tests)

add_executable(minikv_tcp_resp_compat_tests
    tests/tcp_resp_compat_tests.cpp
)

target_link_libraries(minikv_tcp_resp_compat_tests PRIVATE minikv)

add_test(NAME tcp_resp_compat_tests COMMAND minikv_tcp_resp_compat_tests)

add_executable(minikv_tcp_resp_concurrency_tests
    tests/tcp_resp_concurrency_tests.cpp
)

target_link_libraries(minikv_tcp_resp_concurrency_tests PRIVATE minikv)

add_test(NAME tcp_resp_concurrency_tests COMMAND minikv_tcp_resp_concurrency_tests)

add_executable(minikv_client_history_tests
    tests/client_history_tests.cpp
)

target_link_libraries(minikv_client_history_tests PRIVATE minikv)

add_test(NAME client_history_tests COMMAND minikv_client_history_tests)
