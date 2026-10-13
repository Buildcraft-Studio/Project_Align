add_executable(project_align_tests
    test_greeter.cpp
)

target_link_libraries(project_align_tests
    PRIVATE
        ProjectAlign::ProjectAlign
)

add_test(NAME project_align_greeter_test COMMAND project_align_tests)
