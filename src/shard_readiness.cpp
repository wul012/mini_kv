#include "minikv/shard_readiness.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness {
namespace {

constexpr std::string_view contract_version = "shard-readiness.v1";
constexpr std::string_view release_version = "v156";
constexpr int slot_count = 16;

struct RouteSample {
    std::string_view key;
    int slot;
};

constexpr std::array<RouteSample, 3> route_samples = {{
    {"orderops:alpha", 5},
    {"audit:receipt:001", 9},
    {"mini-kv:health", 14},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string format_route_samples_json() {
    std::string response = "[";
    bool first = true;
    for (const auto& sample : route_samples) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"key\":" + json_string(sample.key) +
                    ",\"slot\":" + std::to_string(sample.slot) +
                    ",\"shardId\":\"shard-0\"}";
    }
    response += "]";
    return response;
}

std::string format_shard_map_json() {
    return "[{\"shardId\":\"shard-0\",\"slotStart\":0,\"slotEnd\":15,"
           "\"storagePath\":\"not-created\",\"readOnly\":true,\"writesAllowed\":false}]";
}

std::string format_boundaries_json() {
    return "{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false,"
           "\"activeRouterInstalled\":false,\"archivedNodeEvidenceMutated\":false}";
}

std::string format_diagnostics_json() {
    return "{\"writeCommandsExecuted\":false,\"adminCommandsExecuted\":false,"
           "\"loadRestoreCompactExecuted\":false,"
           "\"nodeConsumer\":\"Node v401+ may consume v156 only as final mini-kv approval gate input evidence\","
           "\"javaEchoExpected\":\"Java shard-readiness echo may consume the same shard-readiness.v1 fields\","
           "\"nodeArchivedEvidencePreserved\":true}";
}

std::string format_command_catalog_json() {
    return "{\"command\":\"SHARDJSON\",\"category\":\"read\",\"mutatesStore\":false,"
           "\"touchesWal\":false,\"stable\":true,\"extraArgsAllowed\":false,"
           "\"sideEffects\":[\"metadata_read\"]}";
}

std::string format_fixture_parity_json() {
    return "{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\","
           "\"historicalFixturePaths\":" +
           json_string_array({
               "fixtures/release/shard-readiness-v144.json",
               "fixtures/release/shard-readiness-v145.json",
               "fixtures/release/shard-readiness-v146.json",
               "fixtures/release/shard-readiness-v147.json",
                "fixtures/release/shard-readiness-v148.json",
                "fixtures/release/shard-readiness-v149.json",
                "fixtures/release/shard-readiness-v150.json",
                "fixtures/release/shard-readiness-v151.json",
                "fixtures/release/shard-readiness-v152.json",
                "fixtures/release/shard-readiness-v153.json",
                "fixtures/release/shard-readiness-v154.json",
                "fixtures/release/shard-readiness-v155.json",
            }) +
           ",\"runtimeMatchesCurrentFixture\":true,\"historicalFixturesPreserved\":true}";
}

std::string format_archive_compatibility_json() {
    return "{\"preservesNodeArchivedEvidence\":true,"
           "\"archivedNodeVersions\":" +
           json_string_array({
               "Node v370",
               "Node v371",
               "Node v372",
               "Node v373",
               "Node v374",
               "Node v375",
               "Node v376",
               "Node v377",
               "Node v378",
               "Node v379",
               "Node v380",
               "Node v381",
               "Node v382",
               "Node v383",
                "Node v384",
                "Node v385",
                "Node v386",
                "Node v387",
                "Node v388",
                "Node v389",
                "Node v390",
                "Node v391",
                "Node v392",
                "Node v393",
                "Node v394",
                "Node v395",
                "Node v396",
                "Node v397",
                "Node v398",
                "Node v399",
                "Node v400",
            }) +
            ",\"changesArchivedNodeEvidence\":false,"
            "\"futureNodeConsumer\":\"Node v401 or later only after final mini-kv input, Node window, correlated approval, and cross-project packet exist together\"}";
}

std::string format_historical_fallback_json() {
    return "{\"previousConsumedReleaseVersion\":\"v155\","
           "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v155.json\","
           "\"previousConsumptionNodeVersion\":\"Node v400 consumed v155 as precheck-only approval input contract evidence\","
           "\"olderPrototypeFixturePath\":\"fixtures/release/shard-readiness-v144.json\","
           "\"rollingCurrentUsedForHistoricalBaseline\":false,"
           "\"nodeV396ProgressIntakePreserved\":true,"
           "\"nodeV397ContributionReviewPreserved\":true,"
           "\"nodeV398ApprovalGateReviewPreserved\":true,"
           "\"nodeV399ArchiveVerificationPreserved\":true,"
           "\"nodeV400InputContractPreserved\":true,"
           "\"nodeV401WaitsForCompleteApprovalInputs\":true}";
}

std::string format_active_prototype_plan_json() {
    return "{\"planMode\":\"prerequisite-only\","
           "\"activeShardPrototypeAllowed\":false,"
           "\"routerActivationAllowed\":false,"
           "\"shardDirectoryCreationAllowed\":false,"
           "\"multiProcessStartAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"requiredBeforeActivation\":" + json_string_array({
               "separate active shard prototype plan",
               "explicit service startup and cleanup responsibility",
               "read-only Java echo",
               "Node consumer gate after frozen mini-kv evidence",
               "fail-closed smoke target",
           }) +
           "}";
}

std::string format_active_prototype_plan_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesActivePrototypePlan\":true,"
           "\"frozenActiveShardPrototypeAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenShardDirectoryCreationAllowed\":false,"
           "\"frozenMultiProcessStartAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_consumer_handoff_json() {
    return "{\"handoffMode\":\"frozen-evidence-only\","
           "\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"readyForNodeConsumption\":true,"
           "\"liveReadGateRequiredBeforeRuntimeProbe\":true,"
           "\"startsServices\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredConsumerChecks\":" + json_string_array({
               "read frozen fixture instead of rolling current",
               "verify releaseVersion and status",
               "verify readOnly true and executionAllowed false",
               "verify no router and no write routing",
               "fail closed on missing frozen evidence",
           }) +
           "}";
}

std::string format_live_read_gate_plan_json() {
    return "{\"planMode\":\"service-lifecycle-prerequisite-only\","
           "\"liveReadGateAllowed\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"startsServices\":false,"
           "\"requiresServiceOwner\":true,"
           "\"requiresPortList\":true,"
           "\"requiresSmokeTarget\":true,"
           "\"requiresFailClosedBehavior\":true,"
           "\"requiresCleanup\":true,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredBeforeLiveRead\":" + json_string_array({
               "explicit live-read gate plan",
               "service owner and startup command",
               "port list and conflict behavior",
               "read-only smoke target",
               "fail-closed missing evidence behavior",
               "cleanup responsibility and stop command",
           }) +
           "}";
}

std::string format_live_read_gate_plan_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesLiveReadGatePlan\":true,"
           "\"frozenLiveReadGateAllowed\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenStartsServices\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_operator_service_lifecycle_template_json() {
    return "{\"evidenceMode\":\"template-only-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v153\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"operatorOwnedServiceLifecycleRequired\":true,"
           "\"serviceOwnerDeclared\":false,"
           "\"startupCommandDeclared\":false,"
           "\"portListDeclared\":false,"
           "\"getOnlySmokeTargetDeclared\":false,"
           "\"failClosedBehaviorRequired\":true,"
           "\"cleanupResponsibilityDeclared\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredOperatorEvidence\":" + json_string_array({
               "service owner",
               "startup command",
               "port list",
               "GET-only smoke target",
               "fail-closed missing evidence behavior",
               "cleanup responsibility and stop command",
           }) +
           "}";
}

