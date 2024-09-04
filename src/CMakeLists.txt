cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
project(waybarHomeassistant)
add_library(sensor sensor.c)
add_library(parser parser.c)
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.c)
include(FindPkgConfig)
pkg_check_modules(CURL libcurl REQUIRED)
include_directories(
  SYSTEM ${CURL_INCLUDE_DIRS}
)
target_link_libraries(${CMAKE_PROJECT_NAME}
  ${CURL_LIBRARIES}
  sensor
  parser
)

