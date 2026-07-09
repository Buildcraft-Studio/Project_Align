#include "runtime/Bootstrap.hpp"

namespace Engine::Runtime {

Bootstrap::Bootstrap() {

}

void Bootstrap::loadRuntimes(RuntimeManger& manager) {
    // Runtime ID 0 is RuntimeManger itself (per its own class doc note) -
    // Bootstrap registers everything ELSE starting at ID 1.

    auto debugLog = std::make_unique<DebugAndLogRuntime>(1);
    manager.setRuntime(1, "DebugAndLogRuntime", debugLog.get());
    ownedModules.push_back(std::move(debugLog));

    // SupervisorRuntime gets ID 2, and hands out IDs 100+ to any workers it
    // spawns later at runtime (leaving room for more Bootstrap-time modules
    // between 2 and 100 without colliding with worker IDs).
    auto supervisorPtr = std::make_unique<SupervisorRuntime>(2, /*firstWorkerID=*/100);
    manager.setRuntime(2, "SupervisorRuntime", supervisorPtr.get());
    supervisor = supervisorPtr.get();
    ownedModules.push_back(std::move(supervisorPtr));

    // Add further modules the same way, e.g.:
    //   auto physics = std::make_unique<PhysicsRuntime>(3);
    //   manager.setRuntime(3, "PhysicsRuntime", physics.get());
    //   ownedModules.push_back(std::move(physics));
}

SupervisorRuntime* Bootstrap::getSupervisor() const {
    return supervisor;
}

}
