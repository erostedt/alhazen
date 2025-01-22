cmake_minimum_required(VERSION 3.25)

project(alhazen)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS
    "-Wall -pedantic -Wextra -Werror -Wshadow -Wsign-conversion -Wconversion -Wunreachable-code"
)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/alhazen)
set(EXAMPLES_PATH ${PROJECT_SOURCE_DIR}/examples)

include_directories(${SRC_DIR})

file(GLOB SRC_SOURCES ${SRC_DIR}/*.cpp)
file(GLOB EXAMPLES ${EXAMPLES_PATH}/*)

add_library(${PROJECT_NAME} ${SRC_SOURCES})

foreach(EXAMPLE ${EXAMPLES})
  if(NOT IS_DIRECTORY ${EXAMPLE})
    continue()
  endif()
  get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
  message("Building: " ${EXAMPLE_NAME})
  file(GLOB EXAMPLE_SOURCES ${EXAMPLE}/*.cpp)
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
  target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME})
endforeach()
