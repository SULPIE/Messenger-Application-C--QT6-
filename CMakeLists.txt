cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(MessengerApp LANGUAGES CXX)
elseif(UNIX)
    project(MessengerApp)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/client)
add_subdirectory(${CMAKE_SOURCE_DIR}/server)

include_directories( ${CMAKE_SOURCE_DIR})