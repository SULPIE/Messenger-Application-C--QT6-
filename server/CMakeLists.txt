cmake_minimum_required(VERSION 3.14)

project(MessengerAppServer)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")

#======================= QT INCLUSION ==============================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_CXX_STANDARD 20)
find_package(Qt6Core REQUIRED)
find_package(Qt6Network REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Core Network Xml REQUIRED) 

#=================== INCLUSION OF Project Files ====================#
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/server/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/server/src")

include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${INCLUDE_DIR}/*.hxx"
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.cxx"
    "*.json"
)

#=================== SETUP EXECTUABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
add_executable(MessengerAppServer ${SOURCES})

# Add the target includes for MessengerAppServer 
target_include_directories(MessengerAppServer PRIVATE ${INCLUDE_DIR})
target_include_directories(MessengerAppServer PRIVATE ${SOURCE_DIR})

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
add_subdirectory(../external/pqxx ${CMAKE_CURRENT_BINARY_DIR}/pqxx)
add_subdirectory(../external/nlohmann_json ${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json)

#===================== LINKING LIBRARIES =======================#
target_link_libraries(MessengerAppServer Qt6::Network spdlog pqxx nlohmann_json::nlohmann_json)


