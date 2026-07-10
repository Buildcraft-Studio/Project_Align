/** *@file ErrorHandler.cpp
 *  @brief Implements ErrorHandler. Intentionally empty stub - see class docs.
 *  @ingroup RuntimeSystem
**/
#include "runtime/ErrorHandler.hpp"

namespace Engine::Runtime {

ErrorHandler::ErrorHandler(int16_t ownerID)
    : ownerID(ownerID) {

}

ErrorHandler::~ErrorHandler() {

}

void ErrorHandler::raiseWarning(int reasonCode) {
    // TODO: no behavior yet - this is an intentionally empty stub.
    (void)reasonCode;
}

}
