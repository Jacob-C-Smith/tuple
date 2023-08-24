# TUPLE CMake configuration file:
# This file is meant to be placed in a cmake subfolder of TUPLE-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(TUPLE PROPERTIES
    URL "https://www.g10.app/ADT/tuple"
    DESCRIPTION "Tuple ADT"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(TUPLE_FOUND TRUE)

# For compatibility with autotools TUPLE-config.cmake, provide TUPLE_* variables.

set_and_check(TUPLE_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(TUPLE_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(TUPLE_INCLUDE_DIR  "${TUPLE_PREFIX}/include")
set(TUPLE_INCLUDE_DIRS           "${TUPLE_INCLUDE_DIR}")
set_and_check(TUPLE_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(TUPLE_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(TUPLE_LIBRARIES TUPLE::TUPLE)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated TUPLE-target.cmake files.

set(_TUPLE_library     "${TUPLE_LIBDIR}/tuple.lib")
set(_TUPLE_dll_library "${TUPLE_BINDIR}/tuple.dll")
if(EXISTS "${_TUPLE_library}" AND EXISTS "${_TUPLE_dll_library}")
    if(NOT TARGET TUPLE::TUPLE)
        add_library(TUPLE::TUPLE SHARED IMPORTED)
        set_target_properties(TUPLE::TUPLE
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${TUPLE_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_TUPLE_library}"
                IMPORTED_LOCATION "${_TUPLE_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "TUPLE_SHARED"
                INTERFACE_TUPLE_SHARED "ON"
        )
    endif()
    set(TUPLE_TUPLE_FOUND TRUE)
else()
    set(TUPLE_TUPLE_FOUND FALSE)
endif()
unset(_TUPLE_library)
unset(_TUPLE_dll_library)

check_required_components(TUPLE)
