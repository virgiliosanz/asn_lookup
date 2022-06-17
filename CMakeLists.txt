##########################################################################################
## Configuration
##########################################################################################
cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME ASN_Tools)
set(PROJECT_BRIEF "Code for getting traffic from logs grouped by asn")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION_STRING} LANGUAGES C)

set(CMAKE_BUILD_TYPE release)
set(CMAKE_VERBOSE_MAKEFILE 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/misc/cmake/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()


# Build Doxygen documentation
option(BUILD_DOC "Build documentation" ON)

##########################################################################################
## Script starts
##########################################################################################
enable_testing()
if (NOT CMAKE_BUILD_TYPE MATCHES release)
    set(CMAKE_BUILD_TYPE debug)
endif()
message(CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE})

add_definitions(-Wall -Wextra -Werror -std=gnu99 -march=native)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

# Project libs
set(PROJECT_LIBS asn)

# Add Include directories
include_directories(src/lib src/tests)
#configure_file(${PROJECT_SOURCE_DIR}/src/lib/config.h.in ${PROJECT_SOURCE_DIR}/src/lib/config.h)

# Symlink misc to build, so tests find the files needed
#execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/misc
#                                                           ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/misc)

# External libs
#find_package(csv REQUIRED)

#SET(THIRDPARTY_LIBRARIES "${CSV_LIB}")


# Source code dirs
#add_subdirectory(vendor)
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")


add_subdirectory(src)
add_subdirectory(docs)