std::string format_operator_service_lifecycle_template_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesOperatorLifecycleTemplate\":true,"
           "\"frozenServiceOwnerDeclared\":false,"
           "\"frozenStartupCommandDeclared\":false,"
           "\"frozenPortListDeclared\":false,"
           "\"frozenGetOnlySmokeTargetDeclared\":false,"
           "\"frozenCleanupResponsibilityDeclared\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_operator_service_lifecycle_evidence_json() {
    return "{\"evidenceMode\":\"declared-lifecycle-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v153\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"operatorOwnedServiceLifecycleDeclared\":true,"
           "\"serviceOwnerDeclared\":true,"
           "\"serviceOwnerHandle\":\"mini-kv-local-operator\","
           "\"startupCommandDeclared\":true,"
           "\"startupCommand\":\"minikv_server.exe <operator-approved-loopback-port> 127.0.0.1\","
           "\"portListDeclared\":true,"
           "\"declaredPortHandles\":" + json_string_array({
               "operator-approved-loopback-port",
           }) +
           ",\"getOnlySmokeTargetDeclared\":true,"
           "\"getOnlySmokeTarget\":\"GET-only SHARDJSON evidence read\","
           "\"failClosedBehaviorDeclared\":true,"
           "\"failClosedBehavior\":\"missing evidence or port conflict blocks runtime gate\","
           "\"cleanupResponsibilityDeclared\":true,"
           "\"cleanupResponsibility\":\"operator stops service and verifies no LISTENING port\","
           "\"runtimeGateApproved\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiresSeparateRuntimeGate\":true,"
           "\"requiredBeforeRuntimeGate\":" + json_string_array({
               "operator approval record",
               "concrete loopback port assignment",
               "GET-only smoke command",
               "cleanup proof",
           }) +
           "}";
}

