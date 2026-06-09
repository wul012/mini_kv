#include "minikv/shard_preview_candidate_submission_precheck.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity.hpp"
#include "minikv/shard_preview_candidate_submission_precheck_checkpoints.hpp"
#include "minikv/shard_preview_candidate_submission_precheck_validation.hpp"
#include "minikv/shard_preview_candidate_submission_precheck_validators.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_submission_precheck {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checkpoints = shard_preview_candidate_submission_precheck_checkpoints;
namespace source_integrity = shard_preview_candidate_request_package_integrity;
namespace validation = shard_preview_candidate_submission_precheck_validation;
namespace validators = shard_preview_candidate_submission_precheck_validators;

constexpr std::string_view precheck_command = "SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1411-controlled-read-only-shard-preview-candidate-document-submission-precheck-closeout-roadmap.md";
constexpr int current_candidate_submission_precheck_stage_count = 10;
constexpr int source_candidate_request_package_integrity_planned_stage_count = 25;
constexpr int source_candidate_document_request_package_release_number = 895;
constexpr int source_candidate_request_package_integrity_release_number = 920;
constexpr int node_submission_precheck_checkpoint_count = 25;
constexpr int node_submission_precheck_validator_count = 25;
constexpr int requested_candidate_field_count = 20;
constexpr int submission_gate_count = 40;

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
    return checkpoints::candidate_submission_precheck_checkpoints()
        [static_cast<std::size_t>(current_candidate_submission_precheck_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        checkpoints::candidate_submission_precheck_checkpoints(),
        current_candidate_submission_precheck_stage_count,
        checkpoints::planned_candidate_submission_precheck_checkpoint_count(),
        checkpoints::first_candidate_submission_precheck_release_number());
}

int completed_validator_count() {
    return current_candidate_submission_precheck_stage_count;
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_candidate_request_package_integrity_planned_stage_count;
}

} // namespace

std::string format_candidate_submission_precheck_checkpoint_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        checkpoints::candidate_submission_precheck_checkpoints(),
        current_candidate_submission_precheck_stage_count);
}

