/** *@file LuaRuntime.cpp
 *  @brief Implements LuaRuntime. Script execution is a stub pending an
 *         actual Lua interpreter being linked into the project.
 *  @ingroup RuntimeSystem
**/
#include "runtime/LuaRuntime.hpp"
#include <iostream>

namespace Engine::Runtime {

LuaRuntime::LuaRuntime(int16_t id)
    : id(id) {
    // NOTE: this used to say `Engine::RuntimeManger` here — that namespace
    // doesn't exist anywhere in the project (an IDE autocomplete slip).
    // Everything in this codebase lives in Engine::Runtime.
}

LuaRuntime::~LuaRuntime() {

}

int16_t LuaRuntime::getRuntimeID() const {
    return id;
}

bool LuaRuntime::loadScript(const std::string& path) {
    // TODO: replace with real Lua loading, e.g.:
    //   L = luaL_newstate();
    //   luaL_openlibs(L);
    //   if (luaL_dofile(L, path.c_str()) != LUA_OK) { ... }
    loadedPath = path;
    std::cout << "[LuaRuntime] (stub) would load script: " << path << "\n";
    return true;
}

void LuaRuntime::run() {
    // TODO: replace with real Lua call, e.g. lua_pcall(L, ...)
    std::cout << "[LuaRuntime] (stub) would run: " << loadedPath << "\n";
}

}
