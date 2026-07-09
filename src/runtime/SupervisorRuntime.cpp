#include "runtime/SupervisorRuntime.hpp"

namespace Engine::Runtime {

WorkerRuntime::WorkerRuntime(int16_t id)
    : id(id) {

}

int16_t WorkerRuntime::getRuntimeID() const {
    return id;
}

SupervisorRuntime::SupervisorRuntime(int16_t id, int16_t firstWorkerID)
    : id(id), nextWorkerID(firstWorkerID) {

}

int16_t SupervisorRuntime::getRuntimeID() const {
    return id;
}

int16_t SupervisorRuntime::createWorker(RuntimeManger& manager, const std::string& name) {
    int16_t newID = nextWorkerID++;

    auto worker = std::make_unique<WorkerRuntime>(newID);
    manager.setRuntime(newID, name, worker.get()); // createRuntime, called at runtime - not by Bootstrap
    workers.push_back(std::move(worker));

    return newID;
}

}
