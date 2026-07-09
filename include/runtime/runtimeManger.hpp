#ifndef RUNTIMEMANGER_HPP
#define RUNTIMEMANGER_HPP

#include <string>
#include <stdint.h>
#include <unordered_map>
#include "runtime/ErrorHandler.hpp"

namespace Engine::Runtime {

    /** *@brief Contract every managed runtime module must implement so the
     *         RuntimeManger can ask it to report on itself during a WatchDog pass.
     *  @note  The module reports its OWN ID back — RuntimeManger never assumes it.
    **/
    class IRuntimeModule
    {
        public:
            virtual ~IRuntimeModule() = default;

            /// @brief Called by RuntimeManger::WatchDog(). The module returns its
            ///        own runtime ID so RuntimeManger can look up and log its
            ///        record in the status table.
            virtual int16_t getRuntimeID() const = 0;
    };

/** *@brief All runtime will be started and manged by the RuntimeManger Module

    *@details After the Bootstrap module load all the Runtime are loaded, mangagement is handed to RuntimeManger
    *@note RuntimeManger itself is assume to be runtime ID 0 a
**/
    class RuntimeManger
    {
        public:

            enum class statusCode {
                OK          = 0,  ///< @brief Runtime passed its WatchDog check within the OK tick window (0-100 ticks).
                DEAD        = 1,  ///< @brief Runtime failed the WatchDog check 5 times in a row; it is killed and restarted.
                DEGRADED    = 2,  ///< @brief Reserved: runtime performance starts to lag more than 100 ticks. Not yet driven by WatchDog().
                STALED      = 3,  ///< @brief Runtime failed a single WatchDog check (elapsed ticks > 100). A warning should fire (error handling TODO) and a restart will be attempted if this repeats.
                KILLED      = 13, ///< @brief Set when the RuntimeManger issues a kill order for the runtime (set right before a DEAD restart, or on manual kill).
                STARTED     = 14, ///< @brief Manual flag. Set once by Bootstrap when a runtime module is first loaded, before RuntimeManger takes over management of it.
                INACTIVE    = 15  ///< @brief Manual flag. When set, the runtime will be skipped when RuntimeManger initiates startup.
            };

            /// @brief Number of ticks a runtime may go without checking in before
            ///        it is considered STALED rather than OK.
            static constexpr int64_t WATCHDOG_TICK_LIMIT = 100;

            /// @brief Number of consecutive STALED results before a runtime is
            ///        considered DEAD and gets killed + restarted.
            static constexpr uint8_t WATCHDOG_MAX_FAILURES = 5;

            struct runtimeInfo {
                int16_t    ID;          /// @brief Defind the Runtime ID
                std::string Name;       /// @brief Define the Name of the Runtime.
                uint64_t   isStarted;   /// @brief Is the counter that increment since the Runtime was callad, it unsigned.
                int64_t    sinceAlive;  /// @brief Ticks elapsed since this runtime last passed a WatchDog check. Reset to 0 on OK and on restart.
                uint8_t    failCount;   /// @brief Consecutive failed WatchDog checks (STALED results in a row). Reset to 0 on OK and on restart. Reaching WATCHDOG_MAX_FAILURES triggers DEAD.
                statusCode status;      /// @brief Last known status logged for this runtime ID.
            };

            RuntimeManger();

            /** *@brief Register a runtime module with the manager so it has a
             *         table entry to log status against.
             *  @details This is what Bootstrap calls to hand a freshly-loaded
             *           module off to RuntimeManger. The entry is created with
             *           status STARTED (14) — a manual flag meaning "loaded,
             *           but not yet WatchDog-checked". This is also the natural
             *           point to stand up this runtime's ErrorHandler, since
             *           error handling should exist before WatchDog() can ever
             *           find a fault to report on this ID.
             *  @param id     The runtime ID Bootstrap/this module identifies itself with.
             *  @param setName Human-readable name for the table/logs.
             *  @param module Pointer to the module RuntimeManger will poll during WatchDog().
             *  @note ErrorHandler wiring is a TODO — left as a hook below since
             *        ErrorHandler.hpp's actual interface isn't available here yet.
            **/
            void setRuntime(int16_t id, std::string setName, IRuntimeModule* module);

            /// @brief Advance the activity counter for a runtime by one tick.
            ///        Called each engine tick for a given runtime ID.
            void incrementCounter(int16_t id);

            /** *@brief Kill and restart a runtime, clearing its WatchDog counters.
             *  @details Sets status to KILLED, then STARTED, and zeroes
             *           sinceAlive and failCount so the runtime gets a clean
             *           slate for the next WatchDog sweep.
            **/
            void restartRuntime(int16_t id);

            /** *@brief Sweep the table: every registered module reports its own
             *         ID back, and its record is scored and logged:
             *         - sinceAlive in [0, WATCHDOG_TICK_LIMIT]      -> OK, failCount reset to 0
             *         - sinceAlive  > WATCHDOG_TICK_LIMIT            -> STALED, failCount++, warning (TODO: hook into ErrorHandler)
             *         - failCount reaches WATCHDOG_MAX_FAILURES      -> DEAD, restartRuntime() is called automatically
            **/
            void WatchDog();

            /// @brief Read-only lookup of a runtime's current logged record.
            const runtimeInfo& getStatus(int16_t id) const;

            /// @brief Read-only lookup of just a runtime's consecutive-failure
            ///        count. This is the entry point DebugAndLogRuntime uses
            ///        to report failures specifically, without needing the
            ///        full record.
            uint8_t getFailCount(int16_t id) const;

            /** *@brief Manually set a runtime's status, e.g. INACTIVE (15) to
             *         keep it from starting, or STARTED (14) to re-arm it.
             *  @details This is the entry point a DebugAndLog-style runtime
             *           (or any external caller) uses to WRITE into the
             *           table, mirroring getStatus()/getTable() for reads.
             *  @note This does not run any restart/kill logic - it only
             *        overwrites the logged flag. Use restartRuntime() if you
             *        need the counters cleared too.
            **/
            void setStatus(int16_t id, statusCode status);

            /// @brief Read-only view of the entire table, keyed by runtime ID.
            ///        This is the entry point a DebugAndLog-style runtime uses
            ///        to dump/inspect every runtime's status at once.
            const std::unordered_map<int16_t, runtimeInfo>& getTable() const;

        protected:

        private:
            std::unordered_map<int16_t, runtimeInfo>     table;         ///< @brief ID -> logged status record
            std::unordered_map<int16_t, IRuntimeModule*>  modules;       ///< @brief ID -> module to poll during WatchDog
            std::unordered_map<int16_t, ErrorHandler>     errorHandlers; ///< @brief ID -> this runtime's error handler
    };
}
#endif // RUNTIMEMANGER_HPP
