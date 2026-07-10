find_package(PkgConfig QUIET)

if(PkgConfig_FOUND)
    pkg_check_modules(LUA QUIET lua5.4)
endif()

if(TARGET Project_Align_Rebuild)
    if(LUA_FOUND)
        target_include_directories(Project_Align_Rebuild PRIVATE ${LUA_INCLUDE_DIRS})
        target_link_libraries(Project_Align_Rebuild PRIVATE ${LUA_LIBRARIES})
    endif()
endif()
