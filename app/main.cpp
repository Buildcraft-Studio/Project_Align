#include <chrono>
#include <iostream>
#include <thread>

#include "project_align/runtime/Bootstrap.hpp"
#include "project_align/runtime/Gameclock.hpp"
#include "project_align/runtime/TUI.hpp"

int main() {
    using namespace Engine::Runtime;
    constexpr std::int32_t bootstrapServiceId = 1;
    constexpr std::int32_t gameclockServiceId = 2;

    Service service;
    service.addService(bootstrapServiceId, "bootstrap", ServiceStatus::Healthy);
    service.addService(gameclockServiceId, "gameclock", ServiceStatus::Healthy);

    Watchdog watchdog(std::chrono::seconds(5), 2);
    for (const auto& profile : service.getServices()) {
        watchdog.registerService(profile);
        watchdog.heartbeat(profile.serviceID);
    }

    Bootstrap bootstrap(service, watchdog);
    bootstrap.initialize();
    bootstrap.start();

    Gameclock gameClock;
    gameClock.startGClock();

    TUI tui;
    tui.setPreference("theme", "dark");
    tui.setPreference("refresh_hz", "10");
    tui.addProcess("heartbeat", "watchdog");
    tui.addProcess("status", "render");

    std::cout << "Project Align Started!" << '\n';
    std::cout << "runtime status: " << (bootstrap.isRunning() ? "running" : "stopped")
              << " | ui_theme=" << tui.getPreference("theme")
              << " | processes=" << tui.processCount() << '\n';

    while (bootstrap.isRunning()) {
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check for service health
        auto& runtimeWatchdog = bootstrap.getWatchdog();
        auto& runtimeServices = bootstrap.getService();
        runtimeWatchdog.heartbeat(bootstrapServiceId);

        const bool gameclockHeartbeat = gameClock.execute(std::chrono::seconds(1));
        if (gameclockHeartbeat) {
            runtimeWatchdog.heartbeat(gameclockServiceId);
        }
        runtimeWatchdog.checkForTimeouts();

        for (const auto serviceId : {bootstrapServiceId, gameclockServiceId}) {
            const auto* profile = runtimeServices.findService(serviceId);
            if (profile == nullptr) {
                continue;
            }
            if (!runtimeWatchdog.isHealthy(serviceId)) {
                std::cout << "Service '" << profile->name << "' is not healthy!" << '\n';
                runtimeServices.updateService(serviceId, ServiceStatus::Stalled);
                bootstrap.stop();
            } else {
                runtimeServices.updateService(serviceId, ServiceStatus::Healthy);
            }
        }

        if (gameclockHeartbeat) {
            std::cout << "Game time: " << gameClock.elapsedHMS()
                      << " | Rolling time: " << gameClock.elapsedSSMMMM() << '\n';
        }
    }
    return 0;
}
