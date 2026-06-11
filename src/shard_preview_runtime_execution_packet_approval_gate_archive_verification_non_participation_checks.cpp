#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks {
namespace {

constexpr std::array<RuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheck, 20> checks{{
    {1, "node-v399-plan-observed-only", "Node v399 plan",
     "mini-kv records the archive verification plan without executing Node archive verification"},
    {2, "v398-json-handles-not-read", "Node v399 archive input",
     "Node v398 JSON and summary archive paths stay as handles"},
    {3, "v398-markdown-handles-not-read", "Node v399 archive input",
     "Node v398 Markdown explanation and walkthrough paths stay as handles"},
    {4, "browser-evidence-handles-not-rendered", "Node v399 archive input",
     "browser snapshot and screenshot evidence are not rendered or inspected by mini-kv"},
    {5, "frozen-replay-not-executed", "Node v399 replay",
     "replay from frozen evidence remains Node-owned and is not executed in C++"},
    {6, "approval-input-counts-blocked", "Node v399 runtime boundary",
     "0 of 3 approval inputs is preserved as a blocker"},
    {7, "runtime-artifact-counts-blocked", "Node v399 runtime boundary",
     "0 of 6 runtime artifacts is preserved as a blocker"},
    {8, "runtime-execution-packet-not-present", "Node v399 runtime boundary",
     "runtime execution packet absence cannot be converted to approval"},
    {9, "runtime-gate-approval-not-present", "Node v399 runtime boundary",
     "runtime gate approval absence remains explicit"},
    {10, "java-service-not-started", "Node v399 no-start boundary",
     "Java service startup is not delegated to mini-kv"},
    {11, "minikv-service-not-started", "Node v399 no-start boundary",
     "sibling mini-kv service startup is not performed"},
    {12, "credential-value-not-read", "Node v399 credential boundary",
     "credential and secret values remain unavailable to mini-kv"},
    {13, "raw-endpoint-not-parsed", "Node v399 endpoint boundary",
     "raw endpoint URLs are not parsed or emitted"},
    {14, "managed-audit-connection-not-opened", "Node v399 connection boundary",
     "managed audit connection remains closed"},
    {15, "active-shard-prototype-not-enabled", "Node v399 router boundary",
     "active shard prototype stays disabled"},
    {16, "store-and-wal-not-mutated", "Node v399 write boundary",
     "archive verification evidence does not mutate store or WAL"},
    {17, "node-v400-handoff-blocked-inputs-only", "Node v399 handoff",
     "Node v400 may consume real inputs later, not this archive as approval"},
    {18, "warnings-and-blockers-preserved", "Node v399 blocker model",
     "missing approval inputs and runtime artifacts remain warnings and blockers"},
    {19, "command-surface-read-only", "mini-kv v1583",
     "standalone command exposes only read-only JSON evidence"},
    {20, "clean-ci-closeout-no-services", "mini-kv v1585",
     "cleanup and CI close the evidence without local long-running processes"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const RuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheck>
runtime_execution_packet_approval_gate_archive_verification_non_participation_checks() {
    return checks;
}

int planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{
            "published check count exceeds runtime execution packet approval gate archive verification checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"source\":" + json_string(check.source) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"archiveVerificationOnly\":true"
                  ",\"nodeArchiveFilesReadByMiniKv\":false"
                  ",\"nodeArchiveReplayExecutedByMiniKv\":false"
                  ",\"startsJavaService\":false"
                  ",\"startsMiniKvService\":false"
                  ",\"credentialValueRead\":false"
                  ",\"rawEndpointParsed\":false"
                  ",\"managedAuditConnectionOpened\":false"
                  ",\"runtimeProbeExecuted\":false"
                  ",\"runtimeExecutionPacketPresent\":false"
                  ",\"runtimeExecutionPacketExecutable\":false"
                  ",\"runtimeGateApprovalPresent\":false"
                  ",\"approvalInputCount\":0"
                  ",\"requiredApprovalInputCount\":3"
                  ",\"runtimeArtifactCount\":0"
                  ",\"requiredRuntimeArtifactCount\":6"
                  ",\"activeShardPrototypeEnabled\":false"
                  ",\"activeRouterInstalled\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"mutatesStore\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks
