#include "minikv/shard_route_preview_operator_value_supply_approval_template.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template_fields.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template_slots.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck_slots.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_approval_template {
namespace {

constexpr int first_value_supply_approval_template_release_number = 636;
constexpr int current_value_supply_approval_template_stage_count = 9;
constexpr int planned_value_supply_approval_template_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v986-controlled-read-only-shard-preview-operator-evidence-value-supply-approval-packet-draft-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_value_supply_approval_template_stage_count>
    value_supply_approval_template_stages = {{
        {1,
         "v636",
         "route-preview-value-supply-approval-template-field-catalog-split",
         "v635",
         "fixtures/release/shard-readiness-v635.json",
         "splits approval packet draft fields away from the disabled value-supply precheck"},
        {2,
         "v637",
         "route-preview-value-supply-approval-template-source-precheck-contract",
         "v636",
         "fixtures/release/shard-readiness-v636.json",
         "pins the v635 disabled value-supply precheck command and release contract as source evidence"},
        {3,
         "v638",
         "route-preview-value-supply-approval-template-source-release-freeze",
         "v637",
         "fixtures/release/shard-readiness-v637.json",
         "freezes the prior approval-template fixture before rolling current evidence"},
        {4,
         "v639",
         "route-preview-value-supply-approval-template-stage-chain-audit",
         "v638",
         "fixtures/release/shard-readiness-v638.json",
         "audits approval-template stage continuity through the shared helper"},
        {5,
         "v640",
         "route-preview-value-supply-approval-template-fixture-chain-audit",
         "v639",
         "fixtures/release/shard-readiness-v639.json",
         "audits approval-template fixture continuity without runtime file reads"},
        {6,
         "v641",
         "route-preview-value-supply-approval-template-command-catalog-contract",
         "v640",
         "fixtures/release/shard-readiness-v640.json",
         "pins COMMANDS and COMMANDSJSON metadata for the approval-template command"},
        {7,
         "v642",
         "route-preview-value-supply-approval-template-explain-contract",
         "v641",
         "fixtures/release/shard-readiness-v641.json",
         "pins EXPLAINJSON metadata-read semantics for the approval-template command"},
        {8,
         "v643",
         "route-preview-value-supply-approval-template-checkjson-contract",
         "v642",
         "fixtures/release/shard-readiness-v642.json",
         "pins CHECKJSON no-execution semantics for the approval-template command"},
        {9,
         "v644",
         "route-preview-value-supply-approval-template-shardjson-embedding",
         "v643",
         "fixtures/release/shard-readiness-v643.json",
         "embeds the approval-template section into SHARDJSON once"},
        {10,
         "v645",
         "route-preview-value-supply-approval-template-shardjson-embedding-audit",
         "v644",
         "fixtures/release/shard-readiness-v644.json",
         "audits that SHARDJSON carries exactly one approval-template section"},
        {11,
         "v646",
         "route-preview-value-supply-approval-template-slot-field-contract",
         "v645",
         "fixtures/release/shard-readiness-v645.json",
         "declares per-slot approval packet draft fields as metadata only"},
        {12,
         "v647",
         "route-preview-value-supply-approval-template-slot-count-audit",
         "v646",
         "fixtures/release/shard-readiness-v646.json",
         "audits that approval-template slots match the source precheck slot count"},
        {13,
         "v648",
         "route-preview-value-supply-approval-template-identity-field-policy",
         "v647",
         "fixtures/release/shard-readiness-v647.json",
         "requires future operator identity while keeping it absent"},
        {14,
         "v649",
         "route-preview-value-supply-approval-template-timestamp-field-policy",
         "v648",
         "fixtures/release/shard-readiness-v648.json",
         "requires future approval timestamp while keeping it absent"},
        {15,
         "v650",
         "route-preview-value-supply-approval-template-correlation-id-policy",
         "v649",
         "fixtures/release/shard-readiness-v649.json",
         "requires future approval correlation id while keeping it absent"},
        {16,
         "v651",
         "route-preview-value-supply-approval-template-redaction-policy",
         "v650",
         "fixtures/release/shard-readiness-v650.json",
         "requires redacted value digest fields before any operator value could be reviewed"},
        {17,
         "v652",
         "route-preview-value-supply-approval-template-no-value-ingest-audit",
         "v651",
         "fixtures/release/shard-readiness-v651.json",
         "audits that the approval-template ingests no operator values"},
        {18,
         "v653",
         "route-preview-value-supply-approval-template-no-automatic-import-audit",
         "v652",
         "fixtures/release/shard-readiness-v652.json",
         "audits that no sibling evidence is imported automatically"},
        {19,
         "v654",
         "route-preview-value-supply-approval-template-no-runtime-payload-audit",
         "v653",
         "fixtures/release/shard-readiness-v653.json",
         "audits that runtime payloads are not accepted as approval input"},
        {20,
         "v655",
         "route-preview-value-supply-approval-template-no-secret-storage-audit",
         "v654",
         "fixtures/release/shard-readiness-v654.json",
         "audits that the approval-template stores no credential, secret, or raw endpoint value"},
        {21,
         "v656",
         "route-preview-value-supply-approval-template-no-router-audit",
         "v655",
         "fixtures/release/shard-readiness-v655.json",
         "audits that the approval-template installs no active shard router"},
        {22,
         "v657",
         "route-preview-value-supply-approval-template-no-write-audit",
         "v656",
         "fixtures/release/shard-readiness-v656.json",
         "audits that the approval-template routes no writes and touches no WAL"},
        {23,
         "v658",
         "route-preview-value-supply-approval-template-no-execution-audit",
         "v657",
         "fixtures/release/shard-readiness-v657.json",
         "audits that the approval-template cannot trigger restore, load, compact, routing, or service execution"},
        {24,
         "v659",
         "route-preview-value-supply-approval-template-release-readiness-audit",
         "v658",
         "fixtures/release/shard-readiness-v658.json",
         "audits release readiness while preserving disabled approval boundaries"},
        {25,
         "v660",
         "route-preview-value-supply-approval-template-release-package",
         "v659",
         "fixtures/release/shard-readiness-v659.json",
         "packages final v636-v660 approval-template evidence for downstream review"},
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
    return value_supply_approval_template_stages.at(
        static_cast<std::size_t>(current_value_supply_approval_template_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        value_supply_approval_template_stages,
        current_value_supply_approval_template_stage_count,
        planned_value_supply_approval_template_stage_count,
        first_value_supply_approval_template_release_number);
}

} // namespace

std::string format_value_supply_approval_template_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        value_supply_approval_template_stages,
        current_value_supply_approval_template_stage_count);
}

