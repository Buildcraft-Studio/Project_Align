#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine::Runtime {

class TUI {
public:
    using PreferenceMap = std::unordered_map<std::string, std::string>;
    using ProcessMap = std::unordered_map<std::string, std::string>;

    void setPreference(std::string key, std::string value) {
        preferences_[std::move(key)] = std::move(value);
    }

    std::string getPreference(const std::string& key,
                             const std::string& fallback = "") const {
        const auto it = preferences_.find(key);
        return it == preferences_.end() ? fallback : it->second;
    }

    bool hasPreference(const std::string& key) const {
        return preferences_.find(key) != preferences_.end();
    }

    void addProcess(std::string name, std::string handler) {
        processes_[std::move(name)] = std::move(handler);
    }

    bool hasProcess(const std::string& name) const {
        return processes_.find(name) != processes_.end();
    }

    std::vector<std::string> processNames() const {
        std::vector<std::string> names;
        names.reserve(processes_.size());
        for (const auto& [name, _] : processes_) {
            names.push_back(name);
        }
        return names;
    }

    std::size_t processCount() const noexcept {
        return processes_.size();
    }

    void clear() {
        preferences_.clear();
        processes_.clear();
    }

private:
    PreferenceMap preferences_;
    ProcessMap processes_;
};

}  // namespace Engine::Runtime
