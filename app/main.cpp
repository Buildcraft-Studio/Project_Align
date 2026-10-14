#include <chrono>
#include <iostream>

#include "project_align/greeter.h"
#include "project_align/runtime/Bootstrap.hpp"
#include "project_align/runtime/TUI.hpp"

int main() {
    using namespace Engine::Runtime;

    Service service;
    service.addService(1, "bootstrap", ServiceStatus::Healthy);

    Watchdog watchdog(std::chrono::seconds(5), 2);
    watchdog.registerService(1, "bootstrap");
    watchdog.heartbeat(1);

    Bootstrap bootstrap(service, watchdog);
    bootstrap.initialize();
    bootstrap.start();

    TUI tui;
    tui.setPreference("theme", "dark");
    tui.setPreference("refresh_hz", "10");
    tui.addProcess("heartbeat", "watchdog");
    tui.addProcess("status", "render");

    std::cout << project_align::greet("world") << '\n';
    std::cout << "runtime status: " << (bootstrap.isRunning() ? "running" : "stopped")
              << " | ui_theme=" << tui.getPreference("theme")
              << " | processes=" << tui.processCount() << '\n';

    return 0;
}
