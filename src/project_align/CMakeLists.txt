add_library(ProjectAlign
    src/greeter.cpp
)

add_library(ProjectAlign::ProjectAlign ALIAS ProjectAlign)

target_include_directories(ProjectAlign
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(ProjectAlign PUBLIC cxx_std_17)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS ProjectAlign
    EXPORT ProjectAlignTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ProjectAlignTargets
    FILE ProjectAlignTargets.cmake
    NAMESPACE ProjectAlign::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ProjectAlign
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/ProjectAlignConfig.cmake.in
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
