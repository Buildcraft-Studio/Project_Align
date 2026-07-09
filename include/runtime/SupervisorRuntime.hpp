#ifndef SUPERVISORRUNTIME_HPP
#define SUPERVISORRUNTIME_HPP

#include "runtime/runtimeManger.hpp"
#include <memory>
#include <vector>
#include <string>

namespace Engine::Runtime {

    /** *@brief Minimal worker runtime spawned by SupervisorRuntime. Reports
     *         its own ID like any other IRuntimeModule so WatchDog() can
     *         check it in normally once it's registered.
    **/
    class WorkerRuntime : public IRuntimeModule
    {
        public:
            explicit WorkerRuntime(int16_t id);
            int16_t getRuntimeID() const override;

        private:
            int16_t id;
    };

    /** *@brief Factory/supervisor-style runtime module. Unlike Bootstrap,
     *         which registers everything once up front, SupervisorRuntime
     *         creates new WorkerRuntime instances DURING normal operation
     *         (e.g. one per incoming job) and registers each one with
     *         RuntimeManger::setRuntime() (createRuntime) as it's spawned.
     *  @note SupervisorRuntime owns the workers it creates (unique_ptr) so
     *        the raw pointers RuntimeManger holds stay valid for their
     *        lifetime, same pattern Bootstrap uses for its modules.
    **/
    class SupervisorRuntime : public IRuntimeModule
    {
        public:
            /// @param id           This supervisor's own runtime ID.
            /// @param firstWorkerID First ID to hand out to a spawned worker;
            ///                      each subsequent worker gets the next one up.
            SupervisorRuntime(int16_t id, int16_t firstWorkerID);

            int16_t getRuntimeID() const override;

            /// @brief Spawns a new WorkerRuntime and registers it with manager
            ///        (RuntimeManger::setRuntime / createRuntime), at runtime,
            ///        rather than at Bootstrap time.
            /// @return The runtime ID assigned to the new worker.
            int16_t createWorker(RuntimeManger& manager, const std::string& name);

        private:
            int16_t id;
            int16_t nextWorkerID;
            std::vector<std::unique_ptr<IRuntimeModule>> workers;
    };
}
#endif // SUPERVISORRUNTIME_HPP
