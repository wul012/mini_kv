#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks {

struct RuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const RuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheck>
runtime_execution_packet_approval_gate_archive_verification_non_participation_checks();
int planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_check_count();
std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks
