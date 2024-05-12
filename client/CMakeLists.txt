cmake_minimum_required(VERSION 3.14)

set(CMAKE_AUTORCC ON)

if (WIN32)
    project(MessengerAppClient LANGUAGES CXX)
elseif(UNIX)
    project(MessengerAppClient)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#======================= QT INCLUSION ==============================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Network REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Core Network Xml REQUIRED) 

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/client/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/client/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/client/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${FORMS_DIR}/img/*.png"
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.h"
)

FILE(GLOB_RECURSE headers "src/*.h")
FILE(GLOB_RECURSE sources "src/*.cpp")
FILE(GLOB_RECURSE resources "forms/*.png")

#=================== SETUP EXECTUABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

add_subdirectory(../external/sqlite ${CMAKE_CURRENT_BINARY_DIR}/sqlite)

# Add the executable
if (WIN32) 
    add_executable(MessengerAppClient ${SOURCES})
elseif(UNIX)
    add_executable(MessengerAppClient ${SOURCES})
endif()

# Add the target includes for MessengerAppClient 
target_include_directories(MessengerAppClient PRIVATE ${FORMS_DIR})
target_include_directories(MessengerAppClient PRIVATE ${INCLUDE_DIR})
target_include_directories(MessengerAppClient PRIVATE ${SOURCE_DIR})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${headers} ${resources})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(MessengerAppClient Qt6::Widgets)
target_link_libraries(MessengerAppClient Qt6::Network)

target_link_libraries(MessengerAppClient
  SQLiteCpp
  sqlite3
  spdlog
  nlohmann_json::nlohmann_json
  )
