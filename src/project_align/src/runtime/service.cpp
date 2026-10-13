#include "project_align/runtime/Service.hpp"

#include <algorithm>

namespace Engine::Runtime {

Service::Service(std::vector<ServiceProfile> services)
    : runtimeTable_(std::move(services)) {}

void Service::addService(std::int32_t id, std::string name,
                        ServiceStatus status,
                        std::int32_t lastCheck) {
    runtimeTable_.push_back(ServiceProfile{id, std::move(name), status, lastCheck});
}

void Service::updateService(std::int32_t id, ServiceStatus status,
                           std::int32_t lastCheck) {
    auto it = std::find_if(runtimeTable_.begin(), runtimeTable_.end(),
                          [id](const ServiceProfile& profile) {
                              return profile.serviceID == id;
                          });

    if (it != runtimeTable_.end()) {
        it->status = status;
        it->lastCheck = lastCheck;
    }
}

bool Service::removeService(std::int32_t id) {
    const auto before = runtimeTable_.size();
    runtimeTable_.erase(
        std::remove_if(runtimeTable_.begin(), runtimeTable_.end(),
                       [id](const ServiceProfile& profile) {
                           return profile.serviceID == id;
                       }),
        runtimeTable_.end());

    return runtimeTable_.size() != before;
}

const ServiceProfile* Service::findService(std::int32_t id) const {
    const auto it = std::find_if(runtimeTable_.cbegin(), runtimeTable_.cend(),
                                [id](const ServiceProfile& profile) {
                                    return profile.serviceID == id;
                                });

    return it == runtimeTable_.cend() ? nullptr : &(*it);
}

std::vector<ServiceProfile> Service::getServices() const {
    return runtimeTable_;
}

std::size_t Service::size() const noexcept {
    return runtimeTable_.size();
}

}  // namespace Engine::Runtime
