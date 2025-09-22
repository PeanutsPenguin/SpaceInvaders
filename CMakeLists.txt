
cmake_minimum_required(VERSION 3.22.1)

project(directories)

# Define variables for source and include directories
set(SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()



# Add the include directory to the search path for headers
include_directories(${INCLUDE_DIR})

file(
    #GLOB_RECURSE: This specifies the mode in which the file command should operate. GLOB_RECURSE is used 
    #to search for files recursively in directories that match the provided patterns.
    # It will look for files in subdirectories as well.

    # PROJECT_FILES is the name of the variable where the list of files matching the specified patterns 
    #will be stored.
    GLOB_RECURSE PROJECT_FILES
    ${INCLUDE_DIR}/*.h
    ${SOURCE_DIR}/*.c
    )


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE TARGET_ASSETS_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
list(FILTER TARGET_ASSETS_FILES EXCLUDE REGEX ${CMAKE_CURRENT_BINARY_DIR})

add_executable(SpaceInvaders)


#PRIVATE keyword indicates that these source files are private to the exe target,
# meaning they won't be visible to other targets that depend on exe.
target_sources(SpaceInvaders PRIVATE ${PROJECT_FILES})

target_include_directories(SpaceInvaders PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(SpaceInvaders PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/include) #(#include <raylib/raudio.h> and not #include <raudio.h>)

target_link_libraries(SpaceInvaders raylib)
