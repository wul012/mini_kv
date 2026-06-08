#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace source_comparison_closeout_audit =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit;
namespace guards = shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards;
namespace slots = shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots;
namespace stages = shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages;
namespace validation = shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1331-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "draft-text-package-compared-package-evidence-intake-closeout-roadmap.md";
constexpr int current_compared_package_evidence_intake_audit_stage_count = 9;

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
    return stages::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages()
        [static_cast<std::size_t>(current_compared_package_evidence_intake_audit_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages(),
        current_compared_package_evidence_intake_audit_stage_count,
        stages::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_release_number());
}

int completed_slot_count() {
    return (current_compared_package_evidence_intake_audit_stage_count + 1) / 2;
}

int completed_guard_count() {
    return current_compared_package_evidence_intake_audit_stage_count / 2;
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages(),
        current_compared_package_evidence_intake_audit_stage_count);
}

std::string format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_comparison_closeout_audit_chain_complete = source_comparison_closeout_audit::published_stage_count() == 25;
    const int planned_slot_count =
        slots::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slot_count();
    const int planned_guard_count =
        guards::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guard_count();
    const int slot_count = completed_slot_count();
    const int guard_count = completed_guard_count();
    const bool planned_catalogs_aligned = planned_slot_count == 10 && planned_guard_count == 10;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-audit.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageComparedPackageEvidenceIntakeAuditMode\":\"controlled-read-only-disabled-compared-package-evidence-intake-audit\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v860.json\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditArchiveRootHint\":\"e/860/\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditPublishedStageCount\":" +
           std::to_string(source_comparison_closeout_audit::published_stage_count()) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditPlannedStageCount\":25"
           ",\"sourceDraftTextPackageComparisonCloseoutAuditChainComplete\":" + json_bool(source_comparison_closeout_audit_chain_complete) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditDigestMarker\":" +
           json_string(source_comparison_closeout_audit::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_digest_marker()) +
           ",\"draftTextPackageComparedPackageEvidenceIntakeAuditStage\":" + json_string(stage.stage) +
           ",\"draftTextPackageComparedPackageEvidenceIntakeAuditStageSequence\":" + std::to_string(stage.sequence) +
           ",\"draftTextPackageComparedPackageEvidenceIntakeAuditReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactDraftTextPackageComparedPackageEvidenceIntakeAuditReleaseRangeStart\":\"v861\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageComparedPackageEvidenceIntakeAuditReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_compared_package_evidence_intake_audit_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedEvidenceSlotCount\":" + std::to_string(planned_slot_count) +
           ",\"plannedIntakeGuardCount\":" + std::to_string(planned_guard_count) +
           ",\"completedEvidenceSlotCount\":" + std::to_string(slot_count) +
           ",\"completedIntakeGuardCount\":" + std::to_string(guard_count) +
           ",\"plannedCatalogsAligned\":" + json_bool(planned_catalogs_aligned) +
           ",\"comparedPackageEvidenceIntakeAuditDeclared\":true"
           ",\"comparedPackageEvidenceIntakeAuditOnly\":true"
           ",\"sourceDraftTextPackageComparisonCloseoutAuditPresent\":true"
           ",\"sourceDraftTextPackageComparisonCloseoutAuditClosed\":true"
           ",\"sourceAcceptancePrecheckEvidenceDeclared\":" + json_bool(slot_count >= 1) +
           ",\"realComparedPackageEvidenceRequired\":true"
           ",\"comparedPackageEvidencePresent\":false"
           ",\"comparedPackageEvidenceFabricated\":false"
           ",\"syntheticComparedPackageEvidenceAccepted\":false"
           ",\"manualComparedPackageParsed\":false"
           ",\"comparedPackageAccepted\":false"
           ",\"comparedPackageMaterialized\":false"
           ",\"submittedDraftTextPackageParsed\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"approvalGrantPresent\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"submittedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"secretValueStored\":false"
           ",\"credentialValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"rawSignatureMaterialStored\":false"
           ",\"draftTextStored\":false"
           ",\"packageBodyStored\":false"
           ",\"readyForComparedPackageEvidenceIntake\":false"
           ",\"readyForComparedPackageAcceptance\":false"
           ",\"readyForApprovalGrantReview\":false"
           ",\"evidenceSlotHelperApplied\":true"
           ",\"intakeGuardHelperApplied\":true"
           ",\"intakeValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"compared_package_evidence_intake_audit_core\",\"compared_package_evidence_intake_audit_stages\",\"compared_package_evidence_intake_audit_slots\",\"compared_package_evidence_intake_audit_guards\",\"compared_package_evidence_intake_audit_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_catalog_json() +
           ",\"evidenceSlots\":" +
           slots::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots_json(slot_count) +
           ",\"intakeGuards\":" +
           guards::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards_json(guard_count) +
           ",\"validation\":" +
           validation::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation_json(
               planned_slot_count,
               planned_guard_count,
               slot_count,
               guard_count,
               source_comparison_closeout_audit_chain_complete,
               current_compared_package_evidence_intake_audit_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1331 compared package evidence intake plan is referenced as downstream contract context only",
                              "source comparison closeout audit remains frozen at v860",
                              "mini-kv declares evidence slots and guards without fabricating compared package evidence",
                              "manual compared package payload is absent and unparsed",
                              "approval grant capture remains separated and disabled",
                              "runtime payload import and sibling service starts remain disabled",
                              "no write route, router activation, WAL touch, or execution occurs"}) +
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
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_digest_marker() {
    const int planned_stage_count =
        stages::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_compared_package_evidence_intake_audit_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_compared_package_evidence_intake_audit_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit