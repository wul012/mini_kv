if(NOT DEFINED DOC_PATH)
    message(FATAL_ERROR "DOC_PATH is required")
endif()

if(NOT EXISTS "${DOC_PATH}")
    message(FATAL_ERROR "project orientation guide is missing: ${DOC_PATH}")
endif()

file(READ "${DOC_PATH}" doc_text)

set(required_fragments
    "# mini-kv"
    "flowchart LR"
    "sequenceDiagram"
    "CommandProcessor"
    "WAL"
    "Snapshot"
    "TCP / RESP"
    "CHECKJSON LOAD"
    "SMOKEJSON"
    "read_only"
    "execution_allowed"
    "order_authoritative"
    "managed audit"
    "Node / Java"
)

foreach(fragment IN LISTS required_fragments)
    string(FIND "${doc_text}" "${fragment}" fragment_offset)
    if(fragment_offset EQUAL -1)
        message(FATAL_ERROR "project orientation guide is missing required fragment: ${fragment}")
    endif()
endforeach()

message(STATUS "project orientation guide contract ok: ${DOC_PATH}")
