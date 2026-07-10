/** *@file Bootstrap.hpp
 *  @brief Declares Bootstrap, which loads and registers every startup-time
 *         runtime module before handing management off to RuntimeManger.
 *  @ingroup RuntimeSystem
**/
#ifndef BOOTSTRAP_HPP
#define BOOTSTRAP_HPP

#include "runtime/runtimeManger.hpp"
#include "runtime/DebugAndLogRuntime.hpp"
#include "runtime/SupervisorRuntime.hpp"
#include <memory>
#include <vector>

namespace Engine::Runtime {

    /** *@class Bootstrap
     *  @brief Loads and registers every runtime module BEFORE management is
     *         handed off to RuntimeManger. Per RuntimeManger's own doc note,
     *         RuntimeManger itself is assumed to be runtime ID 0; Bootstrap
     *         registers everything else on top of that.
     *  @details Each module Bootstrap registers gets createRuntime()'d
     *           (RuntimeManger::setRuntime()) with status STARTED (14) -
     *           loaded, but not yet WatchDog-checked. Bootstrap owns the
     *           module instances so their lifetime outlives the RuntimeManger
     *           that only holds raw pointers to them.
     *  @ingroup RuntimeSystem
    **/
    class Bootstrap
    {
        public:
            Bootstrap();

            /** *@brief Registers all known runtime modules with manager and
             *         marks them STARTED. Call this once at startup, before
             *         the main loop begins ticking/WatchDog'ing.
             *  @param manager The RuntimeManger instance to register modules with.
            **/
            void loadRuntimes(RuntimeManger& manager);

            /** *@brief Access to the supervisor Bootstrap registered, so other
             *         code (e.g. main.cpp) can ask it to spawn workers later.
             *  @return Non-owning pointer to the SupervisorRuntime, or nullptr if loadRuntimes() hasn't run yet.
            **/
            SupervisorRuntime* getSupervisor() const;

        private:
            /// Bootstrap owns these so RuntimeManger's raw IRuntimeModule*
            /// pointers stay valid for the life of the program.
            std::vector<std::unique_ptr<IRuntimeModule>> ownedModules;
            SupervisorRuntime* supervisor = nullptr; ///< Non-owning view into ownedModules, for getSupervisor().
    };
}
#endif // BOOTSTRAP_HPP
