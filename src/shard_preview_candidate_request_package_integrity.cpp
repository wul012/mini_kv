#include "minikv/shard_preview_candidate_request_package_integrity.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_request_package.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity_checks.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity_stages.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_request_package_integrity {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_request_package_integrity_checks;
namespace source_package = shard_preview_candidate_request_package;
namespace stages = shard_preview_candidate_request_package_integrity_stages;
namespace validation = shard_preview_candidate_request_package_integrity_validation;

constexpr std::string_view integrity_command = "SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1386-controlled-read-only-shard-preview-candidate-document-request-package-closeout-roadmap.md";
constexpr int current_candidate_request_package_integrity_stage_count = 25;
constexpr int source_candidate_request_package_planned_stage_count = 15;
constexpr int source_candidate_request_package_request_item_count = 15;
constexpr int source_candidate_request_package_acceptance_check_count = 15;
constexpr int source_candidate_request_package_gate_count = 38;
constexpr int source_candidate_request_package_requested_field_count = 20;

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
    return stages::candidate_request_package_integrity_stages()
        [static_cast<std::size_t>(current_candidate_request_package_integrity_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_request_package_integrity_stages(),
        current_candidate_request_package_integrity_stage_count,
        stages::planned_candidate_request_package_integrity_stage_count(),
        stages::first_candidate_request_package_integrity_release_number());
}

int completed_integrity_check_count() {
    return current_candidate_request_package_integrity_stage_count;
}

} // namespace

std::string format_candidate_request_package_integrity_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_request_package_integrity_stages(),
        current_candidate_request_package_integrity_stage_count);
}

std::string candidate_request_package_integrity_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_request_package_integrity_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_published_stage_count = source_package::published_stage_count();
    const bool source_chain_complete =
        source_published_stage_count == source_candidate_request_package_planned_stage_count;
    const int planned_check_count = checks::planned_candidate_request_package_integrity_check_count();
    const int check_count = completed_integrity_check_count();
    const bool integrity_chain_complete =
        current_candidate_request_package_integrity_stage_count ==
            stages::planned_candidate_request_package_integrity_stage_count() &&
        source_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-request-package-fixture-integrity.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(integrity_command) +
           ",\"candidateRequestPackageIntegrityMode\":\"controlled-read-only-fixture-integrity-hardening\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceCandidateDocumentRequestPackageCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON\""
           ",\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\""
           ",\"sourceCandidateDocumentRequestPackageFixturePath\":\"fixtures/release/shard-readiness-v895.json\""
           ",\"sourceCandidateDocumentRequestPackageArchiveRootHint\":\"e/895/\""
           ",\"sourceCandidateDocumentRequestPackagePublishedStageCount\":" +
           std::to_string(source_published_stage_count) +
           ",\"sourceCandidateDocumentRequestPackagePlannedStageCount\":" +
           std::to_string(source_candidate_request_package_planned_stage_count) +
           ",\"sourceCandidateDocumentRequestPackageChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceCandidateDocumentRequestPackageDigestMarker\":" +
           json_string(source_package::candidate_document_request_package_digest_marker()) +
           ",\"sourceCandidateDocumentRequestPackageStatus\":" +
           json_string(source_package::candidate_document_request_package_status()) +
           ",\"sourceCandidateDocumentRequestPackageRequestItemCount\":" +
           std::to_string(source_candidate_request_package_request_item_count) +
           ",\"sourceCandidateDocumentRequestPackageAcceptanceCheckCount\":" +
           std::to_string(source_candidate_request_package_acceptance_check_count) +
           ",\"sourceCandidateDocumentRequestPackageGateCount\":" +
           std::to_string(source_candidate_request_package_gate_count) +
           ",\"sourceCandidateDocumentRequestPackageRequestedFieldCount\":" +
           std::to_string(source_candidate_request_package_requested_field_count) +
           ",\"candidateRequestPackageIntegrityStage\":" + json_string(stage.stage) +
           ",\"candidateRequestPackageIntegrityStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateRequestPackageIntegrityReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateRequestPackageIntegrityReleaseRangeStart\":\"v896\""
           ",\"candidateRequestPackageIntegrityReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_candidate_request_package_integrity_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_candidate_request_package_integrity_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(check_count) +
           ",\"candidateRequestPackageIntegrityDeclared\":true"
           ",\"candidateRequestPackageIntegrityOnly\":true"
           ",\"sourceCandidateDocumentRequestPackageFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"currentFixtureCarriesIntegritySection\":true"
           ",\"sourcePackageReadyForFutureRealDocumentIntake\":true"
           ",\"integrityChainComplete\":" + json_bool(integrity_chain_complete) +
           ",\"additionalRequestEchoCreated\":false"
           ",\"newCandidateDocumentRequestPackageCreated\":false"
           ",\"realCandidateDocumentRequired\":true"
           ",\"realCandidateDocumentPresent\":false"
           ",\"realCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"syntheticCandidateDocumentAccepted\":false"
           ",\"unreviewedCandidateDocumentAccepted\":false"
           ",\"candidatePayloadImported\":false"
           ",\"stagedCandidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"candidateDocumentRejected\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"candidateApprovalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"candidateSignedApprovalEmitted\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"moduleSplit\":[\"candidate_request_package_integrity_core\",\"candidate_request_package_integrity_stages\",\"candidate_request_package_integrity_checks\",\"candidate_request_package_integrity_validation\"]"
           ",\"stageCatalog\":" + format_candidate_request_package_integrity_stage_catalog_json() +
           ",\"integrityChecks\":" +
           checks::format_candidate_request_package_integrity_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_candidate_request_package_integrity_validation_json(
               source_published_stage_count,
               source_chain_complete,
               planned_check_count,
               check_count,
               current_candidate_request_package_integrity_stage_count,
               stages::planned_candidate_request_package_integrity_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1386 stop condition is honored by auditing the frozen request package instead of creating another request echo",
                              "v895 is versioned as fixtures/release/shard-readiness-v895.json before v896-v920 integrity hardening",
                              "mini-kv validates fixture lineage command catalog visibility and no side effect boundaries only",
                              "real candidate document intake remains closed until a reviewed real document exists",
                              "document router write routing WAL touch runtime payload import approval capture signature emission and execution remain disabled"}) +
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

std::string candidate_request_package_integrity_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_request_package_integrity_stage_count,
        stages::planned_candidate_request_package_integrity_stage_count());
}

int published_stage_count() {
    return current_candidate_request_package_integrity_stage_count;
}

} // namespace minikv::shard_preview_candidate_request_package_integrity