std::string format_operator_service_lifecycle_evidence_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesDeclaredLifecycleEvidence\":true,"
           "\"frozenOperatorOwnedServiceLifecycleDeclared\":true,"
           "\"frozenServiceOwnerDeclared\":true,"
           "\"frozenStartupCommandDeclared\":true,"
           "\"frozenPortListDeclared\":true,"
           "\"frozenGetOnlySmokeTargetDeclared\":true,"
           "\"frozenFailClosedBehaviorDeclared\":true,"
           "\"frozenCleanupResponsibilityDeclared\":true,"
           "\"frozenRuntimeGateApproved\":false,"
           "\"frozenStartsServices\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

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

std::string evidence_digest() {
    return runtime_evidence::digest(
        "mini-kv-shard-readiness-v156",
        {
            {std::string{contract_version}},
            {std::string{version}},
            {std::string{release_version}},
            {"shardEnabled=false"},
            {"shardCount=1"},
            {"slotCount=16"},
            {"routingMode=single-shard-readiness-prototype"},
            {fixture_path()},
            {"commandCatalog=read-no-mutate-no-wal"},
            {"fixtureParity=runtime-matches-current-fixture"},
            {"historicalFallback=v155-frozen-no-rolling-current"},
            {"archivedNodeEvidence=v370-v400-preserved"},
            {"activePrototypePlan=prerequisite-only-no-activation"},
            {"activePrototypePlanFreeze=v153-frozen-no-router-no-write"},
            {"consumerHandoff=frozen-evidence-only-no-live-read"},
            {"liveReadGatePlan=prerequisite-only-no-start-no-execution"},
            {"liveReadGatePlanFreeze=v153-frozen-no-runtime-probe"},
            {"operatorServiceLifecycleTemplate=template-only-no-runtime"},
            {"operatorServiceLifecycleTemplateFreeze=v153-frozen-template-only"},
            {"operatorServiceLifecycleEvidence=declared-no-runtime-gate"},
            {"operatorServiceLifecycleEvidenceFreeze=v153-frozen-declared-no-runtime"},
            {"runtimeExecutionArtifactIntakePreflight=blocked-0-of-6-no-execution"},
            {"runtimeExecutionArtifactIntakePreflightFreeze=v154-frozen-candidate-0-of-6"},
            {"miniKvRuntimeExecutionArtifactCandidate=candidate-no-runtime-no-approval"},
            {"miniKvRuntimeExecutionArtifactCandidateFreeze=v154-frozen-candidate-only"},
            {"runtimeExecutionApprovalGateInputPrecheck=blocked-0-of-3-no-execution"},
            {"runtimeExecutionApprovalGateInputPrecheckFreeze=v155-frozen-0-of-3"},
            {"miniKvFinalApprovalGateInput=complete-mini-kv-input-global-runtime-blocked"},
        });
}

} // namespace

