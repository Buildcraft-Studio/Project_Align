#include "runtime/DebugAndLogRuntime.hpp"
#include <iostream>

namespace Engine::Runtime {

DebugAndLogRuntime::DebugAndLogRuntime(int16_t id)
    : id(id) {

}

int16_t DebugAndLogRuntime::getRuntimeID() const {
    return id;
}

void DebugAndLogRuntime::logAll(const RuntimeManger& manager) const {
    for (const auto& [runtimeID, info] : manager.getTable()) {
        std::cout << "[Runtime " << runtimeID << "] "
                  << info.Name
                  << " status=" << static_cast<int>(info.status)
                  << " sinceAlive=" << info.sinceAlive
                  << " failCount=" << static_cast<int>(info.failCount)
                  << "\n";
    }
}

void DebugAndLogRuntime::logFailures(const RuntimeManger& manager) const {
    for (const auto& [runtimeID, info] : manager.getTable()) {
        uint8_t failCount = manager.getFailCount(runtimeID);
        if (failCount > 0) {
            std::cout << "[WARN] Runtime " << runtimeID << " (" << info.Name << ") "
                      << "has " << static_cast<int>(failCount) << " consecutive WatchDog failure(s)"
                      << " - " << (failCount >= RuntimeManger::WATCHDOG_MAX_FAILURES
                                     ? "DEAD, will restart"
                                     : "STALED")
                      << "\n";
        }
    }
}

void DebugAndLogRuntime::setRuntimeStatus(RuntimeManger& manager, int16_t targetID, RuntimeManger::statusCode status) const {
    manager.setStatus(targetID, status);
}

}
