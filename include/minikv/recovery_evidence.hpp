#pragma once

#include "minikv/store.hpp"
#include "minikv/wal.hpp"

#include <cstddef>
#include <string>

namespace minikv {

struct RecoveryEvidenceState {
    std::size_t key_count = 0;
    std::string digest;
};

struct RestartRecoveryEvidence {
    RecoveryEvidenceState before;
    RecoveryEvidenceState after;
    bool recovered = false;
    bool digests_match = false;
    std::size_t snapshot_saved_keys = 0;
    WalReplayReport wal_replay;
};

RecoveryEvidenceState recovery_evidence_state(const Store& store);

RestartRecoveryEvidence make_restart_recovery_evidence(const Store& before_restart,
                                                       const Store& after_restart,
                                                       std::size_t snapshot_saved_keys,
                                                       const WalReplayReport& wal_replay);

std::string format_restart_recovery_evidence_json(const RestartRecoveryEvidence& evidence);

} // namespace minikv
