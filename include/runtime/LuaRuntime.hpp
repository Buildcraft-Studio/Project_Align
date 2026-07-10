/** *@file LuaRuntime.hpp
 *  @brief Declares LuaRuntime, the runtime module responsible for loading
 *         and driving the embedded Lua scripting layer.
 *  @ingroup RuntimeSystem
**/
#ifndef LUARUNTIME_HPP
#define LUARUNTIME_HPP

#include "runtime/runtimeManger.hpp"
#include <string>

namespace Engine::Runtime {

    /** *@class LuaRuntime
     *  @brief The script loader for the game - loads and runs the embedded
     *         Lua language. Implements IRuntimeModule so it gets a
     *         WatchDog'd table entry like any other managed runtime;
     *         register it through Bootstrap (or a supervisor) the same way
     *         DebugAndLogRuntime/SupervisorRuntime are registered.
     *  @ingroup RuntimeSystem
    **/
    class LuaRuntime : public IRuntimeModule
    {
        public:
            /** *@brief Construct with this module's own runtime ID.
             *  @param id The runtime ID this module will be registered under.
            **/
            explicit LuaRuntime(int16_t id);
            virtual ~LuaRuntime();

            /** *@brief IRuntimeModule contract - reports its own ID during WatchDog().
             *  @return This module's own runtime ID.
            **/
            int16_t getRuntimeID() const override;

            /** *@brief Load a Lua script from disk, ready to be run.
             *  @param path Path to the .lua file (e.g. "game/gameLogic.lua").
             *  @return true if the script loaded successfully.
             *  @note STUB - no actual Lua interpreter (lua_State*, luaL_dofile,
             *        etc.) is wired in yet. This is the entry point where that
             *        integration belongs once a Lua library is linked in.
            **/
            bool loadScript(const std::string& path);

            /** *@brief Execute whatever script is currently loaded.
             *  @note STUB - see loadScript() note. Call incrementCounter()
             *        for this runtime's ID (via RuntimeManger) each time this
             *        runs, so WatchDog() can track that the script is still
             *        being driven.
            **/
            void run();

        private:
            int16_t     id;         ///< This module's own runtime ID.
            std::string loadedPath; ///< Path of the currently loaded script, empty if none.
            // TODO: lua_State* L; — the actual interpreter handle goes here
            //       once a Lua library (e.g. liblua, LuaJIT) is linked in.
    };
}
#endif // LUARUNTIME_HPP
