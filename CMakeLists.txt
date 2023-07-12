cmake_minimum_required(VERSION 3.12)

project(mbd-json VERSION 1.0 LANGUAGES C)
set(ProjectTargets)
set(TargetsPath)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

#### Load package config helpers ####
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#### Add targets ####
set(TargetName ${PROJECT_NAME})
set(ProjectTargets ${ProjectTargets} ${TargetName})
set(TargetsPath ${TargetsPath} ${CMAKE_CURRENT_LIST_DIR}/include/)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/include/mbd_json/mbd_json.h
    )

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/mbd_json.c
    )

add_library(${TargetName} ${HEADERS} ${SOURCES})

set_target_properties(${TargetName} PROPERTIES
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    
target_include_directories(
    ${TargetName} 
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    )

### Setup package config ####
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(namespace "${PROJECT_NAME}::${TargetName}")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
install(
    TARGETS ${ProjectTargets}
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Headers:
install(
    DIRECTORY ${TargetsPath}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
)

# Config
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)