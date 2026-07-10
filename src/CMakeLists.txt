file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(Project_Align_Rebuild
    ${PROJECT_SOURCES}
)

set_target_properties(Project_Align_Rebuild PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Release"
)

target_include_directories(Project_Align_Rebuild
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Project_Align_Rebuild PRIVATE -Wall -Wextra -Wpedantic)
endif()
