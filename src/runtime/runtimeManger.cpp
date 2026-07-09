#include "runtime/runtimeManger.hpp"
#include <stdexcept>

namespace Engine::Runtime {

RuntimeManger::RuntimeManger() {

}

void RuntimeManger::setRuntime(int16_t id, std::string setName, IRuntimeModule* module) {
    runtimeInfo info;
    info.ID         = id;
    info.Name       = setName;
    info.isStarted  = 0;
    info.sinceAlive = 0;
    info.failCount  = 0;
    info.status     = statusCode::STARTED; // manual flag: loaded by Bootstrap, not yet WatchDog-checked

    table[id]   = info;
    modules[id] = module;
    errorHandlers.emplace(id, ErrorHandler(id));
}

void RuntimeManger::incrementCounter(int16_t id) {
    auto it = table.find(id);
    if (it != table.end()) {
        it->second.isStarted++;
        it->second.sinceAlive++; // ticks since the last WatchDog pass scored this runtime
    }
}

void RuntimeManger::restartRuntime(int16_t id) {
    auto it = table.find(id);
    if (it == table.end()) {
        return;
    }

    // Kill first, then bring back up clean.
    it->second.status     = statusCode::KILLED;
    it->second.sinceAlive = 0;
    it->second.failCount  = 0;
    it->second.status     = statusCode::STARTED;
}

void RuntimeManger::WatchDog() {
    // Each module reports its own id back; we only trust that id as a lookup
    // key into our own table, we never take the module's word for its status
    // without checking it against this record.
    for (auto& [id, module] : modules) {
        if (module == nullptr) {
            continue;
        }

        int16_t reportedID = module->getRuntimeID();

        auto it = table.find(id);
        if (it == table.end()) {
            continue; // no record to log against, skip
        }

        if (reportedID != id) {
            // Module reported an ID that doesn't match what it was registered
            // under - treat this the same as a failed check-in.
            it->second.failCount++;
            it->second.status = statusCode::STALED;
        } else if (it->second.sinceAlive <= WATCHDOG_TICK_LIMIT) {
            // Passed within the OK window (0-100 ticks).
            it->second.status    = statusCode::OK;
            it->second.failCount = 0;
        } else {
            // Failed this check (elapsed ticks > 100).
            it->second.failCount++;
            it->second.status = statusCode::STALED;

            auto ehIt = errorHandlers.find(id);
            if (ehIt != errorHandlers.end()) {
                ehIt->second.raiseWarning(static_cast<int>(statusCode::STALED));
            }
        }

        if (it->second.failCount >= WATCHDOG_MAX_FAILURES) {
            // 5 failures in a row - considered DEAD, kill and restart it,
            // which also clears sinceAlive/failCount for a clean slate.
            it->second.status = statusCode::DEAD;
            restartRuntime(id);
        } else {
            // Reset the tick counter now that this pass has scored it,
            // whether it passed or failed.
            it->second.sinceAlive = 0;
        }
    }
}

const RuntimeManger::runtimeInfo& RuntimeManger::getStatus(int16_t id) const {
    auto it = table.find(id);
    if (it == table.end()) {
        throw std::out_of_range("No runtime registered under that ID");
    }
    return it->second;
}

uint8_t RuntimeManger::getFailCount(int16_t id) const {
    auto it = table.find(id);
    if (it == table.end()) {
        throw std::out_of_range("No runtime registered under that ID");
    }
    return it->second.failCount;
}

void RuntimeManger::setStatus(int16_t id, statusCode status) {
    auto it = table.find(id);
    if (it != table.end()) {
        it->second.status = status;
    }
}

const std::unordered_map<int16_t, RuntimeManger::runtimeInfo>& RuntimeManger::getTable() const {
    return table;
}

}
