/** *@file DebugAndLogRuntime.hpp
 *  @brief Declares DebugAndLogRuntime, the read/set entry point into the
 *         runtime table for humans and tools.
 *  @ingroup RuntimeSystem
**/
#ifndef DEBUGANDLOGRUNTIME_HPP
#define DEBUGANDLOGRUNTIME_HPP

#include "runtime/runtimeManger.hpp"

namespace Engine::Runtime {

    /** *@class DebugAndLogRuntime
     *  @brief A runtime module in its own right (implements IRuntimeModule,
     *         so it gets a WatchDog'd table entry like everything else), but
     *         it also doubles as the read/write entry point into the table
     *         for humans/tools: it can dump every runtime's status (read)
     *         and can flip a runtime's status flag by hand (set), e.g. to
     *         mark something INACTIVE or force it back to STARTED.
     *  @ingroup RuntimeSystem
    **/
    class DebugAndLogRuntime : public IRuntimeModule
    {
        public:
            /** *@brief Construct with this module's own runtime ID.
             *  @param id The runtime ID this module will be registered under.
            **/
            explicit DebugAndLogRuntime(int16_t id);

            /** *@brief IRuntimeModule contract - reports its own ID during WatchDog().
             *  @return This module's own runtime ID.
            **/
            int16_t getRuntimeID() const override;

            /** *@brief READ: log/print every runtime's current record.
             *         Pulls from RuntimeManger::getTable().
             *  @param manager The RuntimeManger instance to read the table from.
            **/
            void logAll(const RuntimeManger& manager) const;

            /** *@brief READ: report only runtimes currently carrying failures
             *         (failCount > 0), via RuntimeManger::getFailCount().
             *         Keeps failure reporting separate from the full dump.
             *  @param manager The RuntimeManger instance to read failure counts from.
            **/
            void logFailures(const RuntimeManger& manager) const;

            /** *@brief SET: manually override a runtime's status, e.g. to pull
             *         a misbehaving module out of rotation (INACTIVE) or
             *         re-arm it (STARTED). Goes through RuntimeManger::setStatus().
             *  @param manager  The RuntimeManger instance to write into.
             *  @param targetID The runtime ID whose status should be overwritten.
             *  @param status   The status to set.
            **/
            void setRuntimeStatus(RuntimeManger& manager, int16_t targetID, RuntimeManger::statusCode status) const;

        private:
            int16_t id; ///< This module's own runtime ID.
    };
}
#endif // DEBUGANDLOGRUNTIME_HPP
