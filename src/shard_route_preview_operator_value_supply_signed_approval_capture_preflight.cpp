#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight {
namespace {

constexpr int first_capture_preflight_release_number = 686;
constexpr int current_capture_preflight_stage_count = 20;
constexpr int planned_capture_preflight_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1061-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-preflight-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_capture_preflight_stage_count>
    capture_preflight_stages = {{
        {1,
         "v686",
         "route-preview-value-supply-signed-approval-capture-preflight-request-id-input",
         "v685",
         "fixtures/release/shard-readiness-v685.json",
         "freezes the v685 signed approval template and starts capture preflight request id intake"},
        {2,
         "v687",
         "route-preview-value-supply-signed-approval-capture-preflight-source-template-digest",
         "v686",
         "fixtures/release/shard-readiness-v686.json",
         "binds capture preflight to the source signed approval template digest"},
        {3,
         "v688",
         "route-preview-value-supply-signed-approval-capture-preflight-source-review-digest",
         "v687",
         "fixtures/release/shard-readiness-v687.json",
         "requires source approval packet review digest without trusting rolling current material"},
        {4,
         "v689",
         "route-preview-value-supply-signed-approval-capture-preflight-operator-identity",
         "v688",
         "fixtures/release/shard-readiness-v688.json",
         "mirrors operator identity requirement while keeping the value absent"},
        {5,
         "v690",
         "route-preview-value-supply-signed-approval-capture-preflight-operator-role",
         "v689",
         "fixtures/release/shard-readiness-v689.json",
         "mirrors operator role requirement while keeping approval grant absent"},
        {6,
         "v691",
         "route-preview-value-supply-signed-approval-capture-preflight-approval-timestamp",
         "v690",
         "fixtures/release/shard-readiness-v690.json",
         "keeps approval timestamp as an external required source"},
        {7,
         "v692",
         "route-preview-value-supply-signed-approval-capture-preflight-capture-window",
         "v691",
         "fixtures/release/shard-readiness-v691.json",
         "declares manual capture window id placeholder while keeping the window closed"},
        {8,
         "v693",
         "route-preview-value-supply-signed-approval-capture-preflight-channel-policy",
         "v692",
         "fixtures/release/shard-readiness-v692.json",
         "declares capture channel policy without opening raw endpoints"},
        {9,
         "v694",
         "route-preview-value-supply-signed-approval-capture-preflight-signature-algorithm",
         "v693",
         "fixtures/release/shard-readiness-v693.json",
         "declares signature algorithm policy without parsing signature material"},
        {10,
         "v695",
         "route-preview-value-supply-signed-approval-capture-preflight-signature-redaction",
         "v694",
         "fixtures/release/shard-readiness-v694.json",
         "locks raw signature material out of storage"},
        {11,
         "v696",
         "route-preview-value-supply-signed-approval-capture-preflight-approval-statement",
         "v695",
         "fixtures/release/shard-readiness-v695.json",
         "requires approval statement placeholder without fabricating approval text"},
        {12,
         "v697",
         "route-preview-value-supply-signed-approval-capture-preflight-operator-justification",
         "v696",
         "fixtures/release/shard-readiness-v696.json",
         "mirrors operator justification requirement while keeping it absent"},
        {13,
         "v698",
         "route-preview-value-supply-signed-approval-capture-preflight-source-evidence-version",
         "v697",
         "fixtures/release/shard-readiness-v697.json",
         "mirrors source evidence version and rejects rolling current evidence"},
        {14,
         "v699",
         "route-preview-value-supply-signed-approval-capture-preflight-source-file-id",
         "v698",
         "fixtures/release/shard-readiness-v698.json",
         "mirrors source file id without runtime filesystem lookup"},
        {15,
         "v700",
         "route-preview-value-supply-signed-approval-capture-preflight-source-snippet-id",
         "v699",
         "fixtures/release/shard-readiness-v699.json",
         "mirrors source snippet id without runtime source resolution"},
        {16,
         "v701",
         "route-preview-value-supply-signed-approval-capture-preflight-redacted-value-digest",
         "v700",
         "fixtures/release/shard-readiness-v700.json",
         "requires redacted value digest reference and rejects raw values"},
        {17,
         "v702",
         "route-preview-value-supply-signed-approval-capture-preflight-value-shape-binding",
         "v701",
         "fixtures/release/shard-readiness-v701.json",
         "requires typed value shape binding while accepting no values"},
        {18,
         "v703",
         "route-preview-value-supply-signed-approval-capture-preflight-redaction-policy",
         "v702",
         "fixtures/release/shard-readiness-v702.json",
         "mirrors redaction policy acknowledgement as required and absent"},
        {19,
         "v704",
         "route-preview-value-supply-signed-approval-capture-preflight-provenance-policy",
         "v703",
         "fixtures/release/shard-readiness-v703.json",
         "mirrors provenance policy while import remains disabled"},
        {20,
         "v705",
         "route-preview-value-supply-signed-approval-capture-preflight-raw-material-lock",
         "v704",
         "fixtures/release/shard-readiness-v704.json",
         "locks credential, secret, endpoint, value, and signature material out of storage"},
        {21,
         "v706",
         "route-preview-value-supply-signed-approval-capture-preflight-grant-lock",
         "v705",
         "fixtures/release/shard-readiness-v705.json",
         "audits that no approval grant is emitted or accepted"},
        {22,
         "v707",
         "route-preview-value-supply-signed-approval-capture-preflight-value-submission-lock",
         "v706",
         "fixtures/release/shard-readiness-v706.json",
         "audits zero submitted, accepted, imported, and persisted operator values"},
        {23,
         "v708",
         "route-preview-value-supply-signed-approval-capture-preflight-write-route-lock",
         "v707",
         "fixtures/release/shard-readiness-v707.json",
         "audits that no write route or active router is installed"},
        {24,
         "v709",
         "route-preview-value-supply-signed-approval-capture-preflight-sibling-non-mutation",
         "v708",
         "fixtures/release/shard-readiness-v708.json",
         "audits that Node, Java, and sibling evidence remain unmodified"},
        {25,
         "v710",
         "route-preview-value-supply-signed-approval-capture-preflight-closeout",
         "v709",
         "fixtures/release/shard-readiness-v709.json",
         "packages capture preflight closeout for a future signed artifact review only"},
    }};

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
    return capture_preflight_stages.at(static_cast<std::size_t>(current_capture_preflight_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        capture_preflight_stages,
        current_capture_preflight_stage_count,
        planned_capture_preflight_stage_count,
        first_capture_preflight_release_number);
}

} // namespace

