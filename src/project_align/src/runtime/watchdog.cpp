#include "project_align/runtime/Watchdog.hpp"

namespace Engine::Runtime {

Watchdog::Watchdog(std::chrono::milliseconds tolerance, std::size_t maxRetries)
    : monitoredServices_(), tolerance_(tolerance), maxRetries_(maxRetries) {}

void Watchdog::registerService(const ServiceProfile& profile) {
    monitoredServices_[profile.serviceID] = WatchState{profile, Clock::now(), 0};
}

void Watchdog::registerService(std::int32_t id, std::string name,
                              ServiceStatus status) {
    registerService(ServiceProfile{id, std::move(name), status, 0});
}

void Watchdog::heartbeat(std::int32_t id) {
    auto it = monitoredServices_.find(id);
    if (it != monitoredServices_.end()) {
        it->second.lastHeartbeat = Clock::now();
        it->second.retryCount = 0;
        it->second.profile.status = ServiceStatus::Healthy;
    }
}

bool Watchdog::isHealthy(std::int32_t id) const {
    const auto it = monitoredServices_.find(id);
    if (it == monitoredServices_.end()) {
        return false;
    }

    return (Clock::now() - it->second.lastHeartbeat) <= tolerance_ &&
           it->second.profile.status != ServiceStatus::Dead &&
           it->second.profile.status != ServiceStatus::Disabled;
}

void Watchdog::checkForTimeouts() {
    const auto now = Clock::now();

    for (auto& [id, state] : monitoredServices_) {
        const auto elapsed = now - state.lastHeartbeat;
        if (elapsed > tolerance_) {
            ++state.retryCount;
            state.profile.status = (state.retryCount >= maxRetries_)
                                      ? ServiceStatus::Dead
                                      : ServiceStatus::Stalled;
        }
    }
}

std::vector<std::int32_t> Watchdog::deadServices() const {
    std::vector<std::int32_t> result;
    for (const auto& [id, state] : monitoredServices_) {
        if (state.profile.status == ServiceStatus::Dead) {
            result.push_back(id);
        }
    }
    return result;
}

}  // namespace Engine::Runtime
