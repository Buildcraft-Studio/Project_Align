#ifndef PROJECT_ALIGN_RUNTIME_BOOTSTRAP_HPP
#define PROJECT_ALIGN_RUNTIME_BOOTSTRAP_HPP
#include <utility>

#include "Service.hpp"
#include "Watchdog.hpp"

namespace Engine::Runtime {

class Bootstrap {
public:
    Bootstrap();

    explicit Bootstrap(Service service, Watchdog watchdog);

    void initialize();

    void start();

    void stop();

    bool isRunning() const noexcept;

    Service& getService() noexcept;

    const Service& getService() const noexcept;

    Watchdog& getWatchdog() noexcept;

    const Watchdog& getWatchdog() const noexcept;

private:
    Service service_;
    Watchdog watchdog_;
    bool running_;
};

}  // namespace Engine::Runtime

#endif  // PROJECT_ALIGN_RUNTIME_BOOTSTRAP_HPP