std::string candidate_submission_precheck_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_submission_precheck_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_integrity_stage_count = source_integrity::published_stage_count();
    const bool source_integrity_chain_complete = source_integrity_complete();
    const int planned_checkpoint_count = checkpoints::planned_candidate_submission_precheck_checkpoint_count();
    const int planned_validator_count = validators::planned_candidate_submission_precheck_validator_count();
    const int validator_count = completed_validator_count();
    const bool precheck_chain_complete = current_candidate_submission_precheck_stage_count == planned_checkpoint_count &&
                                         source_integrity_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-document-submission-precheck.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(precheck_command) +
           ",\"candidateSubmissionPrecheckMode\":\"controlled-read-only-submission-precheck-handoff\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\""
           ",\"sourceNodeSubmissionCheckpointCount\":" + std::to_string(node_submission_precheck_checkpoint_count) +
           ",\"sourceNodeSubmissionValidatorCount\":" + std::to_string(node_submission_precheck_validator_count) +
           ",\"sourceNodeSubmissionGateCount\":" + std::to_string(submission_gate_count) +
           ",\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v" +
           std::to_string(source_candidate_document_request_package_release_number) + "\""
           ",\"sourceCandidateRequestPackageIntegrityCommand\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\""
           ",\"sourceCandidateRequestPackageIntegrityReleaseVersion\":\"v" +
           std::to_string(source_candidate_request_package_integrity_release_number) + "\""
           ",\"sourceCandidateRequestPackageIntegrityFixturePath\":\"fixtures/release/shard-readiness-v920.json\""
           ",\"sourceCandidateRequestPackageIntegrityPublishedStageCount\":" +
           std::to_string(source_integrity_stage_count) +
           ",\"sourceCandidateRequestPackageIntegrityPlannedStageCount\":" +
           std::to_string(source_candidate_request_package_integrity_planned_stage_count) +
           ",\"sourceCandidateRequestPackageIntegrityComplete\":" + json_bool(source_integrity_chain_complete) +
           ",\"sourceCandidateRequestPackageIntegrityDigestMarker\":" +
           json_string(source_integrity::candidate_request_package_integrity_digest_marker()) +
           ",\"sourceCandidateRequestPackageIntegrityStatus\":" +
           json_string(source_integrity::candidate_request_package_integrity_status()) +
           ",\"requestedCandidateFieldCount\":" + std::to_string(requested_candidate_field_count) +
           ",\"submissionGateCount\":" + std::to_string(submission_gate_count) +
           ",\"candidateSubmissionPrecheckStage\":" + json_string(stage.stage) +
           ",\"candidateSubmissionPrecheckStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateSubmissionPrecheckReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateSubmissionPrecheckReleaseRangeStart\":\"v921\""
           ",\"candidateSubmissionPrecheckReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_candidate_submission_precheck_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedSubmissionPrecheckCheckpointCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"completedSubmissionPrecheckCheckpointCount\":" +
           std::to_string(current_candidate_submission_precheck_stage_count) +
           ",\"plannedSubmissionPrecheckValidatorCount\":" + std::to_string(planned_validator_count) +
           ",\"completedSubmissionPrecheckValidatorCount\":" + std::to_string(validator_count) +
           ",\"candidateSubmissionPrecheckDeclared\":true"
           ",\"candidateSubmissionPrecheckOnly\":true"
           ",\"candidateSubmissionPrecheckHandoffReady\":true"
           ",\"candidateSubmissionPrecheckChainComplete\":" + json_bool(precheck_chain_complete) +
           ",\"sourceCandidateRequestPackageIntegrityFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"requestPackageReadyForFutureRealDocumentIntake\":true"
           ",\"submissionPrecheckReadyForFutureRealDocumentIntake\":true"
           ",\"additionalRequestEchoCreated\":false"
           ",\"additionalSubmissionPrecheckEchoCreated\":false"
           ",\"newCandidateDocumentRequestPackageCreated\":false"
           ",\"reviewedRealCandidateDocumentRequired\":true"
           ",\"reviewedRealCandidateDocumentPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"syntheticCandidateDocumentAccepted\":false"
           ",\"unreviewedCandidateDocumentAccepted\":false"
           ",\"candidateSubmissionAccepted\":false"
           ",\"candidateSubmissionImported\":false"
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
           ",\"moduleSplit\":[\"candidate_submission_precheck_core\",\"candidate_submission_precheck_checkpoints\",\"candidate_submission_precheck_validators\",\"candidate_submission_precheck_validation\"]"
           ",\"checkpointCatalog\":" + format_candidate_submission_precheck_checkpoint_catalog_json() +
           ",\"validators\":" + validators::format_candidate_submission_precheck_validators_json(validator_count) +
           ",\"validation\":" +
           validation::format_candidate_submission_precheck_validation_json(
               source_integrity_stage_count,
               source_integrity_chain_complete,
               planned_checkpoint_count,
               current_candidate_submission_precheck_stage_count,
               planned_validator_count,
               validator_count,
               current_candidate_submission_precheck_stage_count,
               planned_checkpoint_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1411 submission precheck scope is consumed as read-only planning context",
                              "v920 integrity evidence is frozen before v921-v930 submission precheck handoff",
                              "mini-kv records checkpoint and validator handles but does not read candidate documents",
                              "real candidate document intake remains closed until reviewed material exists",
                              "document router write routing WAL touch approval signature runtime import and execution remain disabled"}) +
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

std::string candidate_submission_precheck_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_submission_precheck_stage_count,
        checkpoints::planned_candidate_submission_precheck_checkpoint_count());
}

int published_stage_count() {
    return current_candidate_submission_precheck_stage_count;
}

} // namespace minikv::shard_preview_candidate_submission_precheck
