#pragma once

#include <condition_variable>
#include <mutex>

namespace minikv::command_internal {

class WalCommandGate {
public:
    class Scope {
    public:
        Scope();
        Scope(WalCommandGate& gate, bool compaction);

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

        Scope(Scope&& other) noexcept;
        Scope& operator=(Scope&& other) noexcept;

        ~Scope();

    private:
        void release();

        WalCommandGate* gate_ = nullptr;
        bool compaction_ = false;
    };

    Scope enter_command();
    Scope enter_compaction();

private:
    friend class Scope;

    void leave(bool compaction);

    std::mutex mutex_;
    std::condition_variable condition_;
    bool command_active_ = false;
    bool compaction_active_ = false;
};

WalCommandGate& wal_command_gate();

} // namespace minikv::command_internal
