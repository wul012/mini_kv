#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_checks.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_stages.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_material_submission_precheck_integrity {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_material_submission_precheck_integrity_checks;
namespace source_precheck = shard_preview_candidate_material_submission_precheck;
namespace stages = shard_preview_candidate_material_submission_precheck_integrity_stages;
namespace validation = shard_preview_candidate_material_submission_precheck_integrity_validation;

constexpr std::string_view integrity_command = "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1456-controlled-read-only-shard-preview-candidate-document-material-submission-precheck-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_precheck_planned_stage_count = 10;
constexpr int source_precheck_checkpoint_count = 10;
constexpr int source_precheck_validator_count = 10;
constexpr int source_material_request_item_count = 25;
constexpr int source_acceptance_check_count = 25;
constexpr int source_required_field_count = 20;
constexpr int source_submission_field_count = 20;
constexpr int source_material_submission_gate_count = 41;

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
    return stages::candidate_material_submission_precheck_integrity_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_material_submission_precheck_integrity_stages(),
        current_stage_count,
        stages::planned_candidate_material_submission_precheck_integrity_stage_count(),
        stages::first_candidate_material_submission_precheck_integrity_release_number());
}

int completed_integrity_check_count() {
    return current_stage_count;
}

bool source_precheck_complete() {
    return source_precheck::published_stage_count() == source_precheck_planned_stage_count;
}

} // namespace

std::string format_candidate_material_submission_precheck_integrity_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_material_submission_precheck_integrity_stages(),
        current_stage_count);
}

std::string candidate_material_submission_precheck_integrity_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_material_submission_precheck_integrity_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_published_stage_count = source_precheck::published_stage_count();
    const bool source_chain_complete = source_precheck_complete();
    const int planned_check_count = checks::planned_candidate_material_submission_precheck_integrity_check_count();
    const int check_count = completed_integrity_check_count();
    const bool integrity_chain_complete =
        current_stage_count == stages::planned_candidate_material_submission_precheck_integrity_stage_count() &&
        source_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-material-submission-precheck-fixture-integrity.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(integrity_command) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityMode\":\"controlled-read-only-precheck-fixture-integrity-hardening\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\""
           ",\"sourceCandidateMaterialSubmissionPrecheckCommand\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON\""
           ",\"sourceCandidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\""
           ",\"sourceCandidateMaterialSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v985.json\""
           ",\"sourceCandidateMaterialSubmissionPrecheckArchiveRootHint\":\"e/985/\""
           ",\"sourceCandidateMaterialSubmissionPrecheckPublishedStageCount\":" +
           std::to_string(source_published_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckPlannedStageCount\":" +
           std::to_string(source_precheck_planned_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceCandidateMaterialSubmissionPrecheckDigestMarker\":" +
           json_string(source_precheck::candidate_material_submission_precheck_digest_marker()) +
           ",\"sourceCandidateMaterialSubmissionPrecheckStatus\":" +
           json_string(source_precheck::candidate_material_submission_precheck_status()) +
           ",\"sourceMaterialSubmissionPrecheckCheckpointCount\":" +
           std::to_string(source_precheck_checkpoint_count) +
           ",\"sourceMaterialSubmissionPrecheckValidatorCount\":" +
           std::to_string(source_precheck_validator_count) +
           ",\"sourceMaterialRequestItemCount\":" + std::to_string(source_material_request_item_count) +
           ",\"sourceAcceptanceCheckCount\":" + std::to_string(source_acceptance_check_count) +
           ",\"sourceRequiredMaterialFieldCount\":" + std::to_string(source_required_field_count) +
           ",\"sourceSubmissionMaterialFieldCount\":" + std::to_string(source_submission_field_count) +
           ",\"sourceMaterialSubmissionGateCount\":" + std::to_string(source_material_submission_gate_count) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityStage\":" + json_string(stage.stage) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityReleaseRangeStart\":\"v986\""
           ",\"candidateMaterialSubmissionPrecheckIntegrityReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_candidate_material_submission_precheck_integrity_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(check_count) +
           ",\"candidateMaterialSubmissionPrecheckIntegrityDeclared\":true"
           ",\"candidateMaterialSubmissionPrecheckIntegrityOnly\":true"
           ",\"sourceCandidateMaterialSubmissionPrecheckFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"currentFixtureCarriesIntegritySection\":true"
           ",\"sourcePrecheckReadyForFutureReviewedMaterial\":true"
           ",\"integrityChainComplete\":" + json_bool(integrity_chain_complete) +
           ",\"nodeStopConditionRespected\":true"
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"additionalMaterialSubmissionPrecheckEchoCreated\":false"
           ",\"newMaterialSubmissionPrecheckCreated\":false"
           ",\"materialSubmissionPrecheckReopened\":false"
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
           ",\"moduleSplit\":[\"candidate_material_submission_precheck_integrity_core\",\"candidate_material_submission_precheck_integrity_stages\",\"candidate_material_submission_precheck_integrity_checks\",\"candidate_material_submission_precheck_integrity_validation\"]"
           ",\"stageCatalog\":" + format_candidate_material_submission_precheck_integrity_stage_catalog_json() +
           ",\"integrityChecks\":" +
           checks::format_candidate_material_submission_precheck_integrity_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_candidate_material_submission_precheck_integrity_validation_json(
               source_published_stage_count,
               source_chain_complete,
               source_precheck_checkpoint_count,
               source_precheck_validator_count,
               source_material_request_item_count,
               source_acceptance_check_count,
               source_required_field_count,
               source_submission_field_count,
               source_material_submission_gate_count,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_candidate_material_submission_precheck_integrity_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1456 stop condition is honored by auditing the frozen precheck instead of creating another submission echo",
                              "v985 is versioned as fixtures/release/shard-readiness-v985.json before v986-v1010 integrity hardening",
                              "mini-kv validates fixture lineage command catalog visibility and no side effect boundaries only",
                              "reviewed real material submission remains closed until externally supplied reviewed material exists",
                              "document intake router write routing WAL touch runtime payload import approval signature and execution remain disabled"}) +
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

std::string candidate_material_submission_precheck_integrity_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_candidate_material_submission_precheck_integrity_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_candidate_material_submission_precheck_integrity
