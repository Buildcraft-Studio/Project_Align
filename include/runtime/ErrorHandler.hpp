/** *@file ErrorHandler.hpp
 *  @brief Declares ErrorHandler, a placeholder per-runtime error/warning sink.
 *  @ingroup RuntimeSystem
**/
#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <stdint.h>

namespace Engine::Runtime {

    /** *@class ErrorHandler
     *  @brief Placeholder error/warning handler for a single runtime.
     *  @details Empty for now — RuntimeManger stands one of these up per
     *           runtime ID in setRuntime(), and calls raiseWarning() from
     *           WatchDog() when a check fails. Fill in actual logging /
     *           alerting / escalation behavior here.
     *  @ingroup RuntimeSystem
    **/
    class ErrorHandler
    {
        public:
            /** *@brief Construct a handler for a given runtime.
             *  @param ownerID The runtime ID this handler will handle errors for.
            **/
            explicit ErrorHandler(int16_t ownerID);
            ~ErrorHandler();

            /** *@brief Called when the owning runtime fails a WatchDog check.
             *         Currently a no-op.
             *  @param reasonCode Numeric reason for the warning (e.g. a cast RuntimeManger::statusCode).
            **/
            void raiseWarning(int reasonCode);

            /** *@brief Return the number of warnings raised for this runtime.
             *  @return Warning count seen so far.
            **/
            uint32_t getWarningCount() const;

        private:
            int16_t ownerID; ///< The runtime ID this handler belongs to.
            uint32_t warningCount = 0; ///< Number of warnings emitted for this runtime.
    };
}
#endif // ERRORHANDLER_HPP
