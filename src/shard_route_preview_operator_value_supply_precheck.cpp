#include "minikv/shard_route_preview_operator_value_supply_precheck.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck_requirements.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck_slots.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope_slots.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_precheck {
namespace {

constexpr int first_value_supply_precheck_release_number = 611;
constexpr int current_value_supply_precheck_stage_count = 9;
constexpr int planned_value_supply_precheck_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v961-controlled-read-only-shard-preview-operator-evidence-value-supply-envelope-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_value_supply_precheck_stage_count> value_supply_precheck_stages = {{
    {1,
     "v611",
     "route-preview-value-supply-precheck-requirement-catalog-split",
     "v610",
     "fixtures/release/shard-readiness-v610.json",
     "splits approval packet precheck requirements away from the value-supply envelope"},
    {2,
     "v612",
     "route-preview-value-supply-precheck-source-envelope-contract",
     "v611",
     "fixtures/release/shard-readiness-v611.json",
     "pins the v610 value-supply envelope command and release contract as source evidence"},
    {3,
     "v613",
     "route-preview-value-supply-precheck-source-release-freeze",
     "v612",
     "fixtures/release/shard-readiness-v612.json",
     "freezes the prior precheck fixture before rolling current evidence"},
    {4,
     "v614",
     "route-preview-value-supply-precheck-stage-chain-audit",
     "v613",
     "fixtures/release/shard-readiness-v613.json",
     "audits value-supply precheck stage continuity through the shared helper"},
    {5,
     "v615",
     "route-preview-value-supply-precheck-fixture-chain-audit",
     "v614",
     "fixtures/release/shard-readiness-v614.json",
     "audits value-supply precheck fixture continuity without runtime file reads"},
    {6,
     "v616",
     "route-preview-value-supply-precheck-command-catalog-contract",
     "v615",
     "fixtures/release/shard-readiness-v615.json",
     "pins COMMANDS and COMMANDSJSON metadata for the disabled precheck command"},
    {7,
     "v617",
     "route-preview-value-supply-precheck-explain-contract",
     "v616",
     "fixtures/release/shard-readiness-v616.json",
     "pins EXPLAINJSON metadata-read semantics for the disabled precheck command"},
    {8,
     "v618",
     "route-preview-value-supply-precheck-checkjson-contract",
     "v617",
     "fixtures/release/shard-readiness-v617.json",
     "pins CHECKJSON no-execution semantics for the disabled precheck command"},
    {9,
     "v619",
     "route-preview-value-supply-precheck-shardjson-embedding",
     "v618",
     "fixtures/release/shard-readiness-v618.json",
     "embeds the disabled precheck section into SHARDJSON once"},
    {10,
     "v620",
     "route-preview-value-supply-precheck-shardjson-embedding-audit",
     "v619",
     "fixtures/release/shard-readiness-v619.json",
     "audits that SHARDJSON carries exactly one disabled precheck section"},
    {11,
     "v621",
     "route-preview-value-supply-precheck-slot-requirement-contract",
     "v620",
     "fixtures/release/shard-readiness-v620.json",
     "declares per-slot approval requirements as metadata only"},
    {12,
     "v622",
     "route-preview-value-supply-precheck-slot-count-audit",
     "v621",
     "fixtures/release/shard-readiness-v621.json",
     "audits that precheck slots match the source envelope slot count"},
    {13,
     "v623",
     "route-preview-value-supply-precheck-provenance-policy",
     "v622",
     "fixtures/release/shard-readiness-v622.json",
     "requires per-value provenance before any supplied value can be reviewed"},
    {14,
     "v624",
     "route-preview-value-supply-precheck-redaction-policy",
     "v623",
     "fixtures/release/shard-readiness-v623.json",
     "declares redact-before-persist policy before any operator value can appear"},
    {15,
     "v625",
     "route-preview-value-supply-precheck-missing-malformed-policy",
     "v624",
     "fixtures/release/shard-readiness-v624.json",
     "pins fail-closed handling for missing or malformed operator values"},
    {16,
     "v626",
     "route-preview-value-supply-precheck-approval-identity-fields",
     "v625",
     "fixtures/release/shard-readiness-v625.json",
     "requires operator approval identity fields while keeping them absent"},
    {17,
     "v627",
     "route-preview-value-supply-precheck-approval-timestamp-fields",
     "v626",
     "fixtures/release/shard-readiness-v626.json",
     "requires approval timestamp fields while keeping them absent"},
    {18,
     "v628",
     "route-preview-value-supply-precheck-no-automatic-import-audit",
     "v627",
     "fixtures/release/shard-readiness-v627.json",
     "audits that no sibling evidence is imported automatically"},
    {19,
     "v629",
     "route-preview-value-supply-precheck-no-runtime-payload-audit",
     "v628",
     "fixtures/release/shard-readiness-v628.json",
     "audits that runtime payloads are not accepted as precheck input"},
    {20,
     "v630",
     "route-preview-value-supply-precheck-no-secret-storage-audit",
     "v629",
     "fixtures/release/shard-readiness-v629.json",
     "audits that the precheck stores no credential, secret, or raw endpoint value"},
    {21,
     "v631",
     "route-preview-value-supply-precheck-no-router-audit",
     "v630",
     "fixtures/release/shard-readiness-v630.json",
     "audits that the precheck installs no active shard router"},
    {22,
     "v632",
     "route-preview-value-supply-precheck-no-write-audit",
     "v631",
     "fixtures/release/shard-readiness-v631.json",
     "audits that the precheck routes no writes and touches no WAL"},
    {23,
     "v633",
     "route-preview-value-supply-precheck-no-execution-audit",
     "v632",
     "fixtures/release/shard-readiness-v632.json",
     "audits that the precheck cannot trigger restore, load, compact, routing, or service execution"},
    {24,
     "v634",
     "route-preview-value-supply-precheck-release-readiness-audit",
     "v633",
     "fixtures/release/shard-readiness-v633.json",
     "audits release readiness while preserving disabled value-supply approval boundaries"},
    {25,
     "v635",
     "route-preview-value-supply-precheck-release-package",
     "v634",
     "fixtures/release/shard-readiness-v634.json",
     "packages final v611-v635 disabled value-supply precheck evidence for downstream review"},
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
    return value_supply_precheck_stages.at(
        static_cast<std::size_t>(current_value_supply_precheck_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(value_supply_precheck_stages,
                                                                current_value_supply_precheck_stage_count,
                                                                planned_value_supply_precheck_stage_count,
                                                                first_value_supply_precheck_release_number);
}

} // namespace

std::string format_value_supply_precheck_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(value_supply_precheck_stages,
                                                                        current_value_supply_precheck_stage_count);
}

std::string format_value_supply_precheck_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_operator_value_supply_envelope::published_stage_count() == 25;
    const bool slot_count_matches_source =
        shard_route_preview_operator_value_supply_precheck_slots::planned_value_supply_precheck_slot_count() ==
        shard_route_preview_operator_value_supply_envelope_slots::planned_value_supply_slot_count();

    return "{\"contract\":\"shard-route-preview-operator-value-supply-precheck.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\""
           ",\"precheckMode\":\"controlled-read-only-disabled-operator-value-supply-precheck\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceValueSupplyEnvelopeCommand\":\"SHARDROUTEVALUESUPPLYJSON\""
           ",\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\""
           ",\"sourceValueSupplyEnvelopeFixturePath\":\"fixtures/release/shard-readiness-v610.json\""
           ",\"sourceValueSupplyEnvelopeArchiveRootHint\":\"e/610/\""
           ",\"sourceValueSupplyEnvelopePublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_envelope::published_stage_count()) +
           ",\"sourceValueSupplyEnvelopePlannedStageCount\":25"
           ",\"sourceValueSupplyEnvelopeChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceValueSupplyEnvelopeDigestMarker\":" +
           json_string(shard_route_preview_operator_value_supply_envelope::value_supply_envelope_digest_marker()) +
           ",\"valueSupplyPrecheckStage\":" +
           json_string(stage.stage) +
           ",\"valueSupplyPrecheckStageSequence\":" + std::to_string(stage.sequence) +
           ",\"valueSupplyPrecheckReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_value_supply_precheck_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_value_supply_precheck_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceValueSupplyEnvelopeReleaseRangeStart\":\"v586\""
           ",\"sourceValueSupplyEnvelopeReleaseRangeEnd\":\"v610\""
           ",\"sourceValueSupplyEnvelopeReleaseCount\":25"
           ",\"valueSupplyPrecheckReleaseRangeStart\":\"v611\""
           ",\"valueSupplyPrecheckReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"valueSupplyPrecheckReleaseCount\":" +
           std::to_string(current_value_supply_precheck_stage_count) +
           ",\"expectedOperatorEvidenceSlotCount\":25"
           ",\"sourceEnvelopeSlotCount\":" +
           std::to_string(shard_route_preview_operator_value_supply_envelope_slots::planned_value_supply_slot_count()) +
           ",\"precheckSlotCount\":" +
           std::to_string(
               shard_route_preview_operator_value_supply_precheck_slots::planned_value_supply_precheck_slot_count()) +
           ",\"precheckSlotCountMatchesSourceEnvelope\":" +
           json_bool(slot_count_matches_source) +
           ",\"precheckRequirementCount\":" +
           std::to_string(
               shard_route_preview_operator_value_supply_precheck_requirements::planned_precheck_requirement_count()) +
           ",\"publishedPrecheckRequirementCount\":" +
           std::to_string(current_value_supply_precheck_stage_count) +
           ",\"operatorValueSupplyApprovalPacketState\":\"required-not-present\""
           ",\"approvalPacketRequired\":true"
           ",\"approvalPacketPresent\":false"
           ",\"operatorIdentityRequired\":true"
           ",\"operatorIdentityPresent\":false"
           ",\"approvalTimestampRequired\":true"
           ",\"approvalTimestampPresent\":false"
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
           ",\"valueSupplyPrecheckSlotHelper\":\"shard_route_preview_operator_value_supply_precheck_slots\""
           ",\"valueSupplyPrecheckRequirementHelper\":\"shard_route_preview_operator_value_supply_precheck_requirements\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"valueSupplyPrecheckStageCatalog\":" +
           format_value_supply_precheck_stage_catalog_json() +
           ",\"valueSupplyPrecheckStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"valueSupplyPrecheckSlots\":" +
           shard_route_preview_operator_value_supply_precheck_slots::format_value_supply_precheck_slots_json(
               current_value_supply_precheck_stage_count) +
           ",\"valueSupplyPrecheckRequirements\":" +
           shard_route_preview_operator_value_supply_precheck_requirements::format_precheck_requirements_json(
               current_value_supply_precheck_stage_count) +
           ",\"precheckChecks\":" +
           json_string_array({"source value-supply envelope command remains available",
                              "source value-supply envelope chain remains complete",
                              "source value-supply envelope digest marker remains pinned",
                              "Node v961 value-supply envelope closeout plan is referenced as plan evidence only",
                              "approval packet is required but absent",
                              "operator identity is required but absent",
                              "approval timestamp is required but absent",
                              "per-value provenance is required but absent",
                              "redaction-before-persist policy is required",
                              "missing values fail closed",
                              "malformed values are rejected",
                              "automatic sibling import is disabled",
                              "runtime payload acceptance is disabled",
                              "credential, secret, and raw endpoint values are not stored",
                              "precheck slots match source envelope slots",
                              "precheck requirements are cataloged separately",
                              "current SHARDJSON embeds exactly one value-supply precheck section",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"precheckCheckCount\":18"
           ",\"precheckPassedCount\":18"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"valueSupplyPrecheckSlotHelperApplied\":true"
           ",\"valueSupplyPrecheckRequirementHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleValueSupplyPrecheckSectionExpected\":true"
           ",\"valueSupplyPrecheckChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"readyForDisabledValueSupplyPrecheck\":true"
           ",\"readyForOperatorValueSupplyApproval\":false"
           ",\"readyForOperatorValueSupply\":false"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForRuntimePayloadAcceptance\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"valueSupplyPrecheckDeclared\":true"
           ",\"valueSupplyPrecheckDisabledByDefault\":true"
           ",\"operatorValueSupplyApprovalAllowed\":false"
           ",\"approvalPacketAccepted\":false"
           ",\"operatorIdentityAccepted\":false"
           ",\"approvalTimestampAccepted\":false"
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
           ",\"precheckValuesPersisted\":false"
           ",\"valueSupplyPrecheckPackageOnly\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"operator value supply approval",
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
           json_string_array({"read value-supply approval precheck evidence only",
                              "treat v586-v610 value-supply envelope release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat approval packet, operator identity, and approval timestamp as absent",
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

std::string value_supply_precheck_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_value_supply_precheck_stage_count,
                                                                   planned_value_supply_precheck_stage_count);
}

int published_stage_count() {
    return current_value_supply_precheck_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_precheck
