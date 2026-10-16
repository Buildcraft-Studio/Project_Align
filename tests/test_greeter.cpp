#include <cassert>
#include <chrono>
#include <string>
#include <thread>

#include "project_align/greeter.h"
#include "project_align/runtime/Bootstrap.hpp"
#include "project_align/runtime/Gameclock.hpp"
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

    Engine::Runtime::Watchdog gameclockWatchdog(std::chrono::milliseconds(10), 2);
    gameclockWatchdog.registerService(2, "gameclock");
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    gameclockWatchdog.checkForTimeouts();
    assert(!gameclockWatchdog.isHealthy(2));
    gameclockWatchdog.heartbeat(2);
    assert(gameclockWatchdog.isHealthy(2));

    Engine::Runtime::Gameclock gameClock;
    assert(gameClock.elapsed() == 0.0);
    assert(gameClock.elapsedHMS() == "00:00:00");
    assert(gameClock.elapsedSSMMMM() == "00:0000");
    gameClock.startGClock();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    assert(gameClock.elapsedHMS().size() == 8U);
    const auto rollingTime = gameClock.elapsedSSMMMM();
    assert(rollingTime.size() == 7U);
    assert(rollingTime[2] == ':');
    assert(rollingTime.substr(3).find_first_not_of("0123456789") == std::string::npos);
    assert(gameClock.execute(std::chrono::milliseconds(1)));
    assert(gameClock.intervalElapsedMilliseconds() == 0);
    const double elapsedAfterThreshold = gameClock.elapsed();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    assert(gameClock.elapsed() > elapsedAfterThreshold);
    assert(!gameClock.execute(std::chrono::milliseconds(100)));
    assert(gameClock.intervalElapsedMilliseconds() > 0);

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
