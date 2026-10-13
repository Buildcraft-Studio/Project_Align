#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Engine::Runtime {

enum class ServiceStatus : std::int8_t {
    Standby = 0,
    Healthy = 1,
    Degrading = 3,
    Stalled = 4,
    Retry1 = 5,
    Retry2 = 6,
    FinalRetry = 7,
    Dead = 8,
    Disabled = -1
};

struct ServiceProfile {
    std::int32_t serviceID{0};
    std::string name;
    ServiceStatus status{ServiceStatus::Standby};
    std::int32_t lastCheck{0};
};

class Service {
public:
    Service() = default;

    explicit Service(std::vector<ServiceProfile> services);

    void addService(std::int32_t id, std::string name,
                    ServiceStatus status = ServiceStatus::Standby,
                    std::int32_t lastCheck = 0);

    void updateService(std::int32_t id, ServiceStatus status,
                       std::int32_t lastCheck = 0);

    bool removeService(std::int32_t id);

    const ServiceProfile* findService(std::int32_t id) const;

    std::vector<ServiceProfile> getServices() const;

    std::size_t size() const noexcept;

private:
    std::vector<ServiceProfile> runtimeTable_;
};

}  // namespace Engine::Runtime