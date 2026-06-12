#include "command_wal_gate.hpp"

#include <mutex>

namespace minikv::command_internal {

WalCommandGate::Scope::Scope() = default;

WalCommandGate::Scope::Scope(WalCommandGate& gate, bool compaction) : gate_(&gate), compaction_(compaction) {}

WalCommandGate::Scope::Scope(Scope&& other) noexcept : gate_(other.gate_), compaction_(other.compaction_) {
    other.gate_ = nullptr;
}

WalCommandGate::Scope& WalCommandGate::Scope::operator=(Scope&& other) noexcept {
    if (this != &other) {
        release();
        gate_ = other.gate_;
        compaction_ = other.compaction_;
        other.gate_ = nullptr;
    }
    return *this;
}

WalCommandGate::Scope::~Scope() { release(); }

void WalCommandGate::Scope::release() {
    if (gate_ == nullptr) {
        return;
    }
    gate_->leave(compaction_);
    gate_ = nullptr;
}

WalCommandGate::Scope WalCommandGate::enter_command() {
    std::unique_lock lock{mutex_};
    condition_.wait(lock, [this] { return !command_active_ && !compaction_active_; });
    command_active_ = true;
    return Scope{*this, false};
}

WalCommandGate::Scope WalCommandGate::enter_compaction() {
    std::unique_lock lock{mutex_};
    condition_.wait(lock, [this] { return !command_active_ && !compaction_active_; });
    compaction_active_ = true;
    return Scope{*this, true};
}

void WalCommandGate::leave(bool compaction) {
    {
        std::lock_guard lock{mutex_};
        if (compaction) {
            compaction_active_ = false;
        } else {
            command_active_ = false;
        }
    }
    condition_.notify_all();
}

WalCommandGate& wal_command_gate() {
    static WalCommandGate gate;
    return gate;
}

} // namespace minikv::command_internal
