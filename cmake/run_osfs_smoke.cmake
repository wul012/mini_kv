if(NOT DEFINED OSFS_PATH)
    message(FATAL_ERROR "OSFS_PATH is required")
endif()

if(NOT DEFINED DISK_PATH)
    message(FATAL_ERROR "DISK_PATH is required")
endif()

if(NOT DEFINED SCRIPT_PATH)
    message(FATAL_ERROR "SCRIPT_PATH is required")
endif()

file(REMOVE "${DISK_PATH}")

execute_process(
    COMMAND "${OSFS_PATH}" --disk "${DISK_PATH}" --format --blocks 96 --script "${SCRIPT_PATH}"
    RESULT_VARIABLE osfs_result
    OUTPUT_VARIABLE osfs_output
    ERROR_VARIABLE osfs_error
)

file(REMOVE "${DISK_PATH}")

if(NOT osfs_result EQUAL 0)
    message(FATAL_ERROR "minikv_osfs smoke failed with ${osfs_result}\nstdout:\n${osfs_output}\nstderr:\n${osfs_error}")
endif()

if(NOT osfs_output MATCHES "mini-kv osfs course shell")
    message(FATAL_ERROR "minikv_osfs smoke did not print the shell banner\n${osfs_output}")
endif()

if(NOT osfs_output MATCHES "OK created report")
    message(FATAL_ERROR "minikv_osfs smoke did not create report\n${osfs_output}")
endif()

if(NOT osfs_output MATCHES "ERR authentication failed")
    message(FATAL_ERROR "minikv_osfs smoke did not reject the wrong password\n${osfs_output}")
endif()

if(NOT osfs_output MATCHES "course design storage layer")
    message(FATAL_ERROR "minikv_osfs smoke did not read the written file\n${osfs_output}")
endif()

if(NOT osfs_output MATCHES "ERR permission denied")
    message(FATAL_ERROR "minikv_osfs smoke did not enforce write permission denial\n${osfs_output}")
endif()

if(NOT osfs_output MATCHES "report [0-9]+ [0-9]+ 0600 1000 27")
    message(FATAL_ERROR "minikv_osfs smoke did not expose the expected directory row\n${osfs_output}")
endif()
