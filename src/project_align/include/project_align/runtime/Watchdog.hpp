#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Service.hpp"

namespace Engine::Runtime {

class Watchdog {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    explicit Watchdog(std::chrono::milliseconds tolerance = std::chrono::seconds(5),
                      std::size_t maxRetries = 3);

    void registerService(const ServiceProfile& profile);

    void registerService(std::int32_t id, std::string name,
                         ServiceStatus status = ServiceStatus::Standby);

    void heartbeat(std::int32_t id);

    bool isHealthy(std::int32_t id) const;

    void checkForTimeouts();

    std::vector<std::int32_t> deadServices() const;

private:
    struct WatchState {
        ServiceProfile profile;
        TimePoint lastHeartbeat;
        std::size_t retryCount{0};
    };

    std::unordered_map<std::int32_t, WatchState> monitoredServices_;
    std::chrono::milliseconds tolerance_;
    std::size_t maxRetries_;
};

}  // namespace Engine::Runtime 