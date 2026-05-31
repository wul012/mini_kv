#include "minikv/shard_readiness_approval_inputs.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness::approval_inputs {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_final_approval_gate_input_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v156\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v156.json\","
           "\"frozenStatus\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\","
           "\"preservesMiniKvFinalApprovalGateInput\":true,"
           "\"frozenFinalMiniKvApprovalGateInputPresent\":true,"
           "\"frozenMiniKvApprovalGateInputComplete\":true,"
           "\"frozenMiniKvApprovalGateInputItemCount\":4,"
           "\"frozenMiniKvLoopbackHost\":\"127.0.0.1\","
           "\"frozenMiniKvLoopbackPort\":6424,"
           "\"frozenGetOnlySmokeCommandDeclared\":true,"
           "\"frozenServiceOwnerConfirmed\":true,"
           "\"frozenProcessCleanupRulesComplete\":true,"
           "\"frozenCleanupProofPresent\":false,"
           "\"frozenRuntimeGateApprovalPresent\":false,"
           "\"frozenReadyForRuntimeExecutionPacket\":false,"
           "\"frozenReadyForRuntimeLiveReadGate\":false,"
           "\"frozenRuntimeExecutionPacketExecutable\":false,"
           "\"frozenStartsJavaService\":false,"
           "\"frozenStartsMiniKvService\":false,"
           "\"frozenStartsServices\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_template_validator_echo_json() {
    return "{\"echoMode\":\"template-validator-echo-no-canonical-inputs\","
           "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\","
           "\"sourceNodePlan\":\"docs/plans3/v402-post-runtime-execution-approval-input-template-validator-roadmap.md\","
           "\"templateArchiveRoot\":\"e/402/input-templates\","
           "\"canonicalInputRoot\":\"e/398/input\","
           "\"templateOnlyInputCount\":3,"
           "\"templateArchivePaths\":" + json_string_array({
               "e/402/input-templates/node-approved-runtime-window-v402.template.json",
               "e/402/input-templates/correlated-operator-approval-record-v402.template.json",
               "e/402/input-templates/cross-project-runtime-execution-packet-v402.template.json",
           }) +
           ",\"canonicalTargetPaths\":" + json_string_array({
               "e/398/input/node-approved-runtime-window-v398.json",
               "e/398/input/correlated-operator-approval-record-v398.json",
               "e/398/input/cross-project-runtime-execution-packet-v398.json",
           }) +
           ",\"canonicalRuntimeInputPresent\":false,"
           "\"templateCopiedToCanonicalInput\":false,"
           "\"nodeApprovedRuntimeWindowCanonicalPresent\":false,"
           "\"correlatedOperatorApprovalRecordCanonicalPresent\":false,"
           "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false,"
           "\"sharedApprovalCorrelationIdPresent\":false,"
           "\"templatesAuthorizeRuntime\":false,"
           "\"templateDigestAcceptedAsApproval\":false,"
           "\"writesCanonicalApprovalInputFiles\":false,"
           "\"changesNodeInputTemplateFiles\":false,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
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
           "\"requiresCanonicalInputs\":true,"
           "\"requiresSharedApprovalCorrelationId\":true,"
           "\"requiresGetOnlySmokeCommands\":true,"
           "\"requiresCleanupProofAfterApprovedStart\":true,"
           "\"failClosedOnTemplateOnlyInputs\":true}";
}

std::string format_template_validator_echo_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v157\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v157.json\","
           "\"frozenStatus\":\"runtime-execution-approval-input-template-validator-echo-read-only\","
           "\"preservesTemplateValidatorEcho\":true,"
           "\"frozenTemplateOnlyInputCount\":3,"
           "\"frozenCanonicalRuntimeInputPresent\":false,"
           "\"frozenTemplateCopiedToCanonicalInput\":false,"
           "\"frozenSharedApprovalCorrelationIdPresent\":false,"
           "\"frozenTemplatesAuthorizeRuntime\":false,"
           "\"frozenTemplateDigestAcceptedAsApproval\":false,"
           "\"frozenWritesCanonicalApprovalInputFiles\":false,"
           "\"frozenChangesNodeInputTemplateFiles\":false,"
           "\"frozenRuntimeExecutionPacketExecutable\":false,"
           "\"frozenStartsJavaService\":false,"
           "\"frozenStartsMiniKvService\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_canonical_approval_input_precheck_json() {
    return "{\"precheckMode\":\"blocked-missing-canonical-approval-inputs\","
           "\"sourceFrozenReleaseVersion\":\"v157\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v157.json\","
           "\"sourceNodeCompatibilityVersion\":\"Node v403 template compatibility intake\","
           "\"sourceNodePlan\":\"docs/plans3/v403-post-java-mini-kv-runtime-execution-approval-input-template-compatibility-intake-roadmap.md\","
           "\"canonicalInputRoot\":\"e/398/input\","
           "\"requiredCanonicalInputCount\":3,"
           "\"presentCanonicalInputCount\":0,"
           "\"missingCanonicalInputCount\":3,"
           "\"requiredCanonicalInputPaths\":" + json_string_array({
               "e/398/input/node-approved-runtime-window-v398.json",
               "e/398/input/correlated-operator-approval-record-v398.json",
               "e/398/input/cross-project-runtime-execution-packet-v398.json",
           }) +
           ",\"nodeApprovedRuntimeWindowCanonicalPresent\":false,"
           "\"correlatedOperatorApprovalRecordCanonicalPresent\":false,"
           "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false,"
           "\"canonicalApprovalInputsComplete\":false,"
           "\"sharedApprovalCorrelationIdPresent\":false,"
           "\"sharedApprovalCorrelationIdValidated\":false,"
           "\"templatesAcceptedAsCanonicalInputs\":false,"
           "\"templateCompatibilityEvidenceAcceptedAsApproval\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
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
           "\"requiresRealCanonicalInputs\":true,"
           "\"requiresSharedApprovalCorrelationId\":true,"
           "\"requiresConcreteApprovalValues\":true,"
           "\"requiresGetOnlySmokeCommands\":true,"
           "\"requiresCleanupProofAfterApprovedStart\":true,"
           "\"failClosedOnMissingCanonicalInputs\":true}";
}

} // namespace minikv::shard_readiness::approval_inputs
