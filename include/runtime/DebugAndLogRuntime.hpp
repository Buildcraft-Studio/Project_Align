#ifndef DEBUGANDLOGRUNTIME_HPP
#define DEBUGANDLOGRUNTIME_HPP

#include "runtime/runtimeManger.hpp"

namespace Engine::Runtime {

    /** *@brief A runtime module in its own right (implements IRuntimeModule,
     *         so it gets a WatchDog'd table entry like everything else), but
     *         it also doubles as the read/write entry point into the table
     *         for humans/tools: it can dump every runtime's status (read)
     *         and can flip a runtime's status flag by hand (set), e.g. to
     *         mark something INACTIVE or force it back to STARTED.
    **/
    class DebugAndLogRuntime : public IRuntimeModule
    {
        public:
            explicit DebugAndLogRuntime(int16_t id);

            /// @brief IRuntimeModule contract - reports its own ID during WatchDog().
            int16_t getRuntimeID() const override;

            /// @brief READ: log/print every runtime's current record.
            ///        Pulls from RuntimeManger::getTable().
            void logAll(const RuntimeManger& manager) const;

            /// @brief READ: report only runtimes currently carrying failures
            ///        (failCount > 0), via RuntimeManger::getFailCount().
            ///        Keeps failure reporting separate from the full dump.
            void logFailures(const RuntimeManger& manager) const;

            /// @brief SET: manually override a runtime's status, e.g. to pull
            ///        a misbehaving module out of rotation (INACTIVE) or
            ///        re-arm it (STARTED). Goes through RuntimeManger::setStatus().
            void setRuntimeStatus(RuntimeManger& manager, int16_t targetID, RuntimeManger::statusCode status) const;

        private:
            int16_t id; ///< @brief This module's own runtime ID.
    };
}
#endif // DEBUGANDLOGRUNTIME_HPP
