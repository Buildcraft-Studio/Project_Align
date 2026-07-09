#include "runtime/runtimeManger.hpp"
#include "runtime/Bootstrap.hpp"
#include "runtime/DebugAndLogRuntime.hpp"
#include "runtime/SupervisorRuntime.hpp"
#include <iostream>

int main() {
    using namespace Engine::Runtime;

    RuntimeManger manager;
    Bootstrap     bootstrap;
    DebugAndLogRuntime debugView(1);

    // Bootstrap loads/registers every startup-time runtime module
    // (createRuntime under the hood) and marks them STARTED.
    bootstrap.loadRuntimes(manager);

    // Normal operation: a few healthy ticks/WatchDog passes for the
    // Bootstrap-time modules.
    for (int tick = 0; tick < 6; ++tick) {
        manager.incrementCounter(1); // DebugAndLogRuntime
        manager.WatchDog();
    }

    // Later, AT RUNTIME (not at Bootstrap time), the supervisor spawns a
    // worker on demand and registers it itself via createRuntime.
    SupervisorRuntime* supervisor = bootstrap.getSupervisor();
    int16_t workerID = supervisor->createWorker(manager, "JobWorker-1");

    // Simulate the worker going quiet: each round it accumulates more than
    // WATCHDOG_TICK_LIMIT ticks before a single WatchDog pass scores it, so
    // it fails that check. Repeat until it goes DEAD and auto-restarts.
    for (int failurePass = 0; failurePass < RuntimeManger::WATCHDOG_MAX_FAILURES; ++failurePass) {
        for (int tick = 0; tick <= RuntimeManger::WATCHDOG_TICK_LIMIT; ++tick) {
            manager.incrementCounter(workerID);
        }
        manager.WatchDog();

        if (failurePass == RuntimeManger::WATCHDOG_MAX_FAILURES - 2) {
            std::cout << "-- after " << (failurePass + 1) << " failure(s) --\n";
            debugView.logFailures(manager);
        }
    }

    std::cout << "-- after the " << static_cast<int>(RuntimeManger::WATCHDOG_MAX_FAILURES)
              << "th failure (DEAD -> auto-restarted) --\n";
    debugView.logAll(manager);
    debugView.logFailures(manager); // should print nothing now - restart cleared failCount

    return 0;
}
