#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_shard_readiness_contract(const std::string& json) {
    assert_contains(json, "\"contract\":\"shard-readiness.v1\"");
    assert_contains(json, "\"evidenceType\":\"shard_readiness\"");
    assert_contains(json, "\"project\":\"mini-kv\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(json, "\"releaseVersion\":\"v157\"");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardEnabled\":false");
    assert_contains(json, "\"shardCount\":1");
    assert_contains(json, "\"slotCount\":16");
    assert_contains(json, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_contains(json, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"status\":\"runtime-execution-approval-input-template-validator-echo-read-only\"");
    assert_contains(json, "\"shardId\":\"shard-0\"");
    assert_contains(json, "\"storagePath\":\"not-created\"");
    assert_contains(json, "\"writesAllowed\":false");
    assert_contains(json, "\"key\":\"orderops:alpha\",\"slot\":5,\"shardId\":\"shard-0\"");
    assert_contains(json, "\"key\":\"audit:receipt:001\",\"slot\":9,\"shardId\":\"shard-0\"");
    assert_contains(json, "\"orderAuthoritative\":false");
    assert_contains(json, "\"auditAuthoritative\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"setnxexExecutionAllowed\":false");
    assert_contains(json, "\"multiProcessStarted\":false");
    assert_contains(json, "\"storageDirectoriesCreated\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"archivedNodeEvidenceMutated\":false");
    assert_contains(json, "\"writeCommandsExecuted\":false");
    assert_contains(json, "\"adminCommandsExecuted\":false");
    assert_contains(json, "\"loadRestoreCompactExecuted\":false");
    assert_contains(json,
                    "\"nodeConsumer\":\"Node v402+ may consume v157 only as approval input template validator echo evidence\"");
    assert_contains(json, "\"nodeArchivedEvidencePreserved\":true");
    assert_contains(json, "\"commandCatalog\":{\"command\":\"SHARDJSON\",\"category\":\"read\"");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"extraArgsAllowed\":false");
    assert_contains(json, "\"sideEffects\":[\"metadata_read\"]");
    assert_contains(json, "\"fixtureParity\":{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"historicalFixturePaths\":[\"fixtures/release/shard-readiness-v144.json\","
                          "\"fixtures/release/shard-readiness-v145.json\","
                          "\"fixtures/release/shard-readiness-v146.json\","
                          "\"fixtures/release/shard-readiness-v147.json\","
                          "\"fixtures/release/shard-readiness-v148.json\","
                          "\"fixtures/release/shard-readiness-v149.json\","
                          "\"fixtures/release/shard-readiness-v150.json\","
                          "\"fixtures/release/shard-readiness-v151.json\","
                          "\"fixtures/release/shard-readiness-v152.json\","
                          "\"fixtures/release/shard-readiness-v153.json\","
                          "\"fixtures/release/shard-readiness-v154.json\","
                          "\"fixtures/release/shard-readiness-v155.json\","
                          "\"fixtures/release/shard-readiness-v156.json\"]");
    assert_contains(json, "\"runtimeMatchesCurrentFixture\":true");
    assert_contains(json, "\"historicalFixturesPreserved\":true");
    assert_contains(json, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(json, "\"archivedNodeVersions\":[\"Node v370\",\"Node v371\",\"Node v372\",\"Node v373\","
                          "\"Node v374\",\"Node v375\",\"Node v376\",\"Node v377\",\"Node v378\",\"Node v379\","
                          "\"Node v380\",\"Node v381\",\"Node v382\",\"Node v383\",\"Node v384\",\"Node v385\","
                          "\"Node v386\",\"Node v387\",\"Node v388\",\"Node v389\",\"Node v390\",\"Node v391\","
                          "\"Node v392\",\"Node v393\",\"Node v394\",\"Node v395\",\"Node v396\","
                          "\"Node v397\",\"Node v398\",\"Node v399\",\"Node v400\",\"Node v401\",\"Node v402\"]");
    assert_contains(json, "\"changesArchivedNodeEvidence\":false");
    assert_contains(json, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v156\"");
    assert_contains(json, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v156.json\"");
    assert_contains(
        json,
        "\"previousConsumptionNodeVersion\":\"Node v401 consumed v156 as final mini-kv approval gate input completion evidence\"");
    assert_contains(json, "\"rollingCurrentUsedForHistoricalBaseline\":false");
    assert_contains(json, "\"nodeV396ProgressIntakePreserved\":true");
    assert_contains(json, "\"nodeV397ContributionReviewPreserved\":true");
    assert_contains(json, "\"nodeV398ApprovalGateReviewPreserved\":true");
    assert_contains(json, "\"nodeV399ArchiveVerificationPreserved\":true");
    assert_contains(json, "\"nodeV400InputContractPreserved\":true");
    assert_contains(json, "\"nodeV401CompletionIntakePreserved\":true");
    assert_contains(json, "\"nodeV402TemplateValidatorPreserved\":true");
    assert_contains(json, "\"nodeV402TemplatesTreatedAsNonCanonicalInputs\":true");
    assert_contains(json, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(json, "\"activeShardPrototypeAllowed\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"shardDirectoryCreationAllowed\":false");
    assert_contains(json, "\"multiProcessStartAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"requiredBeforeActivation\":[\"separate active shard prototype plan\"");
    assert_contains(json, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesActivePrototypePlan\":true");
    assert_contains(json, "\"frozenActiveShardPrototypeAllowed\":false");
    assert_contains(json, "\"frozenRouterActivationAllowed\":false");
    assert_contains(json, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(json, "\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"readyForNodeConsumption\":true");
    assert_contains(json, "\"liveReadGateRequiredBeforeRuntimeProbe\":true");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredConsumerChecks\":[\"read frozen fixture instead of rolling current\"");
    assert_contains(json, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(json, "\"liveReadGateAllowed\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"requiresServiceOwner\":true");
    assert_contains(json, "\"requiresPortList\":true");
    assert_contains(json, "\"requiresSmokeTarget\":true");
    assert_contains(json, "\"requiresFailClosedBehavior\":true");
    assert_contains(json, "\"requiresCleanup\":true");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredBeforeLiveRead\":[\"explicit live-read gate plan\"");
    assert_contains(json, "\"liveReadGatePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesLiveReadGatePlan\":true");
    assert_contains(json, "\"frozenLiveReadGateAllowed\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenStartsServices\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"operatorOwnedServiceLifecycleRequired\":true");
    assert_contains(json, "\"serviceOwnerDeclared\":false");
    assert_contains(json, "\"startupCommandDeclared\":false");
    assert_contains(json, "\"portListDeclared\":false");
    assert_contains(json, "\"getOnlySmokeTargetDeclared\":false");
    assert_contains(json, "\"failClosedBehaviorRequired\":true");
    assert_contains(json, "\"cleanupResponsibilityDeclared\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredOperatorEvidence\":[\"service owner\"");
    assert_contains(json, "\"operatorServiceLifecycleTemplateFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesOperatorLifecycleTemplate\":true");
    assert_contains(json, "\"frozenServiceOwnerDeclared\":false");
    assert_contains(json, "\"frozenStartupCommandDeclared\":false");
    assert_contains(json, "\"frozenPortListDeclared\":false");
    assert_contains(json, "\"frozenGetOnlySmokeTargetDeclared\":false");
    assert_contains(json, "\"frozenCleanupResponsibilityDeclared\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(json, "\"serviceOwnerDeclared\":true");
    assert_contains(json, "\"serviceOwnerHandle\":\"mini-kv-local-operator\"");
    assert_contains(json, "\"startupCommandDeclared\":true");
    assert_contains(json, "\"startupCommand\":\"minikv_server.exe <operator-approved-loopback-port> 127.0.0.1\"");
    assert_contains(json, "\"portListDeclared\":true");
    assert_contains(json, "\"declaredPortHandles\":[\"operator-approved-loopback-port\"]");
    assert_contains(json, "\"getOnlySmokeTargetDeclared\":true");
    assert_contains(json, "\"getOnlySmokeTarget\":\"GET-only SHARDJSON evidence read\"");
    assert_contains(json, "\"failClosedBehaviorDeclared\":true");
    assert_contains(json, "\"failClosedBehavior\":\"missing evidence or port conflict blocks runtime gate\"");
    assert_contains(json, "\"cleanupResponsibilityDeclared\":true");
    assert_contains(json, "\"cleanupResponsibility\":\"operator stops service and verifies no LISTENING port\"");
    assert_contains(json, "\"runtimeGateApproved\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiresSeparateRuntimeGate\":true");
    assert_contains(json, "\"requiredBeforeRuntimeGate\":[\"operator approval record\"");
    assert_contains(json, "\"operatorServiceLifecycleEvidenceFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"preservesDeclaredLifecycleEvidence\":true");
    assert_contains(json, "\"frozenOperatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(json, "\"frozenRuntimeGateApproved\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                    "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(json, "\"readyForRuntimeExecutionPacket\":false");
    assert_contains(json, "\"readyForRuntimeLiveReadGate\":false");
    assert_contains(json, "\"runtimeExecutionArtifactsComplete\":false");
    assert_contains(json, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"requiredRuntimeExecutionArtifacts\":[\"operator approval record\","
                          "\"concrete loopback port assignment\",\"GET-only smoke command\","
                          "\"cleanup proof\",\"service owner confirmation\",\"process cleanup rules\"]");
    assert_contains(json, "\"runtimeExecutionPacketPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"executionAttempted\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"failClosedOnMissingArtifacts\":true");
    assert_contains(json,
                    "\"runtimeExecutionArtifactIntakePreflightFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\"");
    assert_contains(json, "\"frozenStatus\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(json, "\"preservesBlockedPreflight\":true");
    assert_contains(json, "\"frozenPresentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json,
                    "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                    "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\"");
    assert_contains(json, "\"candidateArtifactCount\":4");
    assert_contains(json, "\"candidateArtifactRows\":[\"mini-kv loopback port candidate\","
                          "\"mini-kv GET-only smoke command candidate\","
                          "\"mini-kv service owner candidate\",\"mini-kv process cleanup rules candidate\"]");
    assert_contains(json, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"acceptedRuntimeExecutionArtifactsComplete\":false");
    assert_contains(json, "\"operatorApprovalRecordPresent\":false");
    assert_contains(json, "\"nodeRuntimeWindowApproved\":false");
    assert_contains(json, "\"crossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(json, "\"javaPortBindingPresent\":false");
    assert_contains(json, "\"miniKvLoopbackPortCandidateDeclared\":true");
    assert_contains(json, "\"miniKvLoopbackHost\":\"127.0.0.1\"");
    assert_contains(json, "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(json, "\"miniKvLoopbackPortOperatorApproved\":false");
    assert_contains(json, "\"startupCommandCandidate\":\"minikv_server.exe 6424 127.0.0.1\"");
    assert_contains(json,
                    "\"getOnlySmokeCommandCandidate\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\"");
    assert_contains(json, "\"getOnlySmokeCommandOperatorApproved\":false");
    assert_contains(json, "\"serviceOwnerCandidateDeclared\":true");
    assert_contains(json, "\"serviceOwnerOperatorConfirmed\":false");
    assert_contains(json, "\"processCleanupRulesCandidateDeclared\":true");
    assert_contains(json, "\"cleanupProofPresent\":false");
    assert_contains(json, "\"requiresCrossProjectPacket\":true");
    assert_contains(json,
                    "\"miniKvRuntimeExecutionArtifactCandidateFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"preservesCandidateOnlyContribution\":true");
    assert_contains(json, "\"frozenCandidateArtifactCount\":4");
    assert_contains(json, "\"frozenAcceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v155\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\"");
    assert_contains(json, "\"sourceNodeReviewVersions\":[\"Node v397 contribution review\","
                          "\"Node v398 approval gate review\",\"Node v399 approval gate archive verification\","
                          "\"Node v400 approval input intake contract\"]");
    assert_contains(json, "\"approvalGateInputCount\":0");
    assert_contains(json, "\"missingApprovalGateInputCount\":3");
    assert_contains(json, "\"requiredApprovalGateInputs\":[\"Node-approved runtime window\","
                          "\"correlated operator approval record\","
                          "\"complete cross-project runtime execution packet\"]");
    assert_contains(json, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(json, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(json, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(json, "\"approvalGateInputsComplete\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"concreteLoopbackPortsAssigned\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"failClosedOnMissingApprovalInputs\":true");
    assert_contains(json,
                    "\"runtimeExecutionApprovalGateInputPrecheckFreeze\":{\"frozenReleaseVersion\":\"v155\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(json, "\"preservesApprovalGateInputPrecheck\":true");
    assert_contains(json, "\"frozenApprovalGateInputCount\":0");
    assert_contains(json, "\"frozenMissingApprovalGateInputCount\":3");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsJavaService\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(json, "\"sourceNodeContractVersion\":\"Node v400 approval input intake contract\"");
    assert_contains(json, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(json, "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(json, "\"miniKvApprovalGateInputItemCount\":4");
    assert_contains(json, "\"miniKvApprovalGateInputItems\":[\"concrete mini-kv loopback port assignment\","
                          "\"GET-only SHARDJSON smoke command\",\"mini-kv service owner confirmation\","
                          "\"stop-only-owned-process cleanup rules\"]");
    assert_contains(json, "\"miniKvLoopbackHost\":\"127.0.0.1\"");
    assert_contains(json, "\"miniKvLoopbackPort\":6424");
    assert_contains(json, "\"startupCommand\":\"minikv_server.exe 6424 127.0.0.1\"");
    assert_contains(json,
                    "\"getOnlySmokeCommand\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\"");
    assert_contains(json, "\"serviceOwnerConfirmed\":true");
    assert_contains(json, "\"processCleanupRulesComplete\":true");
    assert_contains(json, "\"stopOnlyOwnedProcessCleanupRequired\":true");
    assert_contains(json, "\"cleanupProofPresent\":false");
    assert_contains(json, "\"cleanupProofRequiresApprovedRuntimeStart\":true");
    assert_contains(json, "\"requiresNodeRuntimeWindow\":true");
    assert_contains(json, "\"requiresCorrelatedOperatorApproval\":true");
    assert_contains(json, "\"requiresCompleteCrossProjectPacket\":true");
    assert_contains(json, "\"miniKvFinalApprovalGateInputFreeze\":{\"frozenReleaseVersion\":\"v156\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v156.json\"");
    assert_contains(json, "\"frozenStatus\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(json, "\"preservesMiniKvFinalApprovalGateInput\":true");
    assert_contains(json, "\"frozenMiniKvApprovalGateInputComplete\":true");
    assert_contains(json, "\"frozenMiniKvLoopbackPort\":6424");
    assert_contains(json, "\"frozenCleanupProofPresent\":false");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenRouterActivationAllowed\":false");
    assert_contains(json, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                    "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(json, "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v402-post-runtime-execution-approval-input-template-validator-roadmap.md\"");
    assert_contains(json, "\"templateArchiveRoot\":\"e/402/input-templates\"");
    assert_contains(json, "\"canonicalInputRoot\":\"e/398/input\"");
    assert_contains(json, "\"templateOnlyInputCount\":3");
    assert_contains(json, "\"templateArchivePaths\":[\"e/402/input-templates/node-approved-runtime-window-v402.template.json\","
                          "\"e/402/input-templates/correlated-operator-approval-record-v402.template.json\","
                          "\"e/402/input-templates/cross-project-runtime-execution-packet-v402.template.json\"]");
    assert_contains(json, "\"canonicalTargetPaths\":[\"e/398/input/node-approved-runtime-window-v398.json\","
                          "\"e/398/input/correlated-operator-approval-record-v398.json\","
                          "\"e/398/input/cross-project-runtime-execution-packet-v398.json\"]");
    assert_contains(json, "\"canonicalRuntimeInputPresent\":false");
    assert_contains(json, "\"templateCopiedToCanonicalInput\":false");
    assert_contains(json, "\"nodeApprovedRuntimeWindowCanonicalPresent\":false");
    assert_contains(json, "\"correlatedOperatorApprovalRecordCanonicalPresent\":false");
    assert_contains(json, "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false");
    assert_contains(json, "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(json, "\"templatesAuthorizeRuntime\":false");
    assert_contains(json, "\"templateDigestAcceptedAsApproval\":false");
    assert_contains(json, "\"writesCanonicalApprovalInputFiles\":false");
    assert_contains(json, "\"changesNodeInputTemplateFiles\":false");
    assert_contains(json, "\"requiresCanonicalInputs\":true");
    assert_contains(json, "\"requiresSharedApprovalCorrelationId\":true");
    assert_contains(json, "\"requiresGetOnlySmokeCommands\":true");
    assert_contains(json, "\"requiresCleanupProofAfterApprovedStart\":true");
    assert_contains(json, "\"failClosedOnTemplateOnlyInputs\":true");
    assert_contains(json, "\"readOnlyBoundaryFields\":[\"readOnly\",\"executionAllowed\"");
    assert_contains(json, "\"evidenceDigest\":\"fnv1a64:");
}

} // namespace

int main() {
    const auto fixture_path = std::filesystem::path{"fixtures"} / "release" / "shard-readiness.json";
    const auto historical_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v144.json";
    const auto consumed_v145_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v145.json";
    const auto consumed_v146_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v146.json";
    const auto consumed_v147_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v147.json";
    const auto consumed_v148_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v148.json";
    const auto consumed_v149_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v149.json";
    const auto consumed_v150_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v150.json";
    const auto consumed_v151_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v151.json";
    const auto consumed_v152_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v152.json";
    const auto consumed_v153_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v153.json";
    const auto consumed_v154_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v154.json";
    const auto consumed_v155_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v155.json";
    const auto consumed_v156_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v156.json";
    const auto fixture = read_fixture_text(fixture_path);
    const auto historical_fixture = read_fixture_text(historical_fixture_path);
    const auto consumed_v145_fixture = read_fixture_text(consumed_v145_fixture_path);
    const auto consumed_v146_fixture = read_fixture_text(consumed_v146_fixture_path);
    const auto consumed_v147_fixture = read_fixture_text(consumed_v147_fixture_path);
    const auto consumed_v148_fixture = read_fixture_text(consumed_v148_fixture_path);
    const auto consumed_v149_fixture = read_fixture_text(consumed_v149_fixture_path);
    const auto consumed_v150_fixture = read_fixture_text(consumed_v150_fixture_path);
    const auto consumed_v151_fixture = read_fixture_text(consumed_v151_fixture_path);
    const auto consumed_v152_fixture = read_fixture_text(consumed_v152_fixture_path);
    const auto consumed_v153_fixture = read_fixture_text(consumed_v153_fixture_path);
    const auto consumed_v154_fixture = read_fixture_text(consumed_v154_fixture_path);
    const auto consumed_v155_fixture = read_fixture_text(consumed_v155_fixture_path);
    const auto consumed_v156_fixture = read_fixture_text(consumed_v156_fixture_path);

    assert(fixture == minikv::shard_readiness::format_json());
    assert(minikv::shard_readiness::fixture_path() == "fixtures/release/shard-readiness.json");
    assert_shard_readiness_contract(fixture);
    assert(fixture != historical_fixture);
    assert(fixture != consumed_v145_fixture);
    assert(fixture != consumed_v146_fixture);
    assert(fixture != consumed_v147_fixture);
    assert(fixture != consumed_v148_fixture);
    assert(fixture != consumed_v149_fixture);
    assert(fixture != consumed_v150_fixture);
    assert(fixture != consumed_v151_fixture);
    assert(fixture != consumed_v152_fixture);
    assert(fixture != consumed_v153_fixture);
    assert(fixture != consumed_v154_fixture);
    assert(fixture != consumed_v155_fixture);
    assert_contains(historical_fixture, "\"releaseVersion\":\"v144\"");
    assert_contains(historical_fixture, "\"status\":\"prototype-ready-read-only\"");
    assert_contains(historical_fixture, "\"evidenceDigest\":\"fnv1a64:22d3c4815a440804\"");
    assert_contains(consumed_v145_fixture, "\"releaseVersion\":\"v145\"");
    assert_contains(consumed_v145_fixture, "\"status\":\"hardened-read-only\"");
    assert_contains(consumed_v145_fixture, "\"evidenceDigest\":\"fnv1a64:ebe4c7e1a2704482\"");
    assert_contains(consumed_v145_fixture, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(consumed_v146_fixture, "\"releaseVersion\":\"v146\"");
    assert_contains(consumed_v146_fixture, "\"status\":\"historical-fallback-hardened-read-only\"");
    assert_contains(consumed_v146_fixture, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v145.json\"");
    assert_contains(consumed_v146_fixture, "\"evidenceDigest\":\"fnv1a64:6847d87decb76fcb\"");
    assert_contains(consumed_v147_fixture, "\"releaseVersion\":\"v147\"");
    assert_contains(consumed_v147_fixture, "\"status\":\"active-prototype-prerequisite-read-only\"");
    assert_contains(consumed_v147_fixture, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(consumed_v147_fixture, "\"activeShardPrototypeAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"evidenceDigest\":\"fnv1a64:e4a386fc9add4eaf\"");
    assert_contains(consumed_v148_fixture, "\"releaseVersion\":\"v148\"");
    assert_contains(consumed_v148_fixture, "\"status\":\"active-prototype-plan-frozen-read-only\"");
    assert_contains(consumed_v148_fixture, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v147\"");
    assert_contains(consumed_v148_fixture, "\"frozenRouterActivationAllowed\":false");
    assert_contains(consumed_v148_fixture, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(consumed_v148_fixture, "\"evidenceDigest\":\"fnv1a64:8270eeebbf1a7f29\"");
    assert_contains(consumed_v149_fixture, "\"releaseVersion\":\"v149\"");
    assert_contains(consumed_v149_fixture, "\"status\":\"frozen-evidence-handoff-read-only\"");
    assert_contains(consumed_v149_fixture, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v148\"");
    assert_contains(consumed_v149_fixture, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(consumed_v149_fixture, "\"startsServices\":false");
    assert_contains(consumed_v149_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v149_fixture, "\"evidenceDigest\":\"fnv1a64:521fe6dee47f7f2c\"");
    assert_contains(consumed_v150_fixture, "\"releaseVersion\":\"v150\"");
    assert_contains(consumed_v150_fixture, "\"status\":\"live-read-gate-prerequisite-read-only\"");
    assert_contains(consumed_v150_fixture, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(consumed_v150_fixture, "\"liveReadGateAllowed\":false");
    assert_contains(consumed_v150_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v150_fixture, "\"startsServices\":false");
    assert_contains(consumed_v150_fixture, "\"evidenceDigest\":\"fnv1a64:b8b134f6aa527ca4\"");
    assert_contains(consumed_v151_fixture, "\"releaseVersion\":\"v151\"");
    assert_contains(consumed_v151_fixture, "\"status\":\"operator-service-lifecycle-template-read-only\"");
    assert_contains(consumed_v151_fixture, "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(consumed_v151_fixture, "\"serviceOwnerDeclared\":false");
    assert_contains(consumed_v151_fixture, "\"startupCommandDeclared\":false");
    assert_contains(consumed_v151_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v151_fixture, "\"evidenceDigest\":\"fnv1a64:c9fb568ddff895e3\"");
    assert_contains(consumed_v152_fixture, "\"releaseVersion\":\"v152\"");
    assert_contains(consumed_v152_fixture, "\"status\":\"declared-operator-lifecycle-no-runtime-read-only\"");
    assert_contains(consumed_v152_fixture,
                    "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(consumed_v152_fixture, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(consumed_v152_fixture, "\"serviceOwnerDeclared\":true");
    assert_contains(consumed_v152_fixture, "\"runtimeGateApproved\":false");
    assert_contains(consumed_v152_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v152_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v152_fixture, "\"evidenceDigest\":\"fnv1a64:55cd5b8db109c64f\"");
    assert_contains(consumed_v153_fixture, "\"releaseVersion\":\"v153\"");
    assert_contains(consumed_v153_fixture,
                    "\"status\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(consumed_v153_fixture,
                    "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                    "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(consumed_v153_fixture, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(consumed_v153_fixture, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(consumed_v153_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v153_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v153_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v153_fixture, "\"evidenceDigest\":\"fnv1a64:ca09095f3f3e4b2c\"");
    assert_contains(consumed_v154_fixture, "\"releaseVersion\":\"v154\"");
    assert_contains(consumed_v154_fixture,
                    "\"status\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(consumed_v154_fixture,
                    "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                    "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(consumed_v154_fixture, "\"candidateArtifactCount\":4");
    assert_contains(consumed_v154_fixture, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(consumed_v154_fixture, "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(consumed_v154_fixture, "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(consumed_v154_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v154_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v154_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v154_fixture, "\"evidenceDigest\":\"fnv1a64:edbe9c546c0d780a\"");
    assert_contains(consumed_v155_fixture, "\"releaseVersion\":\"v155\"");
    assert_contains(consumed_v155_fixture,
                    "\"status\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(consumed_v155_fixture,
                    "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(consumed_v155_fixture, "\"approvalGateInputCount\":0");
    assert_contains(consumed_v155_fixture, "\"missingApprovalGateInputCount\":3");
    assert_contains(consumed_v155_fixture, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(consumed_v155_fixture, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(consumed_v155_fixture, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(consumed_v155_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v155_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v155_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v155_fixture, "\"evidenceDigest\":\"fnv1a64:edb1c4d8534461ab\"");
    assert_contains(consumed_v156_fixture, "\"releaseVersion\":\"v156\"");
    assert_contains(consumed_v156_fixture, "\"status\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(consumed_v156_fixture,
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(consumed_v156_fixture, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(consumed_v156_fixture, "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(consumed_v156_fixture, "\"miniKvLoopbackPort\":6424");
    assert_contains(consumed_v156_fixture, "\"cleanupProofPresent\":false");
    assert_contains(consumed_v156_fixture, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(consumed_v156_fixture, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(consumed_v156_fixture, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(consumed_v156_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v156_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v156_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v156_fixture, "\"evidenceDigest\":\"fnv1a64:f240d1fe1b25fab7\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDJSON extra");
    assert(result.response == "ERR usage: SHARDJSON");

    result = processor.execute("SHARDJSON");
    assert(result.response == fixture);
    assert_shard_readiness_contract(result.response);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    assert_contains(result.response, "SHARDJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    assert_contains(result.response, "\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");

    result = processor.execute("EXPLAINJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    result = processor.execute("CHECKJSON SHARDJSON extra");
    assert_contains(result.response, "\"allowed_by_parser\":false");
    assert_contains(result.response, "\"usage: SHARDJSON\"");
}
