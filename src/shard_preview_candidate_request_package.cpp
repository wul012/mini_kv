#include "minikv/shard_preview_candidate_request_package.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_request_package_checks.hpp"
#include "minikv/shard_preview_candidate_request_package_gates.hpp"
#include "minikv/shard_preview_candidate_request_package_items.hpp"
#include "minikv/shard_preview_candidate_request_package_stages.hpp"
#include "minikv/shard_preview_candidate_request_package_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_request_package {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_request_package_checks;
namespace gates = shard_preview_candidate_request_package_gates;
namespace items = shard_preview_candidate_request_package_items;
namespace source_intake =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit;
namespace stages = shard_preview_candidate_request_package_stages;
namespace validation = shard_preview_candidate_request_package_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/v1386-controlled-read-only-shard-preview-candidate-document-request-package-closeout-roadmap.md";
constexpr int current_candidate_document_request_package_stage_count = 15;
constexpr int requested_candidate_field_count = 20;

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
    return stages::candidate_document_request_package_stages()
        [static_cast<std::size_t>(current_candidate_document_request_package_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_document_request_package_stages(),
        current_candidate_document_request_package_stage_count,
        stages::planned_candidate_document_request_package_stage_count(),
        stages::first_candidate_document_request_package_release_number());
}

int completed_request_item_count() {
    return current_candidate_document_request_package_stage_count;
}

int completed_acceptance_check_count() {
    return current_candidate_document_request_package_stage_count;
}

int completed_request_gate_count() {
    return std::min(gates::planned_candidate_document_request_gate_count(),
                    (current_candidate_document_request_package_stage_count *
                         gates::planned_candidate_document_request_gate_count() +
                     stages::planned_candidate_document_request_package_stage_count() - 1) /
                        stages::planned_candidate_document_request_package_stage_count());
}

} // namespace

std::string format_candidate_document_request_package_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_document_request_package_stages(),
        current_candidate_document_request_package_stage_count);
}

std::string candidate_document_request_package_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_document_request_package_closeout_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_intake_audit_chain_complete = source_intake::published_stage_count() == 20;
    const int planned_item_count = items::planned_candidate_document_request_item_count();
    const int planned_check_count = checks::planned_candidate_document_acceptance_check_count();
    const int planned_gate_count = gates::planned_candidate_document_request_gate_count();
    const int item_count = completed_request_item_count();
    const int check_count = completed_acceptance_check_count();
    const int gate_count = completed_request_gate_count();
    const bool planned_catalogs_aligned =
        planned_item_count == 15 && planned_check_count == 15 && planned_gate_count == 38;
    const bool closeout_complete = current_candidate_document_request_package_stage_count == 15;

    return "{\"contract\":\"shard-route-preview-candidate-document-request-package-closeout.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON\""
           ",\"candidateDocumentRequestPackageMode\":\"controlled-read-only-disabled-candidate-document-request-package-closeout\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceComparedPackageEvidenceIntakeAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\""
           ",\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\""
           ",\"sourceComparedPackageEvidenceIntakeAuditFixturePath\":\"fixtures/release/shard-readiness-v880.json\""
           ",\"sourceComparedPackageEvidenceIntakeAuditArchiveRootHint\":\"e/880/\""
           ",\"sourceComparedPackageEvidenceIntakeAuditPublishedStageCount\":" +
           std::to_string(source_intake::published_stage_count()) +
           ",\"sourceComparedPackageEvidenceIntakeAuditPlannedStageCount\":20"
           ",\"sourceComparedPackageEvidenceIntakeAuditChainComplete\":" +
           json_bool(source_intake_audit_chain_complete) +
           ",\"sourceComparedPackageEvidenceIntakeAuditDigestMarker\":" +
           json_string(source_intake::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_digest_marker()) +
           ",\"candidateDocumentRequestPackageStage\":" + json_string(stage.stage) +
           ",\"candidateDocumentRequestPackageStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateDocumentRequestPackageReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateDocumentRequestPackageReleaseRangeStart\":\"v881\""
           ",\"candidateDocumentRequestPackageReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_candidate_document_request_package_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_candidate_document_request_package_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedRequestItemCount\":" + std::to_string(planned_item_count) +
           ",\"plannedAcceptanceCheckCount\":" + std::to_string(planned_check_count) +
           ",\"plannedRequestGateCount\":" + std::to_string(planned_gate_count) +
           ",\"completedRequestItemCount\":" + std::to_string(item_count) +
           ",\"completedAcceptanceCheckCount\":" + std::to_string(check_count) +
           ",\"completedRequestGateCount\":" + std::to_string(gate_count) +
           ",\"requestedCandidateFieldCount\":" + std::to_string(requested_candidate_field_count) +
           ",\"plannedCatalogsAligned\":" + json_bool(planned_catalogs_aligned) +
           ",\"candidateDocumentRequestPackageDeclared\":true"
           ",\"candidateDocumentRequestPackageCloseoutOnly\":true"
           ",\"requestPackageReadyForFutureRealDocumentIntake\":" + json_bool(closeout_complete) +
           ",\"sourceComparedPackageEvidenceIntakeAuditPresent\":true"
           ",\"sourceComparedPackageEvidenceIntakeAuditClosed\":true"
           ",\"realCandidateDocumentRequired\":true"
           ",\"realCandidateDocumentPresent\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"syntheticCandidateDocumentAccepted\":false"
           ",\"unreviewedCandidateDocumentAccepted\":false"
           ",\"stagedCandidatePayloadImported\":false"
           ",\"candidatePayloadAccepted\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"candidateDocumentRejected\":false"
           ",\"candidateApprovalGrantCaptured\":false"
           ",\"candidateSignedApprovalEmitted\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
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
           ",\"candidateDocumentBodyStored\":false"
           ",\"packageBodyStored\":false"
           ",\"requestItemHelperApplied\":true"
           ",\"acceptanceCheckHelperApplied\":true"
           ",\"requestGateHelperApplied\":true"
           ",\"requestValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"candidate_request_package_core\",\"candidate_request_package_stages\",\"candidate_request_package_items\",\"candidate_request_package_checks\",\"candidate_request_package_gates\",\"candidate_request_package_validation\"]"
           ",\"stageCatalog\":" + format_candidate_document_request_package_stage_catalog_json() +
           ",\"requestItems\":" + items::format_candidate_document_request_items_json(item_count) +
           ",\"acceptanceChecks\":" + checks::format_candidate_document_acceptance_checks_json(check_count) +
           ",\"requestGates\":" + gates::format_candidate_document_request_gates_json(gate_count) +
           ",\"validation\":" +
           validation::format_candidate_document_request_package_validation_json(planned_item_count,
                                                                               planned_check_count,
                                                                               planned_gate_count,
                                                                               item_count,
                                                                               check_count,
                                                                               gate_count,
                                                                               source_intake_audit_chain_complete,
                                                                               current_candidate_document_request_package_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1386 candidate document request package plan is referenced as downstream contract context only",
                              "source compared package evidence intake audit remains frozen at v880",
                              "mini-kv exposes request items checks and gates without creating candidate documents",
                              "missing real candidate document remains a blocked state",
                              "synthetic unreviewed staged imported evaluated accepted and rejected documents remain disabled",
                              "approval grant capture and signed approval emission remain separated and disabled",
                              "runtime payload import sibling starts archive walks WAL touch writes and execution remain disabled"}) +
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

std::string candidate_document_request_package_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_document_request_package_stage_count,
        stages::planned_candidate_document_request_package_stage_count());
}

int published_stage_count() {
    return current_candidate_document_request_package_stage_count;
}

} // namespace minikv::shard_preview_candidate_request_package
