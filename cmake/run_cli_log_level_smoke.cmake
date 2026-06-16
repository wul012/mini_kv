if(NOT DEFINED CLI_PATH)
    message(FATAL_ERROR "CLI_PATH is required")
endif()

if(NOT DEFINED INPUT_PATH)
    message(FATAL_ERROR "INPUT_PATH is required")
endif()

execute_process(
    COMMAND "${CLI_PATH}" --log-level debug
    INPUT_FILE "${INPUT_PATH}"
    OUTPUT_VARIABLE cli_stdout
    ERROR_VARIABLE cli_stderr
    RESULT_VARIABLE cli_result
    TIMEOUT 10
)

if(NOT cli_result EQUAL 0)
    message(FATAL_ERROR "minikv_cli --log-level debug failed with ${cli_result}\nstdout:\n${cli_stdout}\nstderr:\n${cli_stderr}")
endif()

if(NOT cli_stdout MATCHES "mini-kv CLI")
    message(FATAL_ERROR "minikv_cli output did not include startup banner\nstdout:\n${cli_stdout}")
endif()

if(NOT cli_stdout MATCHES "BYE")
    message(FATAL_ERROR "minikv_cli output did not include QUIT acknowledgement\nstdout:\n${cli_stdout}")
endif()

message(STATUS "${cli_stdout}")
