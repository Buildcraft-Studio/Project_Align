#include <cassert>
#include <chrono>
#include <string>

#include "project_align/greeter.h"
#include "project_align/runtime/Bootstrap.hpp"

int main() {
    const std::string result = project_align::greet("world");
    assert(result == "Hello, world!");

    Engine::Runtime::Service service;
    service.addService(1, "core");
    assert(service.size() == 1U);

    const auto* profile = service.findService(1);
    assert(profile != nullptr);
    assert(profile->name == "core");

    Engine::Runtime::Watchdog watchdog(std::chrono::milliseconds(100), 2);
    watchdog.registerService(1, "core");
    watchdog.heartbeat(1);
    assert(watchdog.isHealthy(1));

    Engine::Runtime::Bootstrap bootstrap(service, watchdog);
    assert(!bootstrap.isRunning());
    bootstrap.start();
    assert(bootstrap.isRunning());

    return 0;
}
