﻿# Need CMake 3.16
cmake_minimum_required (VERSION 3.16.0)

# This is the name of the repository
project ("tuple")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -pthread -lpthread -Wlogical-not-parentheses -Wnull-dereference)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Build sync with timer
add_compile_definitions(BUILD_SYNC_WITH_TIMER)

# Build sync with mutex
add_compile_definitions(BUILD_SYNC_WITH_MUTEX)

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()


# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[tuple] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the log module
if ( NOT "${HAS_LOG}")

    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the tuple project
if ( NOT "${HAS_TUPLE}")

    # Let any parent project know that it has the tuple project
    set(HAS_TUPLE true PARENT_SCOPE)

    # Set the tuple include
    set (TUPLE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the socket project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_TUPLE true PARENT_SCOPE)
        set(TUPLE_INCLUDE_DIR ${TUPLE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add source to this project's executable.
add_executable (tuple_example "main.c")
add_dependencies(tuple_example tuple log sync)
target_include_directories(tuple_example PUBLIC ${TUPLE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(tuple_example tuple log sync)

# Add source to the tester
add_executable (tuple_test "tuple_test.c")
add_dependencies(tuple_test tuple sync log)
target_include_directories(tuple_test PUBLIC include ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(tuple_test tuple sync log)

# Add source to this project's library
add_library (tuple SHARED "tuple.c")
add_dependencies(tuple sync log)
target_include_directories(tuple PUBLIC ${TUPLE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(tuple sync log)
