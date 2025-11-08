# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(PROJECT_VER "6.0") 

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Check if HW_USE_FLASH_AS_SD in HW_HEADER
set(_macro_found OFF)
file(READ "${CMAKE_SOURCE_DIR}/main/hwconf/*.h" _header)
string(REGEX MATCH "#define HW_USE_FLASH_AS_SD [0-9]+" _match "${_header}")
if(_match)
    set(_macro_found ON)
    message(STATUS "...")
    message(STATUS ${_match})
    message(STATUS "...")
    break()
endif()

# if(DEFINED ENV{HW_HEADER} AND NOT "$ENV{HW_HEADER}" STREQUAL "")
#     list(APPEND _hw_header_search_order
#         "$ENV{HW_HEADER}"
#         "${CMAKE_SOURCE_DIR}/main/hwconf/$ENV{HW_HEADER}"
#         "${CMAKE_SOURCE_DIR}/main/hwconf/devboards/$ENV{HW_HEADER}"
#         "${CMAKE_SOURCE_DIR}/main/hwconf/trampa/$ENV{HW_HEADER}"
#         "${CMAKE_SOURCE_DIR}/main/hwconf/vesc/$ENV{HW_HEADER}"
#     )
# endif()

# # Collect every hwconf header as a fallback
# file(GLOB_RECURSE _all_hw_headers "${CMAKE_SOURCE_DIR}/main/hwconf/*.h")
# list(APPEND _hw_header_search_order ${_all_hw_headers})

# foreach(_cand IN LISTS _hw_header_search_order)
#     if(EXISTS "${_cand}")
#         set(_hw_header "${_cand}")
#         break()
#     endif()
# endforeach()

# if(NOT _hw_header)
#     message(WARNING "Could not resolve HW_HEADER; using partition_ota.csv")
# endif()

# # ---------------------------------------------------------------------------
# # Decide whether internal flash is used as SD
# # ---------------------------------------------------------------------------
# set(_use_intflash_as_sd OFF)

# if(_hw_header)
#     file(READ "${_hw_header}" _hw_header_contents)
#     if(_hw_header_contents MATCHES "(^|[ \t\r\n])#\\s*define\\s+HW_USE_FLASH_AS_SD(\\s+1\\b|\\b)"
#        OR _hw_header_contents MATCHES "(^|[ \t\r\n])#\\s*define\\s+HW_INTFLASH_AS_SD(\\s+1\\b|\\b)"
#        OR _hw_header_contents MATCHES "(^|[ \t\r\n])#\\s*define\\s+HW_USE_INT_FLASH_AS_SD(\\s+1\\b|\\b)")
#         set(_use_intflash_as_sd ON)
#         message(STATUS "Detected internal-flash-as-SD macro in: ${_hw_header}")
#     endif()
# endif()

# # Manual override hook (optional)
# if(DEFINED ENV{FORCE_INTFLASH_AS_SD})
#     string(TOLOWER "$ENV{FORCE_INTFLASH_AS_SD}" _force)
#     if(_force STREQUAL "1" OR _force STREQUAL "true" OR _force STREQUAL "on")
#         set(_use_intflash_as_sd ON)
#         message(STATUS "FORCE_INTFLASH_AS_SD override -> ON")
#     elseif(_force STREQUAL "0" OR _force STREQUAL "false" OR _force STREQUAL "off")
#         set(_use_intflash_as_sd OFF)
#         message(STATUS "FORCE_INTFLASH_AS_SD override -> OFF")
#     endif()
# endif()

# # ---------------------------------------------------------------------------
# # Write the sdkconfig defaults that ESP-IDF actually consumes
# # ---------------------------------------------------------------------------
# if(_use_intflash_as_sd)
#     message(STATUS "Partition table: partition_storage.csv (internal flash)")
#     file(READ "${CMAKE_SOURCE_DIR}/sdkconfig.defaults.storage" _defaults_text)
# else()
#     message(STATUS "Partition table: partition_ota.csv (default)")
#     file(READ "${CMAKE_SOURCE_DIR}/sdkconfig.defaults.ota" _defaults_text)
# endif()

# file(WRITE "${CMAKE_SOURCE_DIR}/sdkconfig.defaults" "${_defaults_text}")

get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include(cmake/git_rev_parse.cmake)

git_describe(GIT_COMMIT_HASH ".")
# get_git_head_revision(GIT_COMMIT_REF GIT_COMMIT_HASH ".")

if(DEFINED ENV{GIT_COMMIT_HASH})
    set(GIT_COMMIT_HASH $ENV{GIT_COMMIT_HASH})
else()
    git_describe(GIT_COMMIT_HASH ".")
endif()

if(DEFINED ENV{GIT_BRANCH_NAME})
    set(GIT_BRANCH_NAME $ENV{GIT_BRANCH_NAME})
else()
    git_rev_parse(GIT_BRANCH_NAME "." "--abbrev-ref" "HEAD")
endif()

idf_build_set_property(COMPILE_DEFINITIONS "GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"" APPEND)
idf_build_set_property(COMPILE_DEFINITIONS "GIT_BRANCH_NAME=\"${GIT_BRANCH_NAME}\"" APPEND)

if(DEFINED ENV{USER_GIT_COMMIT_HASH})
    idf_build_set_property(COMPILE_DEFINITIONS "USER_GIT_COMMIT_HASH=\"$ENV{USER_GIT_COMMIT_HASH}\"" APPEND)
endif()
if(DEFINED ENV{USER_GIT_BRANCH_NAME})
    idf_build_set_property(COMPILE_DEFINITIONS "USER_GIT_BRANCH_NAME=\"$ENV{USER_GIT_BRANCH_NAME}\"" APPEND)
endif()

idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)
