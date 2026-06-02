#include "shard_readiness_runtime_gate_sections.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::runtime_gate_sections {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_runtime_execution_artifact_intake_preflight_json() {
    return "{\"preflightMode\":\"blocked-missing-runtime-execution-artifacts\","
           "\"sourceFrozenReleaseVersion\":\"v153\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
           "\"runtimeExecutionArtifactsComplete\":false,"
           "\"presentRuntimeExecutionArtifactCount\":0,"
           "\"missingRuntimeExecutionArtifactCount\":6,"
           "\"requiredRuntimeExecutionArtifacts\":" + json_string_array({
               "operator approval record",
               "concrete loopback port assignment",
               "GET-only smoke command",
               "cleanup proof",
               "service owner confirmation",
               "process cleanup rules",
           }) +
           ",\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"executionAttempted\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"failClosedOnMissingArtifacts\":true}";
}

std::string format_runtime_execution_artifact_intake_preflight_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v154\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\","
           "\"frozenStatus\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\","
           "\"preservesBlockedPreflight\":true,"
           "\"frozenPresentRuntimeExecutionArtifactCount\":0,"
           "\"frozenMissingRuntimeExecutionArtifactCount\":6,"
           "\"frozenReadyForRuntimeExecutionPacket\":false,"
           "\"frozenReadyForRuntimeLiveReadGate\":false,"
           "\"frozenRuntimeExecutionArtifactsComplete\":false,"
           "\"frozenRuntimeExecutionPacketPresent\":false,"
           "\"frozenRuntimeExecutionPacketExecutable\":false,"
           "\"frozenExecutionAttempted\":false,"
           "\"frozenStartsMiniKvService\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_mini_kv_runtime_execution_artifact_candidate_json() {
    return "{\"candidateMode\":\"mini-kv-side-candidate-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v154\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\","
           "\"candidateArtifactCount\":4,"
           "\"candidateArtifactRows\":" + json_string_array({
               "mini-kv loopback port candidate",
               "mini-kv GET-only smoke command candidate",
               "mini-kv service owner candidate",
               "mini-kv process cleanup rules candidate",
           }) +
           ",\"acceptedRuntimeExecutionArtifactCount\":0,"
           "\"missingAcceptedRuntimeExecutionArtifactCount\":6,"
           "\"acceptedRuntimeExecutionArtifactsComplete\":false,"
           "\"operatorApprovalRecordPresent\":false,"
           "\"nodeRuntimeWindowApproved\":false,"
           "\"crossProjectRuntimeExecutionPacketPresent\":false,"
           "\"javaPortBindingPresent\":false,"
           "\"miniKvLoopbackPortCandidateDeclared\":true,"
           "\"miniKvLoopbackHost\":\"127.0.0.1\","
           "\"miniKvLoopbackPortCandidate\":6424,"
           "\"miniKvLoopbackPortOperatorApproved\":false,"
           "\"startupCommandCandidate\":\"minikv_server.exe 6424 127.0.0.1\","
           "\"getOnlySmokeCommandCandidate\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\","
           "\"getOnlySmokeCommandOperatorApproved\":false,"
           "\"serviceOwnerCandidateDeclared\":true,"
           "\"serviceOwnerHandle\":\"mini-kv-local-operator\","
           "\"serviceOwnerOperatorConfirmed\":false,"
           "\"processCleanupRulesCandidateDeclared\":true,"
           "\"processCleanupRules\":" + json_string_array({
               "record owned minikv_server pid before smoke",
               "stop only the minikv_server process started by the approved packet",
               "verify the mini-kv candidate port has no LISTENING socket after cleanup",
           }) +
           ",\"cleanupProofPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiresNodeRuntimeWindow\":true,"
           "\"requiresOperatorApproval\":true,"
           "\"requiresCrossProjectPacket\":true}";
}

