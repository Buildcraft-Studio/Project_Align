#include <cassert>
#include <chrono>
#include <string>

#include "project_align/greeter.h"
#include "project_align/runtime/Bootstrap.hpp"
#include "project_align/runtime/TUI.hpp"

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

    Engine::Runtime::TUI tui;
    tui.setPreference("theme", "dark");
    tui.setPreference("refresh_hz", "10");
    assert(tui.getPreference("theme") == "dark");
    assert(tui.getPreference("refresh_hz") == "10");

    tui.addProcess("heartbeat", "watchdog");
    assert(tui.hasProcess("heartbeat"));

    return 0;
}