std::string format_signed_approval_capture_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        capture_preflight_stages,
        current_capture_preflight_stage_count);
}

std::string format_signed_approval_capture_preflight_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_operator_value_supply_signed_approval_template::published_stage_count() == 25;
    const int input_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs::
            planned_signed_approval_capture_preflight_input_count();
    const int policy_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies::
            planned_signed_approval_capture_preflight_policy_count();
    const int attestation_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations::
            planned_signed_approval_capture_preflight_attestation_count();
    const bool catalogs_aligned = input_count == policy_count && policy_count == attestation_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-preflight.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\""
           ",\"signedApprovalCapturePreflightMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-capture-preflight\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceSignedApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\""
           ",\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\""
           ",\"sourceSignedApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v685.json\""
           ",\"sourceSignedApprovalTemplateArchiveRootHint\":\"e/685/\""
           ",\"sourceSignedApprovalTemplatePublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_signed_approval_template::published_stage_count()) +
           ",\"sourceSignedApprovalTemplatePlannedStageCount\":25"
           ",\"sourceSignedApprovalTemplateChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceSignedApprovalTemplateDigestMarker\":" +
           json_string(shard_route_preview_operator_value_supply_signed_approval_template::
                           signed_approval_template_digest_marker()) +
           ",\"signedApprovalCapturePreflightStage\":" + json_string(stage.stage) +
           ",\"signedApprovalCapturePreflightStageSequence\":" + std::to_string(stage.sequence) +
           ",\"signedApprovalCapturePreflightReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_capture_preflight_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_capture_preflight_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceSignedApprovalTemplateReleaseRangeStart\":\"v661\""
           ",\"sourceSignedApprovalTemplateReleaseRangeEnd\":\"v685\""
           ",\"sourceSignedApprovalTemplateReleaseCount\":25"
           ",\"signedApprovalCapturePreflightReleaseRangeStart\":\"v686\""
           ",\"signedApprovalCapturePreflightReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"signedApprovalCapturePreflightReleaseCount\":" +
           std::to_string(current_capture_preflight_stage_count) +
           ",\"expectedSignedApprovalCapturePreflightInputCount\":25"
           ",\"signedApprovalCapturePreflightInputCount\":" + std::to_string(input_count) +
           ",\"signedApprovalCapturePreflightPolicyCount\":" + std::to_string(policy_count) +
           ",\"signedApprovalCapturePreflightAttestationCount\":" + std::to_string(attestation_count) +
           ",\"signedApprovalCapturePreflightCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"publishedSignedApprovalCapturePreflightInputCount\":" +
           std::to_string(current_capture_preflight_stage_count) +
           ",\"operatorValueSupplyApprovalPacketState\":\"capture-preflight-required-no-approval-present\""
           ",\"signedApprovalCapturePreflightState\":\"preflight-only-no-signature-captured\""
           ",\"signedApprovalCapturePreflightDeclared\":true"
           ",\"signedApprovalCapturePreflightOnly\":true"
           ",\"capturePreflightOnly\":true"
           ",\"signedApprovalCapturePreflightDisabledByDefault\":true"
           ",\"signedApprovalTemplateRequired\":true"
           ",\"signedApprovalTemplatePresent\":true"
           ",\"signedApprovalRequired\":true"
           ",\"signedApprovalPresent\":false"
           ",\"signedApprovalAccepted\":false"
           ",\"signatureCaptureRequired\":true"
           ",\"signatureCaptureAllowed\":false"
           ",\"signatureCaptured\":false"
           ",\"signatureAccepted\":false"
           ",\"approvalGrantRequired\":true"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantAccepted\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"capturePreflightRequestIdRequired\":true"
           ",\"capturePreflightRequestIdPresent\":false"
           ",\"sourceTemplateDigestRequired\":true"
           ",\"sourceTemplateDigestBound\":true"
           ",\"sourceReviewDigestRequired\":true"
           ",\"sourceReviewDigestPresent\":false"
           ",\"operatorIdentityRequired\":true"
           ",\"operatorIdentityPresent\":false"
           ",\"operatorRoleRequired\":true"
           ",\"operatorRolePresent\":false"
           ",\"approvalTimestampRequired\":true"
           ",\"approvalTimestampPresent\":false"
           ",\"manualCaptureWindowRequired\":true"
           ",\"manualCaptureWindowOpen\":false"
           ",\"captureChannelPolicyRequired\":true"
           ",\"captureChannelOpen\":false"
           ",\"signatureAlgorithmPolicyRequired\":true"
           ",\"signatureMaterialAccepted\":false"
           ",\"approvalStatementRequired\":true"
           ",\"approvalStatementPresent\":false"
           ",\"sourceEvidenceVersionRequired\":true"
           ",\"sourceEvidenceVersionPresent\":false"
           ",\"sourceEvidenceFileIdRequired\":true"
           ",\"sourceEvidenceFileIdPresent\":false"
           ",\"sourceEvidenceSnippetIdRequired\":true"
           ",\"sourceEvidenceSnippetIdPresent\":false"
           ",\"redactedValueDigestRequired\":true"
           ",\"redactedValueDigestSupplied\":false"
           ",\"valueShapeBindingRequired\":true"
           ",\"valueShapeBindingPresent\":false"
           ",\"redactionPolicyRequired\":true"
           ",\"redactionPolicyAcknowledged\":false"
           ",\"provenancePolicyRequired\":true"
           ",\"provenancePolicyPresent\":false"
           ",\"missingSignedApprovalRejected\":true"
           ",\"malformedSignedApprovalRejected\":true"
           ",\"rawSignatureMaterialRejected\":true"
           ",\"rawSecretMaterialRejected\":true"
           ",\"missingValueRejected\":true"
           ",\"malformedValueRejected\":true"
           ",\"submittedOperatorValueCount\":0"
           ",\"suppliedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"signedApprovalCapturePreflightInputHelper\":\"shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs\""
           ",\"signedApprovalCapturePreflightPolicyHelper\":\"shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies\""
           ",\"signedApprovalCapturePreflightAttestationHelper\":\"shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations\""
           ",\"signedApprovalCapturePreflightValidationHelper\":\"shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"signedApprovalCapturePreflightStageCatalog\":" +
           format_signed_approval_capture_preflight_stage_catalog_json() +
           ",\"signedApprovalCapturePreflightStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"signedApprovalCapturePreflightInputs\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs::
               format_signed_approval_capture_preflight_inputs_json(current_capture_preflight_stage_count) +
           ",\"signedApprovalCapturePreflightPolicies\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies::
               format_signed_approval_capture_preflight_policies_json(current_capture_preflight_stage_count) +
           ",\"signedApprovalCapturePreflightAttestations\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations::
               format_signed_approval_capture_preflight_attestations_json(current_capture_preflight_stage_count) +
           ",\"signedApprovalCapturePreflightValidation\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation::
               format_signed_approval_capture_preflight_validation_json(input_count,
                                                                        policy_count,
                                                                        attestation_count,
                                                                        current_capture_preflight_stage_count) +
           ",\"signedApprovalCapturePreflightChecks\":" +
           json_string_array({"source signed approval template command remains available",
                              "source signed approval template chain remains complete",
                              "source signed approval template digest marker remains pinned",
                              "Node v1061 capture preflight closeout plan is referenced as plan evidence only",
                              "capture preflight inputs are cataloged separately",
                              "capture preflight policies are cataloged separately",
                              "capture preflight attestations are cataloged separately",
                              "input, policy, and attestation counts align",
                              "capture request id is required and not minted by mini-kv",
                              "source review digest remains required and absent",
                              "operator identity, role, timestamp, and justification remain absent",
                              "manual capture window and channel remain closed",
                              "signature algorithm policy is metadata only",
                              "raw signature and secret material are rejected",
                              "approval statement remains absent",
                              "source version, file id, and snippet id remain absent",
                              "redacted value digest, value shape, redaction policy, and provenance remain absent",
                              "approval grant remains absent and not emitted",
                              "operator value submission, acceptance, import, and persistence counts remain zero",
                              "automatic sibling import is disabled",
                              "runtime payload acceptance is disabled",
                              "credential, secret, raw endpoint, and raw signature values are not stored",
                              "current SHARDJSON embeds exactly one signed approval capture preflight section",
                              "Node, Java, and sibling evidence are not mutated",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"signedApprovalCapturePreflightCheckCount\":25"
           ",\"signedApprovalCapturePreflightPassedCount\":25"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"signedApprovalCapturePreflightInputHelperApplied\":true"
           ",\"signedApprovalCapturePreflightPolicyHelperApplied\":true"
           ",\"signedApprovalCapturePreflightAttestationHelperApplied\":true"
           ",\"signedApprovalCapturePreflightValidationHelperApplied\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleSignedApprovalCapturePreflightSectionExpected\":true"
           ",\"signedApprovalCapturePreflightChainComplete\":" + json_bool(stage_chain.chain_complete) +
           ",\"readyForSignedApprovalCapturePreflightReview\":true"
           ",\"readyForSignedApprovalCaptureArtifact\":false"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"readyForOperatorValueSupplyApproval\":false"
           ",\"readyForOperatorValueSupply\":false"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForRuntimePayloadAcceptance\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"operatorValueSupplyApprovalAllowed\":false"
           ",\"operatorValueSubmitted\":false"
           ",\"operatorValueSupplied\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorEvidenceValuesAccepted\":false"
           ",\"operatorValuesPersisted\":false"
           ",\"sourceEvidenceProvenanceRequired\":true"
           ",\"sourceEvidenceProvenanceSupplied\":false"
           ",\"automaticSiblingImportAllowed\":false"
           ",\"evidenceImportAllowed\":false"
           ",\"manualEvidenceImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"syntheticEvidenceRejected\":true"
           ",\"secretValueStored\":false"
           ",\"credentialValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"rawSignatureMaterialStored\":false"
           ",\"redactionPolicyDeclared\":true"
           ",\"redactionPolicyPreviewOnly\":true"
           ",\"normalizerExecutes\":false"
           ",\"normalizationExecutionAllowed\":false"
           ",\"signedApprovalCapturePreflightValuesPersisted\":false"
           ",\"capturePreflightPackageOnly\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"signed approval capture",
                              "approval grant emission or acceptance",
                              "operator value supply approval",
                              "operator value submission",
                              "operator evidence value import",
                              "automatic sibling evidence import",
                              "manual evidence entry",
                              "runtime payload acceptance",
                              "synthetic evidence normalization",
                              "credential, secret, endpoint, or signature storage",
                              "raw endpoint capture channel",
                              "active shard router installation",
                              "write routing",
                              "restore, load, compact, service start, or live read execution"}) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read signed approval capture preflight evidence only",
                              "treat v661-v685 signed approval template release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat capture request id, review digest, operator fields, timestamp, and justification as absent",
                              "treat signed approval, signature capture, approval statement, and approval grant as absent",
                              "keep capture window and capture channel closed",
                              "reject raw signature, secret, credential, endpoint, and value material",
                              "do not capture a signature in mini-kv",
                              "do not emit or accept approval grants through mini-kv",
                              "do not submit, approve, import, or persist operator evidence values through mini-kv",
                              "do not perform automatic sibling evidence import",
                              "do not accept runtime payloads as value supply",
                              "do not normalize synthetic evidence into accepted evidence",
                              "do not mutate Node, Java, or sibling evidence",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           route_preview_evidence_boundary::format_no_execution_boundary_fields() + "}";
}

std::string signed_approval_capture_preflight_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_capture_preflight_stage_count,
        planned_capture_preflight_stage_count);
}

int published_stage_count() {
    return current_capture_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight
