/** *@file runtimeManger.hpp
 *  @brief Declares IRuntimeModule and RuntimeManger, the core of the runtime
 *         watchdog/lifecycle system.
 *  @defgroup RuntimeSystem Runtime Management System
 *  @{
**/
#ifndef RUNTIMEMANGER_HPP
#define RUNTIMEMANGER_HPP

#include <string>
#include <stdint.h>
#include <unordered_map>
#include "runtime/ErrorHandler.hpp"

namespace Engine::Runtime {

    /** *@class IRuntimeModule
     *  @brief Contract every managed runtime module must implement so the
     *         RuntimeManger can ask it to report on itself during a WatchDog pass.
     *  @note  The module reports its OWN ID back — RuntimeManger never assumes it.
     *  @ingroup RuntimeSystem
    **/
    class IRuntimeModule
    {
        public:
            virtual ~IRuntimeModule() = default;

            /** *@brief Called by RuntimeManger::WatchDog(). The module returns its
             *         own runtime ID so RuntimeManger can look up and log its
             *         record in the status table.
             *  @return This module's own runtime ID.
            **/
            virtual int16_t getRuntimeID() const = 0;
    };

    /** *@class RuntimeManger
     *  @brief All runtime will be started and manged by the RuntimeManger Module.
     *  @details After the Bootstrap module load all the Runtime are loaded,
     *           mangagement is handed to RuntimeManger. Modules may also be
     *           registered later, at runtime, by a supervisor-style module
     *           (see SupervisorRuntime) rather than only at Bootstrap time.
     *  @note RuntimeManger itself is assumed to be runtime ID 0.
     *  @ingroup RuntimeSystem
    **/
    class RuntimeManger
    {
        public:

            /** *@brief Every status a runtime entry in the table can carry.
             *  @details OK/STALED/DEAD are computed automatically by
             *           WatchDog(). KILLED is a transient step DEAD passes
             *           through on its way back to STARTED. STARTED and
             *           INACTIVE (14/15) are manual flags — nothing derives
             *           them automatically; see setRuntime() and setStatus().
            **/
            enum class statusCode {
                OK          = 0,  ///< Runtime passed its WatchDog check within the OK tick window (0-100 ticks).
                DEAD        = 1,  ///< Runtime failed the WatchDog check 5 times in a row; it is killed and restarted.
                DEGRADED    = 2,  ///< Reserved: runtime performance starts to lag more than 100 ticks. Not yet driven by WatchDog().
                STALED      = 3,  ///< Runtime failed a single WatchDog check (elapsed ticks > 100). A warning fires (via ErrorHandler) and a restart is attempted if this repeats.
                KILLED      = 13, ///< Set when the RuntimeManger issues a kill order for the runtime (set right before a DEAD restart, or on manual kill).
                STARTED     = 14, ///< Manual flag. Set once by Bootstrap (or SupervisorRuntime) when a runtime module is first loaded, before RuntimeManger takes over management of it.
                INACTIVE    = 15  ///< Manual flag. When set, the runtime will be skipped when RuntimeManger initiates startup.
            };

            /// @brief Number of ticks a runtime may go without checking in before
            ///        it is considered STALED rather than OK.
            static constexpr int64_t WATCHDOG_TICK_LIMIT = 100;

            /// @brief Number of consecutive STALED results before a runtime is
            ///        considered DEAD and gets killed + restarted.
            static constexpr uint8_t WATCHDOG_MAX_FAILURES = 5;

            /** *@brief One logged row of the runtime table: everything
             *         RuntimeManger tracks about a single runtime ID.
            **/
            struct runtimeInfo {
                int16_t     ID;         ///< Defind the Runtime ID.
                std::string Name;       ///< Define the Name of the Runtime.
                uint64_t    isStarted;  ///< Is the counter that increment since the Runtime was callad, it unsigned.
                int64_t     sinceAlive; ///< Ticks elapsed since this runtime last passed a WatchDog check. Reset to 0 on OK and on restart.
                uint8_t     failCount;  ///< Consecutive failed WatchDog checks (STALED results in a row). Reset to 0 on OK and on restart. Reaching WATCHDOG_MAX_FAILURES triggers DEAD.
                statusCode  status;     ///< Last known status logged for this runtime ID.
            };

            RuntimeManger();

