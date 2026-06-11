#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

int main() {
    namespace checks =
        minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks;
    using minikv::test_support::assert_contains;

    const auto records =
        checks::runtime_execution_packet_approval_gate_archive_verification_non_participation_checks();
    assert(records.size() == 20);
    assert(checks::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_check_count() ==
           20);
    assert(records.front().sequence == 1);
    assert(records.front().check_code == "node-v399-plan-observed-only");
    assert(records.back().sequence == 20);
    assert(records.back().check_code == "clean-ci-closeout-no-services");

    const auto empty =
        checks::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json =
        checks::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(20);
    for (const auto& check : records) {
        assert_contains(json, "\"checkCode\":\"" + std::string{check.check_code} + "\"");
        assert_contains(json, "\"source\":\"" + std::string{check.source} + "\"");
    }

    const std::string_view false_fields[] = {
        "nodeArchiveFilesReadByMiniKv",
        "nodeArchiveReplayExecutedByMiniKv",
        "startsJavaService",
        "startsMiniKvService",
        "credentialValueRead",
        "rawEndpointParsed",
        "managedAuditConnectionOpened",
        "runtimeProbeExecuted",
        "runtimeExecutionPacketPresent",
        "runtimeExecutionPacketExecutable",
        "runtimeGateApprovalPresent",
        "activeShardPrototypeEnabled",
        "activeRouterInstalled",
        "writeRoutingAllowed",
        "mutatesStore",
        "touchesWal",
        "executionAllowed",
    };
    for (const auto field : false_fields) {
        assert_contains(json, "\"" + std::string{field} + "\":false");
    }
    assert_contains(json, "\"approvalInputCount\":0");
    assert_contains(json, "\"requiredApprovalInputCount\":3");
    assert_contains(json, "\"runtimeArtifactCount\":0");
    assert_contains(json, "\"requiredRuntimeArtifactCount\":6");

    bool threw_negative = false;
    try {
        (void)checks::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(
            -1);
    } catch (const std::out_of_range&) {
        threw_negative = true;
    }
    assert(threw_negative);

    bool threw_too_many = false;
    try {
        (void)checks::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(
            21);
    } catch (const std::out_of_range&) {
        threw_too_many = true;
    }
    assert(threw_too_many);

    return 0;
}
