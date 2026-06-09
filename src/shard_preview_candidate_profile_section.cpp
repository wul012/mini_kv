#include "minikv/shard_preview_candidate_profile_section.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"
#include "minikv/shard_preview_candidate_profile_section_checks.hpp"
#include "minikv/shard_preview_candidate_profile_section_stages.hpp"
#include "minikv/shard_preview_candidate_profile_section_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_profile_section {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_profile_section_checks;
namespace source_integrity = shard_preview_candidate_material_submission_precheck_integrity;
namespace stages = shard_preview_candidate_profile_section_stages;
namespace validation = shard_preview_candidate_profile_section_validation;

constexpr std::string_view profile_section_command = "SHARDROUTECANDIDATEPROFILESECTIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1481-controlled-read-only-shard-preview-candidate-document-profile-section-renderer-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_integrity_planned_stage_count = 25;
constexpr int candidate_profile_section_count = 5;
constexpr int node_migrated_candidate_profile_section_count = 5;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
constexpr bool mini_kv_consumes_profile_material = false;

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
    return stages::candidate_profile_section_stages()[static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_profile_section_stages(),
        current_stage_count,
        stages::planned_candidate_profile_section_stage_count(),
        stages::first_candidate_profile_section_release_number());
}

int completed_profile_section_check_count() {
    return current_stage_count;
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_integrity_planned_stage_count;
}

} // namespace

std::string format_candidate_profile_section_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_profile_section_stages(),
        current_stage_count);
}

std::string candidate_profile_section_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_profile_section_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_published_stage_count = source_integrity::published_stage_count();
    const bool source_chain_complete = source_integrity_complete();
    const int planned_check_count = checks::planned_candidate_profile_section_check_count();
    const int check_count = completed_profile_section_check_count();
    const bool profile_section_chain_complete =
        current_stage_count == stages::planned_candidate_profile_section_stage_count() && source_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-document-profile-section-renderer-split.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(profile_section_command) +
           ",\"candidateProfileSectionMode\":\"controlled-read-only-profile-section-renderer-split-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProfileSectionReleaseVersion\":\"Node v1481\""
           ",\"sourceNodeCandidateProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewCandidateDocumentProfileSectionRenderer.ts\""
           ",\"sourceNodeProfileSectionGroupCount\":" + std::to_string(candidate_profile_section_count) +
           ",\"sourceNodeMigratedCandidateProfileSectionCount\":" +
           std::to_string(node_migrated_candidate_profile_section_count) +
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeStartsNodeServiceForMiniKv\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityCommand\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON\""
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\""
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1010.json\""
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount\":" +
           std::to_string(source_published_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityPlannedStageCount\":" +
           std::to_string(source_integrity_planned_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityDigestMarker\":" +
           json_string(source_integrity::candidate_material_submission_precheck_integrity_digest_marker()) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityStatus\":" +
           json_string(source_integrity::candidate_material_submission_precheck_integrity_status()) +
           ",\"candidateProfileSectionStage\":" + json_string(stage.stage) +
           ",\"candidateProfileSectionStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateProfileSectionReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateProfileSectionReleaseRangeStart\":\"v1011\""
           ",\"candidateProfileSectionReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(stages::planned_candidate_profile_section_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProfileSectionCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedProfileSectionCheckCount\":" + std::to_string(check_count) +
           ",\"candidateProfileSectionRendererSplitDeclared\":true"
           ",\"candidateProfileSectionOnly\":true"
           ",\"profileSectionChainComplete\":" + json_bool(profile_section_chain_complete) +
           ",\"sourceIntegrityFixtureFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"nodeRendererSplitObservedAsReadOnlyPlan\":true"
           ",\"routeFacingMarkdownStable\":true"
           ",\"routeFacingOutputChanged\":false"
           ",\"nodeRuntimeBehaviorAdded\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"candidateProfileSectionNames\":" +
           json_string_array({"candidate-document-request-package",
                              "candidate-document-submission-precheck",
                              "candidate-document-intake-packet",
                              "candidate-document-material-request",
                              "candidate-document-material-submission-precheck"}) +
           ",\"candidateProfileSectionCount\":" + std::to_string(candidate_profile_section_count) +
           ",\"nodeMigratedCandidateProfileSectionCount\":" +
           std::to_string(node_migrated_candidate_profile_section_count) +
           ",\"candidateProfileSectionRendererModuleImported\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesProfileMaterial\":" + json_bool(mini_kv_consumes_profile_material) +
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentRequired\":true"
           ",\"reviewedRealCandidateDocumentPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"candidateSubmissionAccepted\":false"
           ",\"candidateSubmissionImported\":false"
           ",\"materialSubmissionOpened\":false"
           ",\"materialSubmissionSatisfied\":false"
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
           ",\"moduleSplit\":[\"candidate_profile_section_core\",\"candidate_profile_section_stages\",\"candidate_profile_section_checks\",\"candidate_profile_section_validation\"]"
           ",\"stageCatalog\":" + format_candidate_profile_section_stage_catalog_json() +
           ",\"profileSectionChecks\":" + checks::format_candidate_profile_section_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_candidate_profile_section_validation_json(
               source_published_stage_count,
               source_chain_complete,
               candidate_profile_section_count,
               node_migrated_candidate_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_profile_material,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_candidate_profile_section_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1481 is treated as a renderer ownership split, not a fresh evidence dependency",
                              "v1010 material submission precheck integrity remains the frozen source fixture",
                              "the five candidate profile sections are cataloged without rendering Node Markdown",
                              "mini-kv does not consume candidate documents or reviewed material in this evidence",
                              "router write routing WAL touch service startup runtime probes and execution remain disabled"}) +
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
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string candidate_profile_section_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_candidate_profile_section_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_candidate_profile_section
