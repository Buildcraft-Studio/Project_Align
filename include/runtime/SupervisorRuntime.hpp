/** *@file SupervisorRuntime.hpp
 *  @brief Declares WorkerRuntime and SupervisorRuntime - a runtime module
 *         that spawns and registers other runtimes at runtime.
 *  @ingroup RuntimeSystem
**/
#ifndef SUPERVISORRUNTIME_HPP
#define SUPERVISORRUNTIME_HPP

#include "runtime/runtimeManger.hpp"
#include <memory>
#include <vector>
#include <string>

namespace Engine::Runtime {

    /** *@class WorkerRuntime
     *  @brief Minimal worker runtime spawned by SupervisorRuntime. Reports
     *         its own ID like any other IRuntimeModule so WatchDog() can
     *         check it in normally once it's registered.
     *  @ingroup RuntimeSystem
    **/
    class WorkerRuntime : public IRuntimeModule
    {
        public:
            /** *@brief Construct with this worker's own runtime ID.
             *  @param id The runtime ID this worker will be registered under.
            **/
            explicit WorkerRuntime(int16_t id);

            /** *@brief IRuntimeModule contract - reports its own ID during WatchDog().
             *  @return This worker's own runtime ID.
            **/
            int16_t getRuntimeID() const override;

        private:
            int16_t id; ///< This worker's own runtime ID.
    };

    /** *@class SupervisorRuntime
     *  @brief Factory/supervisor-style runtime module. Unlike Bootstrap,
     *         which registers everything once up front, SupervisorRuntime
     *         creates new WorkerRuntime instances DURING normal operation
     *         (e.g. one per incoming job) and registers each one with
     *         RuntimeManger::setRuntime() (createRuntime) as it's spawned.
     *  @note SupervisorRuntime owns the workers it creates (unique_ptr) so
     *        the raw pointers RuntimeManger holds stay valid for their
     *        lifetime, same pattern Bootstrap uses for its modules.
     *  @ingroup RuntimeSystem
    **/
    class SupervisorRuntime : public IRuntimeModule
    {
        public:
            /** *@brief Construct the supervisor.
             *  @param id           This supervisor's own runtime ID.
             *  @param firstWorkerID First ID to hand out to a spawned worker;
             *                       each subsequent worker gets the next one up.
            **/
            SupervisorRuntime(int16_t id, int16_t firstWorkerID);

            /** *@brief IRuntimeModule contract - reports its own ID during WatchDog().
             *  @return This supervisor's own runtime ID.
            **/
            int16_t getRuntimeID() const override;

            /** *@brief Spawns a new WorkerRuntime and registers it with manager
             *         (RuntimeManger::setRuntime / createRuntime), at runtime,
             *         rather than at Bootstrap time.
             *  @param manager The RuntimeManger instance to register the new worker with.
             *  @param name    Human-readable name for the new worker's table entry.
             *  @return The runtime ID assigned to the new worker.
            **/
            int16_t createWorker(RuntimeManger& manager, const std::string& name);

        private:
            int16_t id;           ///< This supervisor's own runtime ID.
            int16_t nextWorkerID; ///< Next free ID to hand out to a spawned worker.
            std::vector<std::unique_ptr<IRuntimeModule>> workers; ///< Owns spawned workers so RuntimeManger's raw pointers stay valid.
    };
}
#endif // SUPERVISORRUNTIME_HPP
