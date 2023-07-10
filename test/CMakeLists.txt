cmake_minimum_required(VERSION 3.12)
project(mbd-json-testing    LANGUAGES C
                                        VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

###### LIB TEST ######
set(TargetName mbd-json-testing)

###### add 3rd Party Libs ######
# Coderitter embedded json lib
set(MBD_JSON_INSTALL_DIR "${PROJECT_SOURCE_DIR}/../install/")
set(MBD_JSON_ROOT_DIR "${PROJECT_SOURCE_DIR}/../3rdParty/mbd-json")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${MBD_JSON_INSTALL_DIR} ${MBD_JSON_ROOT_DIR})
find_package(mbd-json REQUIRED)
################################

###### add targets ######
INCLUDE_DIRECTORIES(
)

set(HEADERS
    )

set(SOURCES
        mbd_json.test.c
    )

add_executable(${TargetName} ${HEADERS} ${SOURCES})
target_link_libraries(${TargetName} Coderitter::mbd-json)
set_target_properties(${TargetName} PROPERTIES
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    
install(TARGETS ${TargetName} RUNTIME DESTINATION "bin")