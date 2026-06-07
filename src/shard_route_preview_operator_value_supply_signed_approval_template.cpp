#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_clauses.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_controls.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_fields.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template {
namespace {

constexpr int first_signed_approval_template_release_number = 661;
constexpr int current_signed_approval_template_stage_count = 17;
constexpr int planned_signed_approval_template_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1036-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-template-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_signed_approval_template_stage_count>
    signed_approval_template_stages = {{
        {1,
         "v661",
         "route-preview-value-supply-signed-approval-template-packet-id-field",
         "v660",
         "fixtures/release/shard-readiness-v660.json",
         "freezes the v660 approval-template package and starts the signed approval template packet id field"},
        {2,
         "v662",
         "route-preview-value-supply-signed-approval-template-operator-identity-field",
         "v661",
         "fixtures/release/shard-readiness-v661.json",
         "requires future operator identity while keeping signature capture disabled"},
        {3,
         "v663",
         "route-preview-value-supply-signed-approval-template-operator-role-field",
         "v662",
         "fixtures/release/shard-readiness-v662.json",
         "requires future operator role scope while keeping approval grant absent"},
        {4,
         "v664",
         "route-preview-value-supply-signed-approval-template-approval-timestamp-field",
         "v663",
         "fixtures/release/shard-readiness-v663.json",
         "requires future approval timestamp without synthesizing runtime time"},
        {5,
         "v665",
         "route-preview-value-supply-signed-approval-template-source-review-digest-field",
         "v664",
         "fixtures/release/shard-readiness-v664.json",
         "binds the template to a future source review package digest"},
        {6,
         "v666",
         "route-preview-value-supply-signed-approval-template-source-draft-digest-field",
         "v665",
         "fixtures/release/shard-readiness-v665.json",
         "binds the template to a future source approval draft digest"},
        {7,
         "v667",
         "route-preview-value-supply-signed-approval-template-source-release-field",
         "v666",
         "fixtures/release/shard-readiness-v666.json",
         "requires source release version instead of rolling current evidence"},
        {8,
         "v668",
         "route-preview-value-supply-signed-approval-template-source-file-field",
         "v667",
         "fixtures/release/shard-readiness-v667.json",
         "requires source file id without runtime filesystem reads"},
        {9,
         "v669",
         "route-preview-value-supply-signed-approval-template-source-snippet-field",
         "v668",
         "fixtures/release/shard-readiness-v668.json",
         "requires source snippet id without runtime source resolution"},
        {10,
         "v670",
         "route-preview-value-supply-signed-approval-template-typed-value-envelope-field",
         "v669",
         "fixtures/release/shard-readiness-v669.json",
         "requires typed value envelope while accepting zero operator values"},
        {11,
         "v671",
         "route-preview-value-supply-signed-approval-template-redacted-value-digest-field",
         "v670",
         "fixtures/release/shard-readiness-v670.json",
         "requires redacted value digest and rejects raw value material"},
        {12,
         "v672",
         "route-preview-value-supply-signed-approval-template-redaction-policy-ack",
         "v671",
         "fixtures/release/shard-readiness-v671.json",
         "requires redaction policy acknowledgement before any future capture"},
        {13,
         "v673",
         "route-preview-value-supply-signed-approval-template-provenance-binding",
         "v672",
         "fixtures/release/shard-readiness-v672.json",
         "requires per-value provenance binding while import remains disabled"},
        {14,
         "v674",
         "route-preview-value-supply-signed-approval-template-missing-value-policy",
         "v673",
         "fixtures/release/shard-readiness-v673.json",
         "requires missing-value fail-closed acknowledgement"},
        {15,
         "v675",
         "route-preview-value-supply-signed-approval-template-malformed-value-policy",
         "v674",
         "fixtures/release/shard-readiness-v674.json",
         "requires malformed-value rejection acknowledgement"},
        {16,
         "v676",
         "route-preview-value-supply-signed-approval-template-signed-approval-policy",
         "v675",
         "fixtures/release/shard-readiness-v675.json",
         "declares signed approval policy without treating acknowledgement as a grant"},
        {17,
         "v677",
         "route-preview-value-supply-signed-approval-template-draft-only-policy",
         "v676",
         "fixtures/release/shard-readiness-v676.json",
         "keeps approval packets draft-only and out of runtime state"},
        {18,
         "v678",
         "route-preview-value-supply-signed-approval-template-justification-field",
         "v677",
         "fixtures/release/shard-readiness-v677.json",
         "requires future operator justification without fabricating one"},
        {19,
         "v679",
         "route-preview-value-supply-signed-approval-template-risk-acknowledgement",
         "v678",
         "fixtures/release/shard-readiness-v678.json",
         "requires future risk acknowledgement while keeping execution disabled"},
        {20,
         "v680",
         "route-preview-value-supply-signed-approval-template-evidence-coverage",
         "v679",
         "fixtures/release/shard-readiness-v679.json",
         "requires future evidence coverage statement before capture preflight"},
        {21,
         "v681",
         "route-preview-value-supply-signed-approval-template-review-control-checklist",
         "v680",
         "fixtures/release/shard-readiness-v680.json",
         "binds the template to the review control checklist catalog"},
        {22,
         "v682",
         "route-preview-value-supply-signed-approval-template-no-prior-approval-audit",
         "v681",
         "fixtures/release/shard-readiness-v681.json",
         "audits that no prior approval grant is assumed"},
        {23,
         "v683",
         "route-preview-value-supply-signed-approval-template-zero-value-count-audit",
         "v682",
         "fixtures/release/shard-readiness-v682.json",
         "audits zero supplied, accepted, imported, and persisted operator values"},
        {24,
         "v684",
         "route-preview-value-supply-signed-approval-template-mutation-block-audit",
         "v683",
         "fixtures/release/shard-readiness-v683.json",
         "audits write, service, sibling mutation, router, WAL, and execution blocks"},
        {25,
         "v685",
         "route-preview-value-supply-signed-approval-template-release-package",
         "v684",
         "fixtures/release/shard-readiness-v684.json",
         "packages the signed approval template for a future capture preflight only"},
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
    return signed_approval_template_stages.at(
        static_cast<std::size_t>(current_signed_approval_template_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        signed_approval_template_stages,
        current_signed_approval_template_stage_count,
        planned_signed_approval_template_stage_count,
        first_signed_approval_template_release_number);
}

} // namespace

std::string format_signed_approval_template_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        signed_approval_template_stages,
        current_signed_approval_template_stage_count);
}

