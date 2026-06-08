#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace source_preflight =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight;
namespace blockers =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers;
namespace checks =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks;
namespace profiles =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles;
namespace stages =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages;
namespace validation =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1261-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "draft-text-package-review-preflight-closeout-roadmap.md";
constexpr int current_draft_text_package_review_closeout_audit_stage_count = 15;

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
    return stages::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages()
        [static_cast<std::size_t>(current_draft_text_package_review_closeout_audit_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages(),
        current_draft_text_package_review_closeout_audit_stage_count,
        stages::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_release_number());
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages(),
        current_draft_text_package_review_closeout_audit_stage_count);
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_preflight_chain_complete = source_preflight::published_stage_count() == 25;
    const int audit_check_count =
        checks::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_check_count();
    const int audit_blocker_count =
        blockers::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blocker_count();
    const int audit_profile_count =
        profiles::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profile_count();
    const bool audit_catalogs_aligned =
        audit_check_count == audit_blocker_count && audit_blocker_count == audit_profile_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewCloseoutAuditMode\":\"controlled-read-only-disabled-draft-text-package-review-closeout-audit\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceDraftTextPackageReviewPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\""
           ",\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\""
           ",\"sourceDraftTextPackageReviewPreflightFixturePath\":\"fixtures/release/shard-readiness-v810.json\""
           ",\"sourceDraftTextPackageReviewPreflightArchiveRootHint\":\"e/810/\""
           ",\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":" +
           std::to_string(source_preflight::published_stage_count()) +
           ",\"sourceDraftTextPackageReviewPreflightPlannedStageCount\":25"
           ",\"sourceDraftTextPackageReviewPreflightChainComplete\":" + json_bool(source_preflight_chain_complete) +
           ",\"sourceDraftTextPackageReviewPreflightDigestMarker\":" +
           json_string(source_preflight::signed_approval_capture_artifact_draft_text_package_review_preflight_digest_marker()) +
           ",\"draftTextPackageReviewCloseoutAuditStage\":" + json_string(stage.stage) +
           ",\"draftTextPackageReviewCloseoutAuditStageSequence\":" + std::to_string(stage.sequence) +
           ",\"draftTextPackageReviewCloseoutAuditReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewCloseoutAuditReleaseRangeStart\":\"v811\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewCloseoutAuditReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"auditCheckCount\":" + std::to_string(audit_check_count) +
           ",\"auditBlockerCount\":" + std::to_string(audit_blocker_count) +
           ",\"auditProfileCount\":" + std::to_string(audit_profile_count) +
           ",\"auditCatalogsAligned\":" + json_bool(audit_catalogs_aligned) +
           ",\"completedAuditCheckCount\":" +
           std::to_string(current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"draftTextPackageReviewCloseoutAuditDeclared\":true"
           ",\"draftTextPackageReviewCloseoutAuditOnly\":true"
           ",\"sourceDraftTextPackageReviewPreflightPresent\":true"
           ",\"sourceDraftTextPackageReviewPreflightClosed\":true"
           ",\"preflightPackageReopened\":false"
           ",\"draftTextPackagePresent\":false"
           ",\"draftTextPackageParsed\":false"
           ",\"draftTextPackageAccepted\":false"
           ",\"draftTextPackageMaterialized\":false"
           ",\"preflightPackageParsedByAudit\":false"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"signedApprovalArtifactDraftTextGenerated\":false"
           ",\"detachedSignaturePayloadPresent\":false"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"submittedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"readyForCloseoutAudit\":true"
           ",\"readyForDraftTextPackageReview\":false"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"automaticSiblingImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"secretValueStored\":false"
           ",\"credentialValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"rawSignatureMaterialStored\":false"
           ",\"draftTextStored\":false"
           ",\"auditWritesReportFile\":false"
           ",\"auditReadsRuntimeArchive\":false"
           ",\"auditCheckHelperApplied\":true"
           ",\"auditBlockerHelperApplied\":true"
           ",\"auditProfileHelperApplied\":true"
           ",\"auditValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"draft_text_package_review_closeout_audit_core\",\"draft_text_package_review_closeout_audit_stages\",\"draft_text_package_review_closeout_audit_checks\",\"draft_text_package_review_closeout_audit_blockers\",\"draft_text_package_review_closeout_audit_profiles\",\"draft_text_package_review_closeout_audit_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_catalog_json() +
           ",\"auditChecks\":" +
           checks::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks_json(
               current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"auditBlockers\":" +
           blockers::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers_json(
               current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"auditProfiles\":" +
           profiles::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles_json(
               current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"validation\":" +
           validation::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation_json(
               audit_check_count,
               audit_blocker_count,
               audit_profile_count,
               source_preflight_chain_complete,
               current_draft_text_package_review_closeout_audit_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1261 closeout plan is referenced as plan evidence only",
                              "source draft text package review preflight remains frozen at v810",
                              "closeout audit reviews evidence fields without parsing a draft text package",
                              "audit blockers preserve no-package no-grant no-runtime no-write controls",
                              "audit profiles are separated for maintenance",
                              "no approval grant is emitted",
                              "no value import or runtime payload is accepted",
                              "no write route, router activation, WAL touch, or sibling mutation occurs"}) +
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"liveReadAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string signed_approval_capture_artifact_draft_text_package_review_closeout_audit_digest_marker() {
    const int planned_stage_count =
        stages::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_draft_text_package_review_closeout_audit_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_draft_text_package_review_closeout_audit_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit
