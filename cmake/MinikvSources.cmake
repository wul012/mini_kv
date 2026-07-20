include("${CMAKE_CURRENT_LIST_DIR}/MinikvManifest.cmake")

set(MINIKV_PRODUCT_SOURCE_COUNT 316)
set(MINIKV_PRODUCT_MAIN_SOURCES
    src/main.cpp
    src/server_main.cpp
    src/client_main.cpp
    src/benchmark_main.cpp
    src/osfs_main.cpp
)

function(minikv_read_product_sources manifest_path source_root out_sources)
    minikv_require_lf_file("${manifest_path}" "product source manifest")

    file(STRINGS "${manifest_path}" manifest_rows ENCODING UTF-8)
    set(product_sources)
    foreach(source_file IN LISTS manifest_rows)
        string(STRIP "${source_file}" source_file)
        if(NOT source_file MATCHES "^src/[A-Za-z0-9_]+[.]cpp$")
            message(FATAL_ERROR "invalid product source path: ${source_file}")
        endif()
        if(source_file IN_LIST MINIKV_PRODUCT_MAIN_SOURCES)
            message(FATAL_ERROR "executable main must not enter product source manifest: ${source_file}")
        endif()
        if(NOT EXISTS "${source_root}/${source_file}")
            message(FATAL_ERROR "product source is missing: ${source_file}")
        endif()
        if(source_file IN_LIST product_sources)
            message(FATAL_ERROR "duplicate product source: ${source_file}")
        endif()
        list(APPEND product_sources "${source_file}")
    endforeach()

    list(LENGTH product_sources source_count)
    if(NOT source_count EQUAL MINIKV_PRODUCT_SOURCE_COUNT)
        message(FATAL_ERROR
            "product source manifest census changed: expected=${MINIKV_PRODUCT_SOURCE_COUNT}, actual=${source_count}"
        )
    endif()

    if(CMAKE_SCRIPT_MODE_FILE)
        file(GLOB discovered_sources
            RELATIVE "${source_root}"
            "${source_root}/src/*.cpp"
        )
    else()
        file(GLOB discovered_sources
            CONFIGURE_DEPENDS
            RELATIVE "${source_root}"
            "${source_root}/src/*.cpp"
        )
    endif()
    list(REMOVE_ITEM discovered_sources ${MINIKV_PRODUCT_MAIN_SOURCES})
    list(SORT discovered_sources)
    set(sorted_manifest ${product_sources})
    list(SORT sorted_manifest)
    if(NOT "${sorted_manifest}" STREQUAL "${discovered_sources}")
        message(FATAL_ERROR "product source manifest has an unregistered or stale source")
    endif()

    set(${out_sources} "${product_sources}" PARENT_SCOPE)
endfunction()
