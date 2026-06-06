#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_draft.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope_slots.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_envelope {
namespace {

constexpr int first_value_supply_release_number = 586;
constexpr int current_value_supply_stage_count = 14;
constexpr int planned_value_supply_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v936-controlled-read-only-shard-preview-operator-evidence-fresh-sibling-intake-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_value_supply_stage_count> value_supply_stages = {{
    {1,
     "v586",
     "route-preview-value-supply-envelope-policy-split",
     "v585",
     "fixtures/release/shard-readiness-v585.json",
     "starts a disabled value-supply envelope and splits per-slot policy metadata"},
    {2,
     "v587",
     "route-preview-value-supply-envelope-source-value-draft-contract",
     "v586",
     "fixtures/release/shard-readiness-v586.json",
     "pins the v585 value draft command and release contract as source evidence"},
    {3,
     "v588",
     "route-preview-value-supply-envelope-source-release-freeze",
     "v587",
     "fixtures/release/shard-readiness-v587.json",
     "freezes the prior envelope fixture before rolling current evidence"},
    {4,
     "v589",
     "route-preview-value-supply-envelope-stage-chain-audit",
     "v588",
     "fixtures/release/shard-readiness-v588.json",
     "audits value-supply envelope stage continuity through the shared helper"},
    {5,
     "v590",
     "route-preview-value-supply-envelope-fixture-chain-audit",
     "v589",
     "fixtures/release/shard-readiness-v589.json",
     "audits value-supply fixture continuity without runtime file reads"},
    {6,
     "v591",
     "route-preview-value-supply-envelope-command-catalog-contract",
     "v590",
     "fixtures/release/shard-readiness-v590.json",
     "pins COMMANDS and COMMANDSJSON metadata for the disabled envelope command"},
    {7,
     "v592",
     "route-preview-value-supply-envelope-explain-contract",
     "v591",
     "fixtures/release/shard-readiness-v591.json",
     "pins EXPLAINJSON metadata-read semantics for the disabled envelope command"},
    {8,
     "v593",
     "route-preview-value-supply-envelope-checkjson-contract",
     "v592",
     "fixtures/release/shard-readiness-v592.json",
     "pins CHECKJSON no-execution semantics for the disabled envelope command"},
    {9,
     "v594",
     "route-preview-value-supply-envelope-shardjson-embedding",
     "v593",
     "fixtures/release/shard-readiness-v593.json",
     "embeds the disabled envelope section into SHARDJSON once"},
    {10,
     "v595",
     "route-preview-value-supply-envelope-shardjson-embedding-audit",
     "v594",
     "fixtures/release/shard-readiness-v594.json",
     "audits that SHARDJSON carries exactly one disabled envelope section"},
    {11,
     "v596",
     "route-preview-value-supply-envelope-slot-policy-contract",
     "v595",
     "fixtures/release/shard-readiness-v595.json",
     "declares per-slot value-supply policy as metadata only"},
    {12,
     "v597",
     "route-preview-value-supply-envelope-slot-count-audit",
     "v596",
     "fixtures/release/shard-readiness-v596.json",
     "audits that envelope slots match the value draft slot count"},
    {13,
     "v598",
     "route-preview-value-supply-envelope-redaction-policy",
     "v597",
     "fixtures/release/shard-readiness-v597.json",
     "declares redact-before-persist policy before any operator value exists"},
    {14,
     "v599",
     "route-preview-value-supply-envelope-missing-value-policy",
     "v598",
     "fixtures/release/shard-readiness-v598.json",
     "pins fail-closed handling for missing operator values"},
    {15,
     "v600",
     "route-preview-value-supply-envelope-source-provenance-contract",
     "v599",
     "fixtures/release/shard-readiness-v599.json",
     "requires explicit source evidence provenance for any future supplied value"},
    {16,
     "v601",
     "route-preview-value-supply-envelope-no-automatic-sibling-import-audit",
     "v600",
     "fixtures/release/shard-readiness-v600.json",
     "audits that the envelope performs no automatic sibling import"},
    {17,
     "v602",
     "route-preview-value-supply-envelope-no-manual-entry-audit",
     "v601",
     "fixtures/release/shard-readiness-v601.json",
     "audits that manual value entry remains blocked from mini-kv storage"},
    {18,
     "v603",
     "route-preview-value-supply-envelope-no-runtime-payload-audit",
     "v602",
     "fixtures/release/shard-readiness-v602.json",
     "audits that runtime payloads are not accepted as value-supply input"},
    {19,
     "v604",
     "route-preview-value-supply-envelope-no-synthetic-evidence-audit",
     "v603",
     "fixtures/release/shard-readiness-v603.json",
     "audits that synthetic evidence cannot become a supplied value"},
    {20,
     "v605",
     "route-preview-value-supply-envelope-no-secret-storage-audit",
     "v604",
     "fixtures/release/shard-readiness-v604.json",
     "audits that the envelope stores no credential, secret, or raw endpoint value"},
    {21,
     "v606",
     "route-preview-value-supply-envelope-no-router-audit",
     "v605",
     "fixtures/release/shard-readiness-v605.json",
     "audits that the envelope installs no active shard router"},
    {22,
     "v607",
     "route-preview-value-supply-envelope-no-write-audit",
     "v606",
     "fixtures/release/shard-readiness-v606.json",
     "audits that the envelope routes no writes and touches no WAL"},
    {23,
     "v608",
     "route-preview-value-supply-envelope-no-execution-audit",
     "v607",
     "fixtures/release/shard-readiness-v607.json",
     "audits that the envelope cannot trigger restore, load, compact, routing, or service execution"},
    {24,
     "v609",
     "route-preview-value-supply-envelope-release-readiness-audit",
     "v608",
     "fixtures/release/shard-readiness-v608.json",
     "audits release readiness while preserving disabled value-supply boundaries"},
    {25,
     "v610",
     "route-preview-value-supply-envelope-release-package",
     "v609",
     "fixtures/release/shard-readiness-v609.json",
     "packages final v586-v610 disabled value-supply envelope evidence for downstream review"},
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
    return value_supply_stages.at(static_cast<std::size_t>(current_value_supply_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(value_supply_stages,
                                                                current_value_supply_stage_count,
                                                                planned_value_supply_stage_count,
                                                                first_value_supply_release_number);
}

} // namespace

std::string format_value_supply_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(value_supply_stages,
                                                                        current_value_supply_stage_count);
}

std::string format_value_supply_envelope_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = shard_route_preview_operator_value_draft::published_stage_count() == 25;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-envelope.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYJSON\""
           ",\"envelopeMode\":\"controlled-read-only-disabled-operator-value-supply-envelope\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceValueDraftCommand\":\"SHARDROUTEVALUEDRAFTJSON\""
           ",\"sourceValueDraftReleaseVersion\":\"v585\""
           ",\"sourceValueDraftFixturePath\":\"fixtures/release/shard-readiness-v585.json\""
           ",\"sourceValueDraftArchiveRootHint\":\"e/585/\""
           ",\"sourceValueDraftPublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_value_draft::published_stage_count()) +
           ",\"sourceValueDraftPlannedStageCount\":25"
           ",\"sourceValueDraftChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceValueDraftDigestMarker\":" +
           json_string(shard_route_preview_operator_value_draft::value_draft_digest_marker()) +
           ",\"valueSupplyStage\":" +
           json_string(stage.stage) +
           ",\"valueSupplyStageSequence\":" + std::to_string(stage.sequence) +
           ",\"valueSupplyReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_value_supply_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_value_supply_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceValueDraftReleaseRangeStart\":\"v561\""
           ",\"sourceValueDraftReleaseRangeEnd\":\"v585\""
           ",\"sourceValueDraftReleaseCount\":25"
           ",\"valueSupplyReleaseRangeStart\":\"v586\""
           ",\"valueSupplyReleaseRangeEnd\":\"v610\""
           ",\"valueSupplyReleaseCount\":25"
           ",\"operatorValueEnvelopeSlotCount\":25"
           ",\"expectedOperatorEvidenceSlotCount\":25"
           ",\"envelopeSlotCount\":" +
           std::to_string(
               shard_route_preview_operator_value_supply_envelope_slots::planned_value_supply_slot_count()) +
           ",\"publishedValueSupplySlotPolicyCount\":" + std::to_string(current_value_supply_stage_count) +
           ",\"suppliedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"actualValueState\":\"not-supplied\""
           ",\"operatorValueEnvelopeState\":\"disabled-by-default\""
           ",\"missingValuePolicy\":\"fail-closed\""
           ",\"redactionPolicy\":\"redact-before-persist\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"valueSupplySlotHelper\":\"shard_route_preview_operator_value_supply_envelope_slots\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"valueSupplyStageCatalog\":" +
           format_value_supply_stage_catalog_json() +
           ",\"valueSupplyStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"valueSupplySlots\":" +
           shard_route_preview_operator_value_supply_envelope_slots::format_value_supply_slots_json(
               current_value_supply_stage_count) +
           ",\"envelopeChecks\":" +
           json_string_array({"source value draft command remains available",
                              "source value draft chain remains complete",
                              "source value draft digest marker remains pinned",
                              "Node v936 fresh sibling intake closeout plan is referenced as plan evidence only",
                              "disabled-by-default value-supply envelope is declared",
                              "actual operator values remain not supplied",
                              "redaction policy is defined without storing values",
                              "missing value policy fails closed",
                              "source evidence provenance is required but not supplied",
                              "automatic sibling import is disabled",
                              "evidence import is still blocked",
                              "manual evidence entry is still blocked",
                              "runtime payloads are not accepted",
                              "synthetic evidence is rejected",
                              "credential, secret, and raw endpoint values are not stored",
                              "value-supply slots are policy labels only",
                              "current SHARDJSON embeds exactly one value-supply envelope section",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"envelopeCheckCount\":18"
           ",\"envelopePassedCount\":18"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"valueSupplySlotHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleValueSupplyEnvelopeSectionExpected\":true"
           ",\"valueSupplyEnvelopeChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"readyForDisabledValueSupplyEnvelope\":true"
           ",\"readyForOperatorValueSupply\":false"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForRuntimePayloadAcceptance\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"valueSupplyEnvelopeDeclared\":true"
           ",\"valueSupplyEnvelopeDisabledByDefault\":true"
           ",\"valueSupplyAdapterInstalled\":false"
           ",\"valueSupplyAdapterEnabled\":false"
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
           ",\"envelopeValuesPersisted\":false"
           ",\"valueSupplyEnvelopePackageOnly\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"operator value supply",
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
           json_string_array({"read value-supply envelope evidence only",
                              "treat v561-v585 value draft release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat operator values as absent and not accepted",
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

std::string value_supply_envelope_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_value_supply_stage_count,
                                                                   planned_value_supply_stage_count);
}

int published_stage_count() {
    return current_value_supply_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_envelope