            /** *@brief Register a runtime module with the manager so it has a
             *         table entry to log status against. This is "createRuntime" -
             *         the function that adds an entry to the runtime table.
             *  @details This is what Bootstrap calls to hand a freshly-loaded
             *           module off to RuntimeManger at startup. It can also be
             *           called later, at runtime, by a supervisor/factory-style
             *           module (see SupervisorRuntime::createWorker()) to
             *           register a module it spawns on demand. The entry is
             *           created with status STARTED (14) — a manual flag
             *           meaning "loaded, but not yet WatchDog-checked" - and
             *           this is also the point an ErrorHandler is stood up
             *           for this runtime ID.
             *  @param id      The runtime ID Bootstrap/this module identifies itself with.
             *  @param setName Human-readable name for the table/logs.
             *  @param module  Pointer to the module RuntimeManger will poll during WatchDog(). Ownership is NOT taken - the caller must keep the pointed-to module alive for as long as it stays registered.
            **/
            void setRuntime(int16_t id, std::string setName, IRuntimeModule* module);

            /** *@brief Advance the activity counter for a runtime by one tick.
             *         Called each engine tick for a given runtime ID.
             *  @param id The runtime ID to advance. No-op if id isn't registered.
            **/
            void incrementCounter(int16_t id);

            /** *@brief Kill and restart a runtime, clearing its WatchDog counters.
             *  @details Sets status to KILLED, then STARTED, and zeroes
             *           sinceAlive and failCount so the runtime gets a clean
             *           slate for the next WatchDog sweep.
             *  @param id The runtime ID to restart. No-op if id isn't registered.
            **/
            void restartRuntime(int16_t id);

            /** *@brief Sweep the table: every registered module reports its own
             *         ID back, and its record is scored and logged:
             *         - sinceAlive in [0, WATCHDOG_TICK_LIMIT] -> OK, failCount reset to 0
             *         - sinceAlive  > WATCHDOG_TICK_LIMIT       -> STALED, failCount++, ErrorHandler::raiseWarning() fires
             *         - failCount reaches WATCHDOG_MAX_FAILURES -> DEAD, restartRuntime() is called automatically
            **/
            void WatchDog();

            /** *@brief Read-only lookup of a runtime's current logged record.
             *  @param id The runtime ID to look up.
             *  @return A const reference to that runtime's full table entry.
             *  @throws std::out_of_range if id isn't registered.
            **/
            const runtimeInfo& getStatus(int16_t id) const;

            /** *@brief Read-only lookup of just a runtime's consecutive-failure
             *         count. This is the entry point DebugAndLogRuntime uses
             *         to report failures specifically, without needing the
             *         full record.
             *  @param id The runtime ID to look up.
             *  @return The number of consecutive failed WatchDog checks for that runtime.
             *  @throws std::out_of_range if id isn't registered.
            **/
            uint8_t getFailCount(int16_t id) const;

            /** *@brief Manually set a runtime's status, e.g. INACTIVE (15) to
             *         keep it from starting, or STARTED (14) to re-arm it.
             *  @details This is the entry point a DebugAndLog-style runtime
             *           (or any external caller) uses to WRITE into the
             *           table, mirroring getStatus()/getTable() for reads.
             *  @param id     The runtime ID to update. No-op if id isn't registered.
             *  @param status The status to set.
             *  @note This does not run any restart/kill logic - it only
             *        overwrites the logged flag. Use restartRuntime() if you
             *        need the counters cleared too.
            **/
            void setStatus(int16_t id, statusCode status);

            /** *@brief Read-only view of the entire table, keyed by runtime ID.
             *         This is the entry point a DebugAndLog-style runtime uses
             *         to dump/inspect every runtime's status at once.
             *  @return A const reference to the full ID -> runtimeInfo table.
            **/
            const std::unordered_map<int16_t, runtimeInfo>& getTable() const;

        protected:

        private:
            std::unordered_map<int16_t, runtimeInfo>     table;         ///< ID -> logged status record.
            std::unordered_map<int16_t, IRuntimeModule*>  modules;       ///< ID -> module to poll during WatchDog.
            std::unordered_map<int16_t, ErrorHandler>     errorHandlers; ///< ID -> this runtime's error handler.
    };
}
/** @} */ // end of RuntimeSystem group
#endif // RUNTIMEMANGER_HPP
