/** *@file ErrorHandler.cpp
 *  @brief Implements ErrorHandler. Intentionally empty stub - see class docs.
 *  @ingroup RuntimeSystem
**/
#include "runtime/ErrorHandler.hpp"
#include <iostream>

namespace Engine::Runtime {

ErrorHandler::ErrorHandler(int16_t ownerID)
    : ownerID(ownerID), warningCount(0) {

}

ErrorHandler::~ErrorHandler() {

}

void ErrorHandler::raiseWarning(int reasonCode) {
    ++warningCount;
    std::cerr << "[WARN] Runtime " << ownerID << " raised warning code "
              << reasonCode << "\n";
}

uint32_t ErrorHandler::getWarningCount() const {
    return warningCount;
}

}
