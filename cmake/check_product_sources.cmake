cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED MINIKV_SOURCE_ROOT)
    get_filename_component(MINIKV_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()
if(NOT DEFINED MINIKV_PRODUCT_MANIFEST)
    set(MINIKV_PRODUCT_MANIFEST "${MINIKV_SOURCE_ROOT}/config/product-sources.txt")
endif()
if(NOT DEFINED MINIKV_PRODUCT_ROOT)
    set(MINIKV_PRODUCT_ROOT "${MINIKV_SOURCE_ROOT}")
endif()

include("${MINIKV_SOURCE_ROOT}/cmake/MinikvSources.cmake")
minikv_read_product_sources(
    "${MINIKV_PRODUCT_MANIFEST}"
    "${MINIKV_PRODUCT_ROOT}"
    validated_sources
)
list(LENGTH validated_sources validated_count)
message(STATUS "validated product source manifest: ${validated_count} entries")
