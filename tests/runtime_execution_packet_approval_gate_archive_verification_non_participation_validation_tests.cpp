#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

std::string format_validation(int source_stage_count = 20,
                              bool source_complete = true,
                              bool fresh_minikv = false,
                              bool fresh_java = false,
                              bool reads_archive = false,
                              bool replays_archive = false,
                              bool starts_node = false,
                              bool starts_java = false,
                              bool starts_minikv = false,
                              bool reads_endpoints = false,
                              bool reads_credentials = false,
                              bool parses_raw_endpoint = false,
                              bool executes_probe = false,
                              bool enables_router = false,
                              int planned_checks = 20,
                              int completed_checks = 20,
                              int published_stages = 20,
                              int planned_stages = 20) {
    namespace validation =
        minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation;
    return validation::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_json(
        source_stage_count,
        source_complete,
        fresh_minikv,
        fresh_java,
        reads_archive,
        replays_archive,
        starts_node,
        starts_java,
        starts_minikv,
        reads_endpoints,
        reads_credentials,
        parses_raw_endpoint,
        executes_probe,
        enables_router,
        planned_checks,
        completed_checks,
        published_stages,
        planned_stages);
}

void assert_validation_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json,
        "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = format_validation();
    assert_contains(valid,
                    "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":true");
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"nodePlanObservedOnly\":true");
    assert_contains(valid, "\"miniKvBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");
    assert_contains(valid, "\"approvalGateBlocked\":true");
    assert_contains(valid, "\"approvalInputCount\":0");
    assert_contains(valid, "\"requiredApprovalInputCount\":3");
    assert_contains(valid, "\"runtimeArtifactCount\":0");
    assert_contains(valid, "\"requiredRuntimeArtifactCount\":6");
    assert_contains(valid, "\"nodeArchiveFilesReadByMiniKv\":false");
    assert_contains(valid, "\"managedAuditConnectionOpened\":false");
    assert_contains(valid, "\"writeRoutingAllowed\":false");
    assert_contains(valid, "\"executionAllowed\":false");

    assert_validation_fails(format_validation(19));
    assert_validation_fails(format_validation(20, false));
    assert_validation_fails(format_validation(20, true, true));
    assert_validation_fails(format_validation(20, true, false, true));
    assert_validation_fails(format_validation(20, true, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, false, 20, 19));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, false, 20, 20, 19));

    return 0;
}
