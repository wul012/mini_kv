#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_checkpoints.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_validation.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_validators.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_material_submission_precheck {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checkpoints = shard_preview_candidate_material_submission_precheck_checkpoints;
namespace source_integrity = shard_preview_candidate_material_request_integrity;
namespace validation = shard_preview_candidate_material_submission_precheck_validation;
namespace validators = shard_preview_candidate_material_submission_precheck_validators;

constexpr std::string_view precheck_command = "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1456-controlled-read-only-shard-preview-candidate-document-material-submission-precheck-roadmap.md";
constexpr int current_candidate_material_submission_precheck_stage_count = 10;
constexpr int source_candidate_material_request_integrity_planned_stage_count = 10;
constexpr int source_candidate_material_request_release_number = 965;
constexpr int source_candidate_material_request_integrity_release_number = 975;
constexpr int source_material_request_item_count = 25;
constexpr int source_acceptance_check_count = 25;
constexpr int required_material_field_count = 20;
constexpr int submission_material_field_count = 20;
constexpr int material_submission_gate_count = 41;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return checkpoints::candidate_material_submission_precheck_checkpoints()
        [static_cast<std::size_t>(current_candidate_material_submission_precheck_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        checkpoints::candidate_material_submission_precheck_checkpoints(),
        current_candidate_material_submission_precheck_stage_count,
        checkpoints::planned_candidate_material_submission_precheck_checkpoint_count(),
        checkpoints::first_candidate_material_submission_precheck_release_number());
}

int completed_validator_count() {
    return current_candidate_material_submission_precheck_stage_count;
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_candidate_material_request_integrity_planned_stage_count;
}

} // namespace

std::string format_candidate_material_submission_precheck_checkpoint_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        checkpoints::candidate_material_submission_precheck_checkpoints(),
        current_candidate_material_submission_precheck_stage_count);
}

