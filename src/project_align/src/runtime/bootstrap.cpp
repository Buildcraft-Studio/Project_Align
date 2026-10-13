#include "project_align/runtime/Bootstrap.hpp"

namespace Engine::Runtime {

Bootstrap::Bootstrap() : service_(), watchdog_(), running_(false) {}

Bootstrap::Bootstrap(Service service, Watchdog watchdog)
    : service_(std::move(service)), watchdog_(std::move(watchdog)), running_(false) {}

void Bootstrap::initialize() {
    running_ = true;
}

void Bootstrap::start() {
    initialize();
}

void Bootstrap::stop() {
    running_ = false;
}

bool Bootstrap::isRunning() const noexcept {
    return running_;
}

Service& Bootstrap::getService() noexcept {
    return service_;
}

const Service& Bootstrap::getService() const noexcept {
    return service_;
}

Watchdog& Bootstrap::getWatchdog() noexcept {
    return watchdog_;
}

const Watchdog& Bootstrap::getWatchdog() const noexcept {
    return watchdog_;
}

}  // namespace Engine::Runtime
