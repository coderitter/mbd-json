cmake_minimum_required(VERSION 3.12)
project(mbd-json-testing VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

###### LIB TEST ######
set(TargetName mbd-json-testing)

###### add 3rd Party Libs ######
# Coderitter embedded json lib
add_subdirectory(${PROJECT_SOURCE_DIR}/../ [binary_dir])
if(NOT TARGET mbd-json)
    find_package(mbd-json REQUIRED)
else()
    add_library(mbd-json::mbd-json ALIAS mbd-json)
endif()
################################

###### add targets ######
set(HEADERS
    )

set(SOURCES
        mbd_json.test.c
    )

add_executable(${TargetName} ${HEADERS} ${SOURCES})
target_link_libraries(${TargetName} mbd-json::mbd-json)
set_target_properties(${TargetName} PROPERTIES
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TargetName})

install(TARGETS ${TargetName} RUNTIME DESTINATION "bin")