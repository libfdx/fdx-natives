cmake_minimum_required(VERSION 3.22)
project(fdx_natives_smoke LANGUAGES C CXX)

if(NOT DEFINED FDX_NATIVES_PACKAGE_DIR)
    message(FATAL_ERROR "FDX_NATIVES_PACKAGE_DIR is required")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${FDX_NATIVES_PACKAGE_DIR}/cmake/fdx-natives-targets.cmake")

add_executable(fdx_natives_smoke smoke.cpp)
target_link_libraries(fdx_natives_smoke PRIVATE
    fdx_natives::freetype
    fdx_natives::tint
    fdx_natives::tint_api_helpers
    fdx_natives::tint_lang_core_ir_transform
    fdx_natives::tint_lang_glsl_writer_helpers)
