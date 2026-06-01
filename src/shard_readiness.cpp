#include "minikv/shard_readiness.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_readiness_approval_inputs.hpp"
#include "minikv/shard_readiness_boundary_fields.hpp"
#include "minikv/shard_readiness_history.hpp"
#include "minikv/shard_readiness_node_compatibility.hpp"
#include "minikv/shard_readiness_release_catalog.hpp"
#include "minikv/shard_readiness_route_catalog_cleanup_catalog.hpp"
#include "minikv/shard_readiness_route_catalog_evidence_chain.hpp"
#include "minikv/shard_readiness_route_catalog_post_closeout.hpp"
#include "minikv/shard_readiness_route_catalog_closeout.hpp"
#include "minikv/shard_readiness_slot_preview.hpp"
#include "minikv/shard_readiness_slot_preview_audit.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness {
namespace {

constexpr std::string_view contract_version = "shard-readiness.v1";
constexpr std::string_view release_version = "v203";

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string format_boundaries_json() {
    return "{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false,"
           "\"activeRouterInstalled\":false,\"archivedNodeEvidenceMutated\":false}";
}

std::string format_command_catalog_json() {
    return "{\"command\":\"SHARDJSON\",\"category\":\"read\",\"mutatesStore\":false,"
           "\"touchesWal\":false,\"stable\":true,\"extraArgsAllowed\":false,"
           "\"sideEffects\":[\"metadata_read\"]}";
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
        "mini-kv-shard-readiness-v203",
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
            {"historicalFallback=v202-frozen-no-rolling-current"},
            {"archivedNodeEvidence=v370-v480-preserved"},
            {"shardReadinessHistoryMaintenance=v180-formatter-split-no-execution"},
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
            {"miniKvFinalApprovalGateInputFreeze=v156-frozen-final-input-no-runtime"},
            {"runtimeExecutionApprovalInputTemplateValidatorEcho=v402-template-only-no-canonical-inputs"},
            {"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze=v157-frozen-template-only"},
            {"runtimeExecutionCanonicalApprovalInputPrecheck=blocked-0-of-3-no-execution"},
            {"nodeRouteGroupSplitCompatibility=v418-route-refactor-contract-stable"},
            {"boundaryCatalogMaintenance=v160-contract-preserving-split-no-execution"},
            {"boundaryCatalogIndex=v203-v29-route-catalog-cleanup-post-closeout-continuity-no-execution"},
            {"slotTablePreview=v162-read-only-no-router-no-write-no-execution"},
            {"slotTablePreviewAudit=v163-contiguous-no-duplicates-no-router-no-execution"},
            {"slotTablePreviewAuditMaintenance=v164-formatter-split-contract-preserving"},
            {"shardReadinessReleaseCatalog=v165-versioned-catalog-no-execution"},
            {"shardReadinessReleaseCatalogAudit=v166-catalog-consistency-no-execution"},
            {"nodeRouteSplitCompatibilityWindow=v203-node-v433-v472-route-catalog-cleanup-closeout-no-execution"},
            {"nodeRouteSplitCompatibilityWindowAudit=v203-window-span-count-parse-audit-no-execution"},
            {"nodeRouteCatalogCloseoutSnapshot=v185-node-v464-validation-snapshot-no-execution"},
            {"nodeRouteCatalogAnchorRemovalAudit=v186-node-v467-anchor-removal-no-execution"},
            {"nodeRouteCatalogExpectedIntegritySnapshotAudit=v187-node-v471-integrity-snapshot-no-execution"},
            {"nodeRouteCatalogExpectedIntegritySnapshotFreeze=v188-v187-frozen-no-rolling-current"},
            {"nodeRouteCatalogEvidenceChain=v189-v185-v188-versioned-catalog-no-execution"},
            {"nodeRouteCatalogEvidenceChainAudit=v190-node-v472-closeout-validation-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoff=v191-v190-handoff-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoffAudit=v192-v191-handoff-audit-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze=v193-v192-handoff-audit-freeze-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutReleaseCatalog=v194-v190-v193-release-catalog-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit=v195-v194-release-catalog-audit-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutCatalogMaintenance=v196-formatter-split-no-execution"},
            {"nodeRouteCatalogCleanupLatestEvidencePackage=v197-v194-v196-package-no-execution"},
            {"nodeRouteCatalogCleanupLatestEvidencePackageAudit=v198-v197-package-audit-no-execution"},
            {"nodeRouteCatalogCleanupEvidenceBatchCloseout=v199-v194-v198-closeout-no-execution"},
            {"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit=v200-v199-closeout-audit-no-execution"},
            {"nodeRouteCatalogCleanupPostCloseoutContinuity=v203-v202-archive-readiness-snapshot-no-execution"},
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
           ",\"slotCount\":" + std::to_string(slot_preview::slot_count()) +
           ",\"routingMode\":\"single-shard-readiness-prototype\"" +
           ",\"evidencePath\":" + json_string(fixture_path()) +
           ",\"status\":\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"" +
           ",\"shardMap\":" + slot_preview::format_shard_map_json() +
           ",\"keyRoutingSamples\":" + slot_preview::format_route_samples_json() +
           ",\"slotTablePreview\":" + slot_preview::format_slot_table_preview_json() +
           ",\"slotTablePreviewAudit\":" + slot_preview_audit::format_slot_table_preview_audit_json() +
           ",\"slotTablePreviewAuditMaintenance\":" +
           slot_preview_audit::format_slot_table_preview_audit_maintenance_json() +
           ",\"shardReadinessReleaseCatalog\":" +
           release_catalog::format_release_catalog_json() +
           ",\"shardReadinessReleaseCatalogAudit\":" +
           release_catalog::format_release_catalog_audit_json() +
           ",\"boundaries\":" + format_boundaries_json() +
           ",\"diagnostics\":" + history::format_diagnostics_json() +
           ",\"commandCatalog\":" + format_command_catalog_json() +
           ",\"fixtureParity\":" + history::format_fixture_parity_json() +
           ",\"archiveCompatibility\":" + history::format_archive_compatibility_json() +
           ",\"historicalFallback\":" + history::format_historical_fallback_json() +
           ",\"shardReadinessHistoryMaintenance\":" +
           history::format_history_formatter_maintenance_json() +
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
           ",\"miniKvFinalApprovalGateInputFreeze\":" +
           approval_inputs::format_final_approval_gate_input_freeze_json() +
           ",\"runtimeExecutionApprovalInputTemplateValidatorEcho\":" +
           approval_inputs::format_template_validator_echo_json() +
           ",\"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze\":" +
           approval_inputs::format_template_validator_echo_freeze_json() +
           ",\"runtimeExecutionCanonicalApprovalInputPrecheck\":" +
           approval_inputs::format_canonical_approval_input_precheck_json() +
           ",\"nodeRouteGroupSplitCompatibility\":" +
           node_compatibility::format_route_group_split_compatibility_json() +
           ",\"nodeRouteSplitCompatibilityWindow\":" +
           node_compatibility::format_route_split_compatibility_window_json() +
           ",\"nodeRouteSplitCompatibilityWindowAudit\":" +
           node_compatibility::format_route_split_compatibility_window_audit_json() +
           ",\"nodeRouteCatalogCloseoutSnapshot\":" +
           route_catalog_closeout::format_closeout_snapshot_json() +
           ",\"nodeRouteCatalogAnchorRemovalAudit\":" +
           route_catalog_closeout::format_anchor_removal_audit_json() +
           ",\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":" +
           route_catalog_closeout::format_expected_integrity_snapshot_audit_json() +
           ",\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":" +
           route_catalog_closeout::format_expected_integrity_snapshot_freeze_json() +
           ",\"nodeRouteCatalogEvidenceChain\":" +
           route_catalog_evidence_chain::format_route_catalog_evidence_chain_json() +
           ",\"nodeRouteCatalogEvidenceChainAudit\":" +
           route_catalog_evidence_chain::format_route_catalog_evidence_chain_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoff\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_audit_freeze_json() +
           ",\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_release_catalog_json() +
           ",\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_release_catalog_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_catalog_maintenance_json() +
           ",\"nodeRouteCatalogCleanupLatestEvidencePackage\":" +
           route_catalog_cleanup_catalog::format_latest_route_catalog_cleanup_evidence_package_json() +
           ",\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":" +
           route_catalog_cleanup_catalog::format_latest_route_catalog_cleanup_evidence_package_audit_json() +
           ",\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_evidence_batch_closeout_json() +
           ",\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_evidence_batch_closeout_audit_json() +
           ",\"nodeRouteCatalogCleanupPostCloseoutContinuity\":" +
           route_catalog_post_closeout::format_post_closeout_continuity_json() +
           ",\"boundaryCatalogMaintenance\":" +
           boundary_fields::format_catalog_maintenance_json() +
           ",\"boundaryCatalogIndex\":" +
           boundary_fields::format_catalog_index_json() +
           ",\"readOnlyBoundaryFields\":" +
           boundary_fields::format_read_only_boundary_fields_json() +
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
                 "freezes v156 final mini-kv approval gate input",
                 "echoes Node v402 templates as non-canonical approval inputs",
                 "freezes v157 template validator echo",
                 "adds canonical approval input precheck with zero of three inputs",
                 "adds Node v418 route-group split compatibility evidence",
                 "splits read-only boundary field catalog without changing SHARDJSON command or fixture path",
                 "extends boundary catalog index to v10 for numeric route window audit fields without changing runtime permissions",
                 "adds read-only slot table preview without installing an active router",
                 "adds read-only slot table preview consistency audit without activating routing",
                 "splits slot preview audit formatter without changing command or runtime boundaries",
                 "adds versioned shard readiness release catalog without runtime permissions",
                 "adds release catalog consistency audit without runtime permissions",
                 "keeps Node route split compatibility window through v464 without runtime permissions",
                 "snapshots Node v464 route catalog final closeout validation without consuming fresh mini-kv evidence",
                 "audits Node v467 route catalog anchor removal without adding mini-kv route-anchor dependency",
                  "audits Node v471 expected integrity snapshot centralization without making mini-kv a catalog integrity authority",
                  "freezes v187 expected integrity snapshot audit evidence without using rolling current as the baseline",
                  "catalogs v185-v188 route catalog evidence chain from versioned fixtures without runtime permissions",
                  "audits Node v472 route catalog cleanup closeout validation without adding routes or runtime gates",
                  "hands off the v190 cleanup closeout audit through a versioned read-only fixture",
                  "audits the v191 cleanup closeout handoff manifest without opening runtime behavior",
                  "freezes the v192 cleanup closeout handoff audit as versioned read-only evidence",
                  "catalogs v190-v193 cleanup closeout releases from versioned fixtures without runtime permissions",
                  "audits the v194 cleanup closeout release catalog for continuity and monotonic evidence counts",
                  "splits cleanup closeout catalog formatters without changing catalog or audit sections",
                  "packages v194-v196 latest route catalog cleanup evidence for archive verification",
                  "audits the v197 latest route catalog cleanup evidence package from a frozen fixture",
                  "closes out v194-v198 cleanup evidence as a read-only batch aligned with Node v480",
                  "audits the v199 cleanup evidence batch closeout from a frozen fixture",
                  "starts a post-closeout continuity ledger from the frozen v200 audit fixture",
                 "computes Node route split window contiguity and duplicate checks from the version vector without runtime permissions",
                 "computes Node route split window numeric span and count matching without runtime permissions",
                 "splits shard readiness history formatters without changing command or runtime boundaries",
                "runtime execution artifact intake preflight remains blocked at 0 of 6 artifacts",
                "live-read gate remains prerequisite-only and does not start services",
                "operator service lifecycle evidence still has no runtime probe",
                "active shard prototype remains plan-prerequisite only",
                  "does not mutate Node v370-v480 archived evidence",
               "not order or audit authoritative",
           }) +
           "}";
}

} // namespace minikv::shard_readiness
