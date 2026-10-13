add_library(ProjectAlign
    src/greeter.cpp
)

add_library(ProjectAlign::ProjectAlign ALIAS ProjectAlign)

target_include_directories(ProjectAlign
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ProjectAlign PUBLIC cxx_std_17)