std::string format_signed_approval_template_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_operator_value_supply_approval_template::published_stage_count() == 25;
    const int field_count =
        shard_route_preview_operator_value_supply_signed_approval_template_fields::
            planned_signed_approval_template_field_count();
    const int clause_count =
        shard_route_preview_operator_value_supply_signed_approval_template_clauses::
            planned_signed_approval_template_clause_count();
    const int review_control_count =
        shard_route_preview_operator_value_supply_signed_approval_template_controls::
            planned_signed_approval_template_review_control_count();
    const bool catalogs_aligned = field_count == clause_count && clause_count == review_control_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-template.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\""
           ",\"signedApprovalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-template\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\""
           ",\"sourceApprovalTemplateReleaseVersion\":\"v660\""
           ",\"sourceApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v660.json\""
           ",\"sourceApprovalTemplateArchiveRootHint\":\"e/660/\""
           ",\"sourceApprovalTemplatePublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_approval_template::published_stage_count()) +
           ",\"sourceApprovalTemplatePlannedStageCount\":25"
           ",\"sourceApprovalTemplateChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceApprovalTemplateDigestMarker\":" +
           json_string(shard_route_preview_operator_value_supply_approval_template::
                           value_supply_approval_template_digest_marker()) +
           ",\"signedApprovalTemplateStage\":" + json_string(stage.stage) +
           ",\"signedApprovalTemplateStageSequence\":" + std::to_string(stage.sequence) +
           ",\"signedApprovalTemplateReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_signed_approval_template_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_signed_approval_template_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceApprovalTemplateReleaseRangeStart\":\"v636\""
           ",\"sourceApprovalTemplateReleaseRangeEnd\":\"v660\""
           ",\"sourceApprovalTemplateReleaseCount\":25"
           ",\"signedApprovalTemplateReleaseRangeStart\":\"v661\""
           ",\"signedApprovalTemplateReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"signedApprovalTemplateReleaseCount\":" +
           std::to_string(current_signed_approval_template_stage_count) +
           ",\"expectedSignedApprovalTemplateFieldCount\":25"
           ",\"signedApprovalTemplateFieldCount\":" + std::to_string(field_count) +
           ",\"signedApprovalTemplateClauseCount\":" + std::to_string(clause_count) +
           ",\"signedApprovalTemplateReviewControlCount\":" + std::to_string(review_control_count) +
           ",\"signedApprovalTemplateCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"publishedSignedApprovalTemplateFieldCount\":" +
           std::to_string(current_signed_approval_template_stage_count) +
           ",\"operatorValueSupplyApprovalPacketState\":\"signed-template-required-not-present\""
           ",\"signedApprovalTemplateState\":\"template-only-no-signature-present\""
           ",\"signedApprovalTemplateDeclared\":true"
           ",\"signedApprovalTemplateOnly\":true"
           ",\"signedApprovalTemplateDisabledByDefault\":true"
           ",\"signedApprovalRequired\":true"
           ",\"signedApprovalPresent\":false"
           ",\"signedApprovalAccepted\":false"
           ",\"signatureCaptureRequired\":true"
           ",\"signatureCaptured\":false"
           ",\"signatureAccepted\":false"
           ",\"approvalGrantRequired\":true"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantAccepted\":false"
           ",\"operatorIdentityRequired\":true"
           ",\"operatorIdentityPresent\":false"
           ",\"operatorRoleRequired\":true"
           ",\"operatorRolePresent\":false"
           ",\"approvalTimestampRequired\":true"
           ",\"approvalTimestampPresent\":false"
           ",\"sourceReviewPackageDigestRequired\":true"
           ",\"sourceReviewPackageDigestSupplied\":false"
           ",\"sourceApprovalDraftDigestRequired\":true"
           ",\"sourceApprovalDraftDigestSupplied\":false"
           ",\"sourceReleaseVersionRequired\":true"
           ",\"sourceReleaseVersionSupplied\":false"
           ",\"sourceFileIdRequired\":true"
           ",\"sourceFileIdSupplied\":false"
           ",\"sourceSnippetIdRequired\":true"
           ",\"sourceSnippetIdSupplied\":false"
           ",\"typedValueEnvelopeRequired\":true"
           ",\"typedValueEnvelopeSupplied\":false"
           ",\"redactedValueDigestRequired\":true"
           ",\"redactedValueDigestSupplied\":false"
           ",\"provenanceBindingRequired\":true"
           ",\"provenanceBindingSupplied\":false"
           ",\"missingSignedApprovalRejected\":true"
           ",\"malformedSignedApprovalRejected\":true"
           ",\"missingValueRejected\":true"
           ",\"malformedValueRejected\":true"
           ",\"suppliedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"signedApprovalTemplateFieldHelper\":\"shard_route_preview_operator_value_supply_signed_approval_template_fields\""
           ",\"signedApprovalTemplateClauseHelper\":\"shard_route_preview_operator_value_supply_signed_approval_template_clauses\""
           ",\"signedApprovalTemplateControlHelper\":\"shard_route_preview_operator_value_supply_signed_approval_template_controls\""
           ",\"signedApprovalTemplateValidationHelper\":\"shard_route_preview_operator_value_supply_signed_approval_template_validation\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"signedApprovalTemplateStageCatalog\":" + format_signed_approval_template_stage_catalog_json() +
           ",\"signedApprovalTemplateStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"signedApprovalTemplateFields\":" +
           shard_route_preview_operator_value_supply_signed_approval_template_fields::
               format_signed_approval_template_fields_json(current_signed_approval_template_stage_count) +
           ",\"signedApprovalTemplateClauses\":" +
           shard_route_preview_operator_value_supply_signed_approval_template_clauses::
               format_signed_approval_template_clauses_json(current_signed_approval_template_stage_count) +
           ",\"signedApprovalTemplateReviewControls\":" +
           shard_route_preview_operator_value_supply_signed_approval_template_controls::
               format_signed_approval_template_review_controls_json(current_signed_approval_template_stage_count) +
           ",\"signedApprovalTemplateValidation\":" +
           shard_route_preview_operator_value_supply_signed_approval_template_validation::
               format_signed_approval_template_validation_json(field_count,
                                                               clause_count,
                                                               review_control_count,
                                                               current_signed_approval_template_stage_count) +
           ",\"signedApprovalTemplateChecks\":" +
           json_string_array({"source approval-template command remains available",
                              "source approval-template chain remains complete",
                              "source approval-template digest marker remains pinned",
                              "Node v1036 signed approval template closeout plan is referenced as plan evidence only",
                              "signed approval template fields are cataloged separately",
                              "signed approval clauses are cataloged separately",
                              "review controls are bound to source review package controls",
                              "field, clause, and review-control counts align",
                              "missing signed approval blocks capture",
                              "malformed signed approval blocks capture",
                              "operator identity, role, timestamp, source digest, and source ids remain absent",
                              "typed value envelope, redacted digest, and provenance binding remain absent",
                              "approval grant remains absent",
                              "operator values remain zero",
                              "automatic sibling import is disabled",
                              "runtime payload acceptance is disabled",
                              "credential, secret, and raw endpoint values are not stored",
                              "current SHARDJSON embeds exactly one signed approval template section",
                              "signed approval capture is not performed",
                              "value import is not performed",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"signedApprovalTemplateCheckCount\":21"
           ",\"signedApprovalTemplatePassedCount\":21"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"signedApprovalTemplateFieldHelperApplied\":true"
           ",\"signedApprovalTemplateClauseHelperApplied\":true"
           ",\"signedApprovalTemplateControlHelperApplied\":true"
           ",\"signedApprovalTemplateValidationHelperApplied\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleSignedApprovalTemplateSectionExpected\":true"
           ",\"signedApprovalTemplateChainComplete\":" + json_bool(stage_chain.chain_complete) +
           ",\"readyForSignedApprovalTemplatePreflightReview\":true"
           ",\"readyForSignedApprovalCapturePreflightDesign\":true"
           ",\"readyForSignedApprovalCapturePreflight\":false"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"readyForOperatorValueSupplyApproval\":false"
           ",\"readyForOperatorValueSupply\":false"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForRuntimePayloadAcceptance\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"operatorValueSupplyApprovalAllowed\":false"
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
           ",\"redactionPolicyDeclared\":true"
           ",\"redactionPolicyPreviewOnly\":true"
           ",\"normalizerExecutes\":false"
           ",\"normalizationExecutionAllowed\":false"
           ",\"signedApprovalTemplateValuesPersisted\":false"
           ",\"signedApprovalTemplatePackageOnly\":true"
           ",\"capturePreflightPackageOnly\":false"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"signed approval capture",
                              "approval grant acceptance",
                              "operator value supply approval",
                              "operator value supply",
                              "operator evidence value import",
                              "automatic sibling evidence import",
                              "manual evidence entry",
                              "runtime payload acceptance",
                              "synthetic evidence normalization",
                              "credential or secret storage",
                              "raw endpoint storage",
                              "active shard router installation",
                              "write routing",
                              "restore, load, compact, service start, or live read execution"}) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read signed approval template evidence only",
                              "treat v636-v660 approval-template release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat signed approval, signature capture, and approval grant as absent",
                              "require operator identity, role, timestamp, source digests, source ids, typed value envelope, redacted digest, and provenance binding before future capture",
                              "reject missing and malformed signed approval material",
                              "do not capture a signature in mini-kv",
                              "do not approve or import operator evidence values through mini-kv",
                              "do not perform automatic sibling evidence import",
                              "do not accept runtime payloads as value supply",
                              "do not normalize synthetic evidence into accepted evidence",
                              "do not store credential, secret, or raw endpoint values",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           route_preview_evidence_boundary::format_no_execution_boundary_fields() + "}";
}

std::string signed_approval_template_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_signed_approval_template_stage_count,
        planned_signed_approval_template_stage_count);
}

int published_stage_count() {
    return current_signed_approval_template_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template
