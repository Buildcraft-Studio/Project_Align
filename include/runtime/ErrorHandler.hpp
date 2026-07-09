#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <stdint.h>

namespace Engine::Runtime {

    /** *@brief Placeholder error/warning handler for a single runtime.
     *  @details Empty for now — RuntimeManger stands one of these up per
     *           runtime ID in setRuntime(), and calls raiseWarning() from
     *           WatchDog() when a check fails. Fill in actual logging /
     *           alerting / escalation behavior here.
    **/
    class ErrorHandler
    {
        public:
            /// @brief Constructed with the runtime ID it handles errors for.
            explicit ErrorHandler(int16_t ownerID);
            ~ErrorHandler();

            /// @brief Called when the owning runtime fails a WatchDog check.
            ///        Currently a no-op.
            void raiseWarning(int reasonCode);

        private:
            int16_t ownerID; ///< @brief The runtime ID this handler belongs to.
    };
}
#endif // ERRORHANDLER_HPP
