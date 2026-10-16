#ifndef PROJECT_ALIGN_RUNTIME_GAMECLOCK_HPP
#define PROJECT_ALIGN_RUNTIME_GAMECLOCK_HPP

#include <chrono>
#include <cstdint>
#include <string>

namespace Engine::Runtime {

class Gameclock {
public:
    using Clock = std::chrono::steady_clock;

    void startGClock() noexcept;

    double elapsed() const noexcept;

    std::string elapsedHMS() const;

    std::string elapsedSSMMMM() const;

    std::int64_t intervalElapsedMilliseconds() const noexcept;

    bool execute(std::chrono::duration<double> heartbeatInterval);

    bool checkGClock(float thresholdSeconds);

private:
    Clock::time_point startedAt_{};
    Clock::time_point intervalStartedAt_{};
    std::int64_t intervalElapsedMilliseconds_{0};
    bool started_{false};
};

}  // namespace Engine::Runtime

#endif  // PROJECT_ALIGN_RUNTIME_GAMECLOCK_HPP
