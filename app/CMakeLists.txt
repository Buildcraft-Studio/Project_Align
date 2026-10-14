add_executable(project_align_app
    main.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/FTXUI ${CMAKE_CURRENT_BINARY_DIR}/ftxui)

target_link_libraries(project_align_app
    PRIVATE
        ProjectAlign::ProjectAlign
        ftxui::ftxui
)