std::string format_mini_kv_runtime_execution_artifact_candidate_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v154\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\","
           "\"frozenStatus\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\","
           "\"preservesCandidateOnlyContribution\":true,"
           "\"frozenCandidateArtifactCount\":4,"
           "\"frozenAcceptedRuntimeExecutionArtifactCount\":0,"
           "\"frozenMissingAcceptedRuntimeExecutionArtifactCount\":6,"
           "\"frozenAcceptedRuntimeExecutionArtifactsComplete\":false,"
           "\"frozenOperatorApprovalRecordPresent\":false,"
           "\"frozenNodeRuntimeWindowApproved\":false,"
           "\"frozenCrossProjectRuntimeExecutionPacketPresent\":false,"
           "\"frozenMiniKvLoopbackPortCandidate\":6424,"
           "\"frozenMiniKvLoopbackPortOperatorApproved\":false,"
           "\"frozenGetOnlySmokeCommandOperatorApproved\":false,"
           "\"frozenServiceOwnerOperatorConfirmed\":false,"
           "\"frozenCleanupProofPresent\":false,"
           "\"frozenRuntimeExecutionPacketExecutable\":false,"
           "\"frozenStartsMiniKvService\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_runtime_execution_approval_gate_input_precheck_json() {
    return "{\"precheckMode\":\"blocked-missing-approval-gate-inputs\","
           "\"sourceFrozenReleaseVersion\":\"v155\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\","
           "\"sourceNodeReviewVersions\":" + json_string_array({
               "Node v397 contribution review",
               "Node v398 approval gate review",
               "Node v399 approval gate archive verification",
               "Node v400 approval input intake contract",
           }) +
           ",\"approvalGateInputCount\":0,"
           "\"missingApprovalGateInputCount\":3,"
           "\"requiredApprovalGateInputs\":" + json_string_array({
               "Node-approved runtime window",
               "correlated operator approval record",
               "complete cross-project runtime execution packet",
           }) +
           ",\"nodeApprovedRuntimeWindowPresent\":false,"
           "\"correlatedOperatorApprovalRecordPresent\":false,"
           "\"completeCrossProjectRuntimeExecutionPacketPresent\":false,"
           "\"approvalGateInputsComplete\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
           "\"runtimeExecutionArtifactsComplete\":false,"
           "\"presentRuntimeExecutionArtifactCount\":0,"
           "\"missingRuntimeExecutionArtifactCount\":6,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"concreteLoopbackPortsAssigned\":false,"
           "\"executionAttempted\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"failClosedOnMissingApprovalInputs\":true}";
}

std::string format_runtime_execution_approval_gate_input_precheck_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v155\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\","
           "\"frozenStatus\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\","
           "\"preservesApprovalGateInputPrecheck\":true,"
           "\"frozenApprovalGateInputCount\":0,"
           "\"frozenMissingApprovalGateInputCount\":3,"
           "\"frozenNodeApprovedRuntimeWindowPresent\":false,"
           "\"frozenCorrelatedOperatorApprovalRecordPresent\":false,"
           "\"frozenCompleteCrossProjectRuntimeExecutionPacketPresent\":false,"
           "\"frozenApprovalGateInputsComplete\":false,"
           "\"frozenRuntimeGateApprovalPresent\":false,"
           "\"frozenRuntimeExecutionPacketExecutable\":false,"
           "\"frozenConcreteLoopbackPortsAssigned\":false,"
           "\"frozenExecutionAttempted\":false,"
           "\"frozenStartsJavaService\":false,"
           "\"frozenStartsMiniKvService\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_mini_kv_final_approval_gate_input_json() {
    return "{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v155\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\","
           "\"sourceNodeContractVersion\":\"Node v400 approval input intake contract\","
           "\"finalMiniKvApprovalGateInputPresent\":true,"
           "\"miniKvApprovalGateInputComplete\":true,"
           "\"miniKvApprovalGateInputItemCount\":4,"
           "\"miniKvApprovalGateInputItems\":" + json_string_array({
               "concrete mini-kv loopback port assignment",
               "GET-only SHARDJSON smoke command",
               "mini-kv service owner confirmation",
               "stop-only-owned-process cleanup rules",
           }) +
           ",\"miniKvLoopbackHost\":\"127.0.0.1\","
           "\"miniKvLoopbackPort\":6424,"
           "\"startupCommand\":\"minikv_server.exe 6424 127.0.0.1\","
           "\"getOnlySmokeCommand\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\","
           "\"serviceOwnerConfirmed\":true,"
           "\"serviceOwnerHandle\":\"mini-kv-local-operator\","
           "\"processCleanupRulesComplete\":true,"
           "\"stopOnlyOwnedProcessCleanupRequired\":true,"
           "\"processCleanupRules\":" + json_string_array({
               "record owned minikv_server pid before approved smoke",
               "stop only the minikv_server process started by the complete approved packet",
               "verify the mini-kv port has no LISTENING socket after cleanup",
           }) +
           ",\"cleanupProofPresent\":false,"
           "\"cleanupProofRequiresApprovedRuntimeStart\":true,"
           "\"nodeApprovedRuntimeWindowPresent\":false,"
           "\"correlatedOperatorApprovalRecordPresent\":false,"
           "\"completeCrossProjectRuntimeExecutionPacketPresent\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"concreteLoopbackPortsAssignedByCrossProjectPacket\":false,"
           "\"executionAttempted\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiresNodeRuntimeWindow\":true,"
           "\"requiresCorrelatedOperatorApproval\":true,"
           "\"requiresCompleteCrossProjectPacket\":true}";
}

} // namespace minikv::shard_readiness::runtime_gate_sections