std::string fixture_path() {
    return "fixtures/release/shard-readiness.json";
}

std::string format_json() {
    return "{\"contract\":" + json_string(contract_version) +
           ",\"evidenceType\":\"shard_readiness\"" +
           ",\"project\":\"mini-kv\"" +
           ",\"version\":" + json_string(version) +
           ",\"releaseVersion\":" + json_string(release_version) +
           ",\"readOnly\":true" +
           ",\"executionAllowed\":false" +
           ",\"shardEnabled\":false" +
           ",\"shardCount\":1" +
           ",\"slotCount\":" + std::to_string(slot_count) +
           ",\"routingMode\":\"single-shard-readiness-prototype\"" +
           ",\"evidencePath\":" + json_string(fixture_path()) +
           ",\"status\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"" +
           ",\"shardMap\":" + format_shard_map_json() +
           ",\"keyRoutingSamples\":" + format_route_samples_json() +
           ",\"boundaries\":" + format_boundaries_json() +
           ",\"diagnostics\":" + format_diagnostics_json() +
           ",\"commandCatalog\":" + format_command_catalog_json() +
           ",\"fixtureParity\":" + format_fixture_parity_json() +
           ",\"archiveCompatibility\":" + format_archive_compatibility_json() +
           ",\"historicalFallback\":" + format_historical_fallback_json() +
           ",\"activePrototypePlan\":" + format_active_prototype_plan_json() +
           ",\"activePrototypePlanFreeze\":" + format_active_prototype_plan_freeze_json() +
           ",\"consumerHandoff\":" + format_consumer_handoff_json() +
           ",\"liveReadGatePlan\":" + format_live_read_gate_plan_json() +
           ",\"liveReadGatePlanFreeze\":" + format_live_read_gate_plan_freeze_json() +
           ",\"operatorServiceLifecycleTemplate\":" + format_operator_service_lifecycle_template_json() +
           ",\"operatorServiceLifecycleTemplateFreeze\":" + format_operator_service_lifecycle_template_freeze_json() +
           ",\"operatorServiceLifecycleEvidence\":" + format_operator_service_lifecycle_evidence_json() +
           ",\"operatorServiceLifecycleEvidenceFreeze\":" + format_operator_service_lifecycle_evidence_freeze_json() +
           ",\"runtimeExecutionArtifactIntakePreflight\":" +
           format_runtime_execution_artifact_intake_preflight_json() +
           ",\"runtimeExecutionArtifactIntakePreflightFreeze\":" +
           format_runtime_execution_artifact_intake_preflight_freeze_json() +
           ",\"miniKvRuntimeExecutionArtifactCandidate\":" +
           format_mini_kv_runtime_execution_artifact_candidate_json() +
           ",\"miniKvRuntimeExecutionArtifactCandidateFreeze\":" +
           format_mini_kv_runtime_execution_artifact_candidate_freeze_json() +
           ",\"runtimeExecutionApprovalGateInputPrecheck\":" +
           format_runtime_execution_approval_gate_input_precheck_json() +
           ",\"runtimeExecutionApprovalGateInputPrecheckFreeze\":" +
           format_runtime_execution_approval_gate_input_precheck_freeze_json() +
           ",\"miniKvFinalApprovalGateInput\":" +
           format_mini_kv_final_approval_gate_input_json() +
           ",\"readOnlyBoundaryFields\":" + json_string_array({
               "readOnly",
               "executionAllowed",
               "boundaries.writeCommandsAllowed",
               "boundaries.adminCommandsAllowed",
               "boundaries.loadRestoreCompactAllowed",
               "boundaries.archivedNodeEvidenceMutated",
               "archiveCompatibility.changesArchivedNodeEvidence",
               "historicalFallback.rollingCurrentUsedForHistoricalBaseline",
               "activePrototypePlan.activeShardPrototypeAllowed",
               "activePrototypePlan.routerActivationAllowed",
               "activePrototypePlan.shardDirectoryCreationAllowed",
               "activePrototypePlan.writeRoutingAllowed",
               "activePrototypePlanFreeze.rollingCurrentUsedForFrozenBaseline",
               "activePrototypePlanFreeze.frozenRouterActivationAllowed",
               "activePrototypePlanFreeze.frozenWriteRoutingAllowed",
               "consumerHandoff.startsServices",
               "consumerHandoff.routerActivationAllowed",
               "consumerHandoff.writeRoutingAllowed",
               "consumerHandoff.executionAllowed",
               "liveReadGatePlan.liveReadGateAllowed",
               "liveReadGatePlan.runtimeProbeAllowed",
               "liveReadGatePlan.startsServices",
               "liveReadGatePlan.routerActivationAllowed",
               "liveReadGatePlan.writeRoutingAllowed",
               "liveReadGatePlan.executionAllowed",
               "liveReadGatePlanFreeze.rollingCurrentUsedForFrozenBaseline",
               "liveReadGatePlanFreeze.frozenRuntimeProbeAllowed",
               "liveReadGatePlanFreeze.frozenStartsServices",
               "liveReadGatePlanFreeze.frozenExecutionAllowed",
               "operatorServiceLifecycleTemplate.serviceOwnerDeclared",
               "operatorServiceLifecycleTemplate.startupCommandDeclared",
               "operatorServiceLifecycleTemplate.portListDeclared",
               "operatorServiceLifecycleTemplate.getOnlySmokeTargetDeclared",
               "operatorServiceLifecycleTemplate.startsServices",
               "operatorServiceLifecycleTemplate.runtimeProbeAllowed",
               "operatorServiceLifecycleTemplate.liveReadAllowed",
               "operatorServiceLifecycleTemplate.executionAllowed",
               "operatorServiceLifecycleTemplateFreeze.rollingCurrentUsedForFrozenBaseline",
               "operatorServiceLifecycleTemplateFreeze.frozenRuntimeProbeAllowed",
               "operatorServiceLifecycleTemplateFreeze.frozenExecutionAllowed",
               "operatorServiceLifecycleEvidence.runtimeGateApproved",
               "operatorServiceLifecycleEvidence.startsServices",
                "operatorServiceLifecycleEvidence.runtimeProbeAllowed",
                "operatorServiceLifecycleEvidence.liveReadAllowed",
                "operatorServiceLifecycleEvidence.routerActivationAllowed",
                "operatorServiceLifecycleEvidence.writeRoutingAllowed",
                "operatorServiceLifecycleEvidence.executionAllowed",
                "operatorServiceLifecycleEvidenceFreeze.rollingCurrentUsedForFrozenBaseline",
                "operatorServiceLifecycleEvidenceFreeze.frozenRuntimeGateApproved",
                "operatorServiceLifecycleEvidenceFreeze.frozenStartsServices",
                "operatorServiceLifecycleEvidenceFreeze.frozenRuntimeProbeAllowed",
                "operatorServiceLifecycleEvidenceFreeze.frozenLiveReadAllowed",
                "operatorServiceLifecycleEvidenceFreeze.frozenRouterActivationAllowed",
                "operatorServiceLifecycleEvidenceFreeze.frozenWriteRoutingAllowed",
                "operatorServiceLifecycleEvidenceFreeze.frozenExecutionAllowed",
                "runtimeExecutionArtifactIntakePreflight.readyForRuntimeExecutionPacket",
                "runtimeExecutionArtifactIntakePreflight.readyForRuntimeLiveReadGate",
                "runtimeExecutionArtifactIntakePreflight.runtimeExecutionArtifactsComplete",
                "runtimeExecutionArtifactIntakePreflight.runtimeExecutionPacketPresent",
                "runtimeExecutionArtifactIntakePreflight.runtimeExecutionPacketExecutable",
                "runtimeExecutionArtifactIntakePreflight.executionAttempted",
                "runtimeExecutionArtifactIntakePreflight.startsMiniKvService",
                "runtimeExecutionArtifactIntakePreflight.startsServices",
                "runtimeExecutionArtifactIntakePreflight.runtimeProbeAllowed",
                "runtimeExecutionArtifactIntakePreflight.liveReadAllowed",
                "runtimeExecutionArtifactIntakePreflight.routerActivationAllowed",
                "runtimeExecutionArtifactIntakePreflight.writeRoutingAllowed",
                "runtimeExecutionArtifactIntakePreflight.executionAllowed",
                "runtimeExecutionArtifactIntakePreflightFreeze.rollingCurrentUsedForFrozenBaseline",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenRuntimeExecutionArtifactsComplete",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenRuntimeExecutionPacketExecutable",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenExecutionAttempted",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenStartsMiniKvService",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenRuntimeProbeAllowed",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenLiveReadAllowed",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenRouterActivationAllowed",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenWriteRoutingAllowed",
                "runtimeExecutionArtifactIntakePreflightFreeze.frozenExecutionAllowed",
                "miniKvRuntimeExecutionArtifactCandidate.acceptedRuntimeExecutionArtifactsComplete",
                "miniKvRuntimeExecutionArtifactCandidate.operatorApprovalRecordPresent",
                "miniKvRuntimeExecutionArtifactCandidate.nodeRuntimeWindowApproved",
                "miniKvRuntimeExecutionArtifactCandidate.crossProjectRuntimeExecutionPacketPresent",
                "miniKvRuntimeExecutionArtifactCandidate.javaPortBindingPresent",
                "miniKvRuntimeExecutionArtifactCandidate.miniKvLoopbackPortOperatorApproved",
                "miniKvRuntimeExecutionArtifactCandidate.getOnlySmokeCommandOperatorApproved",
                "miniKvRuntimeExecutionArtifactCandidate.serviceOwnerOperatorConfirmed",
                "miniKvRuntimeExecutionArtifactCandidate.cleanupProofPresent",
                "miniKvRuntimeExecutionArtifactCandidate.runtimeExecutionPacketExecutable",
                "miniKvRuntimeExecutionArtifactCandidate.startsMiniKvService",
                "miniKvRuntimeExecutionArtifactCandidate.startsServices",
                "miniKvRuntimeExecutionArtifactCandidate.runtimeProbeAllowed",
                "miniKvRuntimeExecutionArtifactCandidate.liveReadAllowed",
                "miniKvRuntimeExecutionArtifactCandidate.routerActivationAllowed",
                "miniKvRuntimeExecutionArtifactCandidate.writeRoutingAllowed",
                "miniKvRuntimeExecutionArtifactCandidate.executionAllowed",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.rollingCurrentUsedForFrozenBaseline",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenAcceptedRuntimeExecutionArtifactsComplete",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenRuntimeExecutionPacketExecutable",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenStartsMiniKvService",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenRuntimeProbeAllowed",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenLiveReadAllowed",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenRouterActivationAllowed",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenWriteRoutingAllowed",
                "miniKvRuntimeExecutionArtifactCandidateFreeze.frozenExecutionAllowed",
                "runtimeExecutionApprovalGateInputPrecheck.nodeApprovedRuntimeWindowPresent",
                "runtimeExecutionApprovalGateInputPrecheck.correlatedOperatorApprovalRecordPresent",
                "runtimeExecutionApprovalGateInputPrecheck.completeCrossProjectRuntimeExecutionPacketPresent",
                "runtimeExecutionApprovalGateInputPrecheck.approvalGateInputsComplete",
                "runtimeExecutionApprovalGateInputPrecheck.runtimeGateApprovalPresent",
                "runtimeExecutionApprovalGateInputPrecheck.readyForRuntimeExecutionPacket",
                "runtimeExecutionApprovalGateInputPrecheck.readyForRuntimeLiveReadGate",
                "runtimeExecutionApprovalGateInputPrecheck.runtimeExecutionArtifactsComplete",
                "runtimeExecutionApprovalGateInputPrecheck.runtimeExecutionPacketPresent",
                "runtimeExecutionApprovalGateInputPrecheck.runtimeExecutionPacketExecutable",
                "runtimeExecutionApprovalGateInputPrecheck.concreteLoopbackPortsAssigned",
                "runtimeExecutionApprovalGateInputPrecheck.executionAttempted",
                "runtimeExecutionApprovalGateInputPrecheck.startsJavaService",
                "runtimeExecutionApprovalGateInputPrecheck.startsMiniKvService",
                "runtimeExecutionApprovalGateInputPrecheck.startsServices",
                "runtimeExecutionApprovalGateInputPrecheck.runtimeProbeAllowed",
                "runtimeExecutionApprovalGateInputPrecheck.liveReadAllowed",
                "runtimeExecutionApprovalGateInputPrecheck.routerActivationAllowed",
                "runtimeExecutionApprovalGateInputPrecheck.writeRoutingAllowed",
                "runtimeExecutionApprovalGateInputPrecheck.executionAllowed",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.rollingCurrentUsedForFrozenBaseline",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenApprovalGateInputsComplete",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenRuntimeGateApprovalPresent",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenRuntimeExecutionPacketExecutable",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenConcreteLoopbackPortsAssigned",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenExecutionAttempted",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenStartsJavaService",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenStartsMiniKvService",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenRuntimeProbeAllowed",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenLiveReadAllowed",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenRouterActivationAllowed",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenWriteRoutingAllowed",
                "runtimeExecutionApprovalGateInputPrecheckFreeze.frozenExecutionAllowed",
                "miniKvFinalApprovalGateInput.cleanupProofPresent",
                "miniKvFinalApprovalGateInput.nodeApprovedRuntimeWindowPresent",
                "miniKvFinalApprovalGateInput.correlatedOperatorApprovalRecordPresent",
                "miniKvFinalApprovalGateInput.completeCrossProjectRuntimeExecutionPacketPresent",
                "miniKvFinalApprovalGateInput.runtimeGateApprovalPresent",
                "miniKvFinalApprovalGateInput.readyForRuntimeExecutionPacket",
                "miniKvFinalApprovalGateInput.readyForRuntimeLiveReadGate",
                "miniKvFinalApprovalGateInput.runtimeExecutionPacketExecutable",
                "miniKvFinalApprovalGateInput.concreteLoopbackPortsAssignedByCrossProjectPacket",
                "miniKvFinalApprovalGateInput.executionAttempted",
                "miniKvFinalApprovalGateInput.startsJavaService",
                "miniKvFinalApprovalGateInput.startsMiniKvService",
                "miniKvFinalApprovalGateInput.startsServices",
                "miniKvFinalApprovalGateInput.runtimeProbeAllowed",
                "miniKvFinalApprovalGateInput.liveReadAllowed",
                "miniKvFinalApprovalGateInput.routerActivationAllowed",
                "miniKvFinalApprovalGateInput.writeRoutingAllowed",
                "miniKvFinalApprovalGateInput.executionAllowed",
            }) +
           ",\"evidenceDigest\":" + json_string(evidence_digest()) +
           ",\"notes\":" + json_string_array({
               "read-only shard readiness hardening",
               "single logical shard only",
               "slot table is evidence, not active storage routing",
                "does not create shard directories or start extra processes",
                "freezes v155 approval gate input precheck",
                "declares operator lifecycle evidence without runtime gate approval",
                "adds mini-kv runtime execution artifact candidate without approval",
                "adds runtime execution approval gate input precheck with zero of three inputs",
                "adds final mini-kv approval gate input while global runtime remains blocked",
                "runtime execution artifact intake preflight remains blocked at 0 of 6 artifacts",
                "live-read gate remains prerequisite-only and does not start services",
                "operator service lifecycle evidence still has no runtime probe",
                "active shard prototype remains plan-prerequisite only",
                 "does not mutate Node v370-v400 archived evidence",
               "not order or audit authoritative",
           }) +
           "}";
}

} // namespace minikv::shard_readiness
