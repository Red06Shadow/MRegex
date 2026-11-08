cmake_minimum_required(VERSION 3.14)
project(myregex VERSION 1.0.0 LANGUAGES CXX)

add_library(myregex INTERFACE)
add_library(myregex::myregex ALIAS myregex)  # namespace recomendado

target_include_directories(myregex INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(myregex INTERFACE cxx_std_17)

if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(test)
endif()

# --- Reglas de instalación ---
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS myregex EXPORT myregexTargets)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT myregexTargets
    FILE      myregexTargets.cmake
    NAMESPACE myregex::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/myregex
)

# Genera myregexConfigVersion.cmake (para control de versiones en find_package)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/myregexConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Genera myregexConfig.cmake
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/myregexConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/myregexConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/myregex
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/myregexConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/myregexConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/myregex
)