#include "project_align/runtime/Gameclock.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace Engine::Runtime {

void Gameclock::startGClock() noexcept {
    const auto now = Clock::now();
    startedAt_ = now;
    intervalStartedAt_ = now;
    intervalElapsedMilliseconds_ = 0;
    started_ = true;
}

double Gameclock::elapsed() const noexcept {
    if (!started_) {
        return 0.0;
    }
    return std::chrono::duration<double>(Clock::now() - startedAt_).count();
}

std::string Gameclock::elapsedHMS() const {
    const auto elapsedDuration = started_ ? Clock::now() - startedAt_ : Clock::duration::zero();
    const auto totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedDuration).count();
    const auto hours = totalSeconds / 3600;
    const auto minutes = (totalSeconds / 60) % 60;
    const auto seconds = totalSeconds % 60;

    std::ostringstream formatted;
    formatted << std::setfill('0') << std::setw(2) << hours << ':'
              << std::setw(2) << minutes << ':' << std::setw(2) << seconds;
    return formatted.str();
}

std::string Gameclock::elapsedSSMMMM() const {
    const auto elapsedDuration = started_ ? Clock::now() - startedAt_ : Clock::duration::zero();
    const auto totalMicroseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsedDuration).count();
    const auto seconds = (totalMicroseconds / 1'000'000) % 60;
    const auto tenThousandths = (totalMicroseconds % 1'000'000) / 100;

    std::ostringstream formatted;
    formatted << std::setfill('0') << std::setw(2) << seconds << ':'
              << std::setw(4) << tenThousandths;
    return formatted.str();
}

std::int64_t Gameclock::intervalElapsedMilliseconds() const noexcept {
    return intervalElapsedMilliseconds_;
}

bool Gameclock::execute(std::chrono::duration<double> heartbeatInterval) {
    if (!started_) {
        throw std::runtime_error("Game clock has not been started");
    }
    if (!std::isfinite(heartbeatInterval.count()) ||
        heartbeatInterval <= std::chrono::duration<double>::zero()) {
        throw std::invalid_argument("Game clock heartbeat interval must be finite and positive");
    }

    const auto now = Clock::now();
    const auto intervalElapsed = now - intervalStartedAt_;
    if (intervalElapsed >= heartbeatInterval) {
        intervalStartedAt_ = now;
        intervalElapsedMilliseconds_ = 0;
        return true;
    }

    intervalElapsedMilliseconds_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(intervalElapsed).count();
    return false;
}

bool Gameclock::checkGClock(float thresholdSeconds) {
    return execute(std::chrono::duration<double>(thresholdSeconds));
}

}  // namespace Engine::Runtime
