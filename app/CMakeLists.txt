add_executable(project_align_app
    main.cpp
)

target_link_libraries(project_align_app
    PRIVATE
        ProjectAlign::ProjectAlign
)

install(TARGETS project_align_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
