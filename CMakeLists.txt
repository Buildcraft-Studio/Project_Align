cmake_minimum_required(VERSION 3.16)

project(ProjectAlign VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CTest)

option(BUILD_TESTING "Build the test suite" ON)

add_subdirectory(src/project_align)
add_subdirectory(app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectAlignConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ProjectAlignConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ProjectAlign
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ProjectAlignConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ProjectAlignConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ProjectAlignConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ProjectAlign
)