std::string format_value_supply_approval_template_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_operator_value_supply_precheck::published_stage_count() == 25;
    const bool slot_count_matches_source =
        shard_route_preview_operator_value_supply_approval_template_slots::
            planned_value_supply_approval_template_slot_count() ==
        shard_route_preview_operator_value_supply_precheck_slots::planned_value_supply_precheck_slot_count();

    return "{\"contract\":\"shard-route-preview-operator-value-supply-approval-template.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\""
           ",\"approvalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-approval-template\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceValueSupplyPrecheckCommand\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\""
           ",\"sourceValueSupplyPrecheckReleaseVersion\":\"v635\""
           ",\"sourceValueSupplyPrecheckFixturePath\":\"fixtures/release/shard-readiness-v635.json\""
           ",\"sourceValueSupplyPrecheckArchiveRootHint\":\"e/635/\""
           ",\"sourceValueSupplyPrecheckPublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_precheck::published_stage_count()) +
           ",\"sourceValueSupplyPrecheckPlannedStageCount\":25"
           ",\"sourceValueSupplyPrecheckChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceValueSupplyPrecheckDigestMarker\":" +
           json_string(shard_route_preview_operator_value_supply_precheck::value_supply_precheck_digest_marker()) +
           ",\"valueSupplyApprovalTemplateStage\":" + json_string(stage.stage) +
           ",\"valueSupplyApprovalTemplateStageSequence\":" + std::to_string(stage.sequence) +
           ",\"valueSupplyApprovalTemplateReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_value_supply_approval_template_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_value_supply_approval_template_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceValueSupplyPrecheckReleaseRangeStart\":\"v611\""
           ",\"sourceValueSupplyPrecheckReleaseRangeEnd\":\"v635\""
           ",\"sourceValueSupplyPrecheckReleaseCount\":25"
           ",\"valueSupplyApprovalTemplateReleaseRangeStart\":\"v636\""
           ",\"valueSupplyApprovalTemplateReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"valueSupplyApprovalTemplateReleaseCount\":" +
           std::to_string(current_value_supply_approval_template_stage_count) +
           ",\"expectedApprovalTemplateFieldCount\":25"
           ",\"sourcePrecheckSlotCount\":" +
           std::to_string(
               shard_route_preview_operator_value_supply_precheck_slots::planned_value_supply_precheck_slot_count()) +
           ",\"approvalTemplateSlotCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_approval_template_slots::
                              planned_value_supply_approval_template_slot_count()) +
           ",\"approvalTemplateSlotCountMatchesSourcePrecheck\":" + json_bool(slot_count_matches_source) +
           ",\"approvalTemplateFieldCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_approval_template_fields::
                              planned_approval_template_field_count()) +
           ",\"publishedApprovalTemplateFieldCount\":" +
           std::to_string(current_value_supply_approval_template_stage_count) +
           ",\"operatorValueSupplyApprovalPacketState\":\"draft-template-required-not-present\""
           ",\"approvalPacketDraftState\":\"template-only-no-packet-present\""
           ",\"operatorApprovalPacketTemplateDeclared\":true"
           ",\"operatorApprovalPacketTemplateOnly\":true"
           ",\"approvalTemplateOnly\":true"
           ",\"approvalPacketRequired\":true"
           ",\"approvalPacketPresent\":false"
           ",\"approvalPacketAccepted\":false"
           ",\"approvalGrantRequired\":true"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantAccepted\":false"
           ",\"operatorIdentityRequired\":true"
           ",\"operatorIdentityPresent\":false"
           ",\"operatorIdentityAccepted\":false"
           ",\"approvalTimestampRequired\":true"
           ",\"approvalTimestampPresent\":false"
           ",\"approvalTimestampAccepted\":false"
           ",\"approvalCorrelationIdRequired\":true"
           ",\"approvalCorrelationIdPresent\":false"
           ",\"redactedValueDigestRequired\":true"
           ",\"redactedValueDigestSupplied\":false"
           ",\"typedValueEnvelopeRequired\":true"
           ",\"typedValueEnvelopeSupplied\":false"
           ",\"reviewRecordRequired\":true"
           ",\"reviewRecordPresent\":false"
           ",\"reviewRecordAccepted\":false"
           ",\"perValueProvenanceRequired\":true"
           ",\"perValueProvenanceSupplied\":false"
           ",\"redactionPolicy\":\"redact-before-persist\""
           ",\"redactionBeforePersistRequired\":true"
           ",\"malformedValuePolicy\":\"reject\""
           ",\"missingValuePolicy\":\"fail-closed\""
           ",\"missingValueRejected\":true"
           ",\"malformedValueRejected\":true"
           ",\"suppliedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"approvalTemplateSlotHelper\":\"shard_route_preview_operator_value_supply_approval_template_slots\""
           ",\"approvalTemplateFieldHelper\":\"shard_route_preview_operator_value_supply_approval_template_fields\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"valueSupplyApprovalTemplateStageCatalog\":" +
           format_value_supply_approval_template_stage_catalog_json() +
           ",\"valueSupplyApprovalTemplateStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"valueSupplyApprovalTemplateSlots\":" +
           shard_route_preview_operator_value_supply_approval_template_slots::
               format_value_supply_approval_template_slots_json(current_value_supply_approval_template_stage_count) +
           ",\"valueSupplyApprovalTemplateFields\":" +
           shard_route_preview_operator_value_supply_approval_template_fields::
               format_approval_template_fields_json(current_value_supply_approval_template_stage_count) +
           ",\"valueSupplyApprovalTemplateChecks\":" +
           json_string_array({"source value-supply precheck command remains available",
                              "source value-supply precheck chain remains complete",
                              "source value-supply precheck digest marker remains pinned",
                              "Node v986 approval packet draft closeout plan is referenced as plan evidence only",
                              "approval packet template is declared but no approval packet is present",
                              "approval grant is required but absent",
                              "operator identity is required but absent",
                              "approval timestamp is required but absent",
                              "approval correlation id is required but absent",
                              "redacted value digest is required but absent",
                              "typed value envelope is required but absent",
                              "review record is required but absent",
                              "per-value provenance is required but absent",
                              "missing values fail closed",
                              "malformed values are rejected",
                              "automatic sibling import is disabled",
                              "runtime payload acceptance is disabled",
                              "credential, secret, and raw endpoint values are not stored",
                              "approval-template slots match source precheck slots",
                              "approval-template fields are cataloged separately",
                              "current SHARDJSON embeds exactly one approval-template section",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"valueSupplyApprovalTemplateCheckCount\":22"
           ",\"valueSupplyApprovalTemplatePassedCount\":22"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"approvalTemplateSlotHelperApplied\":true"
           ",\"approvalTemplateFieldHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleValueSupplyApprovalTemplateSectionExpected\":true"
           ",\"valueSupplyApprovalTemplateChainComplete\":" + json_bool(stage_chain.chain_complete) +
           ",\"readyForApprovalPacketDraftReview\":true"
           ",\"readyForOperatorValueSupplyApproval\":false"
           ",\"readyForOperatorValueSupply\":false"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForRuntimePayloadAcceptance\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"valueSupplyApprovalTemplateDeclared\":true"
           ",\"valueSupplyApprovalTemplateDisabledByDefault\":true"
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
           ",\"approvalTemplateValuesPersisted\":false"
           ",\"valueSupplyApprovalTemplatePackageOnly\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"operator value supply approval",
                              "signed approval capture",
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
           json_string_array({"read approval-template evidence only",
                              "treat v611-v635 value-supply precheck release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat approval packet, approval grant, operator identity, timestamp, and correlation id as absent",
                              "require redacted digest, typed value envelope, review record, and provenance before any future approval",
                              "reject missing and malformed supplied values",
                              "do not supply or import operator evidence values through mini-kv",
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

std::string value_supply_approval_template_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_value_supply_approval_template_stage_count,
        planned_value_supply_approval_template_stage_count);
}

int published_stage_count() {
    return current_value_supply_approval_template_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_approval_template