std::string candidate_material_submission_precheck_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_material_submission_precheck_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_integrity_stage_count = source_integrity::published_stage_count();
    const bool source_integrity_chain_complete = source_integrity_complete();
    const int planned_checkpoint_count = checkpoints::planned_candidate_material_submission_precheck_checkpoint_count();
    const int planned_validator_count = validators::planned_candidate_material_submission_precheck_validator_count();
    const int validator_count = completed_validator_count();
    const bool precheck_chain_complete =
        current_candidate_material_submission_precheck_stage_count == planned_checkpoint_count &&
        source_integrity_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-document-material-submission-precheck.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(precheck_command) +
           ",\"candidateMaterialSubmissionPrecheckMode\":\"controlled-read-only-material-submission-precheck-handoff\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\""
           ",\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\""
           ",\"sourceNodeMaterialSubmissionCheckpointCount\":10"
           ",\"sourceNodeMaterialSubmissionValidatorCount\":10"
           ",\"sourceNodeMaterialRequestItemCount\":" + std::to_string(source_material_request_item_count) +
           ",\"sourceNodeMaterialAcceptanceCheckCount\":" + std::to_string(source_acceptance_check_count) +
           ",\"requiredMaterialFieldCount\":" + std::to_string(required_material_field_count) +
           ",\"submissionMaterialFieldCount\":" + std::to_string(submission_material_field_count) +
           ",\"materialSubmissionGateCount\":" + std::to_string(material_submission_gate_count) +
           ",\"sourceCandidateMaterialRequestReleaseVersion\":\"v" +
           std::to_string(source_candidate_material_request_release_number) + "\""
           ",\"sourceCandidateMaterialRequestIntegrityCommand\":\"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\""
           ",\"sourceCandidateMaterialRequestIntegrityReleaseVersion\":\"v" +
           std::to_string(source_candidate_material_request_integrity_release_number) + "\""
           ",\"sourceCandidateMaterialRequestIntegrityFixturePath\":\"fixtures/release/shard-readiness-v975.json\""
           ",\"sourceCandidateMaterialRequestIntegrityPublishedStageCount\":" +
           std::to_string(source_integrity_stage_count) +
           ",\"sourceCandidateMaterialRequestIntegrityPlannedStageCount\":" +
           std::to_string(source_candidate_material_request_integrity_planned_stage_count) +
           ",\"sourceCandidateMaterialRequestIntegrityComplete\":" + json_bool(source_integrity_chain_complete) +
           ",\"sourceCandidateMaterialRequestIntegrityDigestMarker\":" +
           json_string(source_integrity::candidate_material_request_integrity_digest_marker()) +
           ",\"sourceCandidateMaterialRequestIntegrityStatus\":" +
           json_string(source_integrity::candidate_material_request_integrity_status()) +
           ",\"candidateMaterialSubmissionPrecheckStage\":" + json_string(stage.stage) +
           ",\"candidateMaterialSubmissionPrecheckStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateMaterialSubmissionPrecheckReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateMaterialSubmissionPrecheckReleaseRangeStart\":\"v976\""
           ",\"candidateMaterialSubmissionPrecheckReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_candidate_material_submission_precheck_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedMaterialSubmissionPrecheckCheckpointCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"completedMaterialSubmissionPrecheckCheckpointCount\":" +
           std::to_string(current_candidate_material_submission_precheck_stage_count) +
           ",\"plannedMaterialSubmissionPrecheckValidatorCount\":" + std::to_string(planned_validator_count) +
           ",\"completedMaterialSubmissionPrecheckValidatorCount\":" + std::to_string(validator_count) +
           ",\"candidateMaterialSubmissionPrecheckDeclared\":true"
           ",\"candidateMaterialSubmissionPrecheckOnly\":true"
           ",\"candidateMaterialSubmissionPrecheckHandoffReady\":true"
           ",\"candidateMaterialSubmissionPrecheckChainComplete\":" + json_bool(precheck_chain_complete) +
           ",\"sourceCandidateMaterialRequestIntegrityFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"materialSubmissionPrecheckReadyForFutureReviewedMaterial\":true"
           ",\"nodeStopConditionRespected\":true"
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"additionalMaterialSubmissionPrecheckEchoCreated\":false"
           ",\"newMaterialRequestCreated\":false"
           ",\"materialRequestReopened\":false"
           ",\"materialSubmissionOpened\":false"
           ",\"reviewedRealCandidateDocumentRequired\":true"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"materialRequestSatisfied\":false"
           ",\"materialSubmissionSatisfied\":false"
           ",\"realMaterialAccepted\":false"
           ",\"syntheticMaterialAccepted\":false"
           ",\"unreviewedMaterialAccepted\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"candidateSubmissionAccepted\":false"
           ",\"candidateSubmissionImported\":false"
           ",\"materialPayloadImported\":false"
           ",\"candidatePayloadImported\":false"
           ",\"stagedCandidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"candidateDocumentRejected\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"moduleSplit\":[\"candidate_material_submission_precheck_core\",\"candidate_material_submission_precheck_checkpoints\",\"candidate_material_submission_precheck_validators\",\"candidate_material_submission_precheck_validation\"]"
           ",\"checkpointCatalog\":" + format_candidate_material_submission_precheck_checkpoint_catalog_json() +
           ",\"validators\":" +
           validators::format_candidate_material_submission_precheck_validators_json(validator_count) +
           ",\"validation\":" +
           validation::format_candidate_material_submission_precheck_validation_json(
               source_integrity_stage_count,
               source_integrity_chain_complete,
               source_material_request_item_count,
               source_acceptance_check_count,
               required_material_field_count,
               submission_material_field_count,
               material_submission_gate_count,
               planned_checkpoint_count,
               current_candidate_material_submission_precheck_stage_count,
               planned_validator_count,
               validator_count,
               current_candidate_material_submission_precheck_stage_count,
               planned_checkpoint_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1456 material submission precheck scope is consumed as read-only planning context",
                              "v975 material request integrity evidence is frozen before v976-v985 material submission precheck handoff",
                              "mini-kv records checkpoint and validator handles but does not read reviewed material",
                              "real material submission remains closed until externally supplied reviewed material exists",
                              "document intake router write routing WAL touch approval signature runtime import and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string candidate_material_submission_precheck_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_material_submission_precheck_stage_count,
        checkpoints::planned_candidate_material_submission_precheck_checkpoint_count());
}

int published_stage_count() {
    return current_candidate_material_submission_precheck_stage_count;
}

} // namespace minikv::shard_preview_candidate_material_submission_precheck
