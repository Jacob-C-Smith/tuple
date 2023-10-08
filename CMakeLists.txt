# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# This is the name of the repository
project ("tuple")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set for debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[tuple] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Add the tuple project
if ( NOT "${HAS_TUPLE}")

    # Log
    message("[tuple] Adding tuple project")

    # Let any parent project know that it has the tuple project
    set(HAS_TUPLE true PARENT_SCOPE)
endif()

# Add source to this project's executable.
add_executable (tuple_example "main.c" "tuple.c")
target_include_directories(tuple_example PUBLIC include include/tuple)

# Add source to the tester
add_executable (tuple_test "tuple_test.c" "tuple.c")
add_dependencies(tuple_test sync)
target_include_directories(tuple_test PUBLIC include ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(tuple_test sync)

# Add source to this project's library
add_library (tuple SHARED "tuple.c")
target_include_directories(tuple PUBLIC include include/tuple )
