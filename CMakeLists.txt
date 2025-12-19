# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(PROJECT_VER "6.0") 

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# --- Partition Table Selection Logic ---
set(_partition_file "partition_ota.csv")

if(DEFINED ENV{HW_HEADER})
    set(_header "$ENV{HW_HEADER}")
    if(NOT IS_ABSOLUTE "${_header}")
        set(_header "${CMAKE_SOURCE_DIR}/main/${_header}")
    endif()

    if(EXISTS "${_header}")
        file(READ "${_header}" _contents)
        if(_contents MATCHES "(^|[\\r\\n])[ \\t]*#define[ \\t]+HW_USE_FLASH_AS_SD([ \\t]+1)?")
            set(_partition_file "partition_storage.csv")
        endif()
    endif()
endif()

# Apply selection to sdkconfig
if(EXISTS "${CMAKE_SOURCE_DIR}/sdkconfig")
    file(READ "${CMAKE_SOURCE_DIR}/sdkconfig" _sdk_text)
    set(_sdk_new "${_sdk_text}")
    
    foreach(_key CONFIG_PARTITION_TABLE_CUSTOM_FILENAME CONFIG_PARTITION_TABLE_FILENAME)
        if(_sdk_new MATCHES "${_key}=")
            string(REGEX REPLACE "${_key}=\"[^\"]*\"" "${_key}=\"${_partition_file}\"" _sdk_new "${_sdk_new}")
        else()
            string(APPEND _sdk_new "\n${_key}=\"${_partition_file}\"")
        endif()
    endforeach()

    if(NOT _sdk_new STREQUAL _sdk_text)
        file(WRITE "${CMAKE_SOURCE_DIR}/sdkconfig" "${_sdk_new}")
    endif()
endif()
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
