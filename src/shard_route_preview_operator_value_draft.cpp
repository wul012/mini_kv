#include "minikv/shard_route_preview_operator_value_draft.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_import_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_draft_slots.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_draft {
namespace {

constexpr int first_value_draft_release_number = 561;
constexpr int current_value_draft_stage_count = 1;
constexpr int planned_value_draft_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v911-controlled-read-only-shard-preview-operator-evidence-value-draft-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_value_draft_stage_count> value_draft_stages = {{
    {1,
     "v561",
     "route-preview-value-draft-slot-template-split",
     "v560",
     "fixtures/release/shard-readiness-v560.json",
     "starts a read-only operator evidence value draft and splits draft slot templates"},
    {2,
     "v562",
     "route-preview-value-draft-source-preflight-contract",
     "v561",
     "fixtures/release/shard-readiness-v561.json",
     "pins the source import preflight command and contract without accepting values"},
    {3,
     "v563",
     "route-preview-value-draft-source-release-freeze",
     "v562",
     "fixtures/release/shard-readiness-v562.json",
     "freezes the prior value draft fixture before rolling current evidence"},
    {4,
     "v564",
     "route-preview-value-draft-stage-chain-audit",
     "v563",
     "fixtures/release/shard-readiness-v563.json",
     "audits value draft stage sequence continuity through the shared helper"},
    {5,
     "v565",
     "route-preview-value-draft-fixture-chain-audit",
     "v564",
     "fixtures/release/shard-readiness-v564.json",
     "audits value draft source fixture continuity without runtime file reads"},
    {6,
     "v566",
     "route-preview-value-draft-command-catalog-contract",
     "v565",
     "fixtures/release/shard-readiness-v565.json",
     "pins COMMANDS and COMMANDSJSON metadata for the value draft command"},
    {7,
     "v567",
     "route-preview-value-draft-explain-contract",
     "v566",
     "fixtures/release/shard-readiness-v566.json",
     "pins EXPLAINJSON metadata-read semantics for the value draft command"},
    {8,
     "v568",
     "route-preview-value-draft-checkjson-contract",
     "v567",
     "fixtures/release/shard-readiness-v567.json",
     "pins CHECKJSON no-execution semantics for the value draft command"},
    {9,
     "v569",
     "route-preview-value-draft-shardjson-embedding",
     "v568",
     "fixtures/release/shard-readiness-v568.json",
     "embeds the value draft section into SHARDJSON once"},
    {10,
     "v570",
     "route-preview-value-draft-shardjson-embedding-audit",
     "v569",
     "fixtures/release/shard-readiness-v569.json",
     "audits that SHARDJSON carries exactly one value draft section"},
    {11,
     "v571",
     "route-preview-value-draft-slot-builder-contract",
     "v570",
     "fixtures/release/shard-readiness-v570.json",
     "declares the slot builder as contract metadata only, not executable transformation"},
    {12,
     "v572",
     "route-preview-value-draft-slot-count-audit",
     "v571",
     "fixtures/release/shard-readiness-v571.json",
     "audits that value draft slots match the expected operator evidence slot count"},
    {13,
     "v573",
     "route-preview-value-draft-actual-value-state",
     "v572",
     "fixtures/release/shard-readiness-v572.json",
     "pins actual value state as not supplied for every draft slot"},
    {14,
     "v574",
     "route-preview-value-draft-import-state-blocked",
     "v573",
     "fixtures/release/shard-readiness-v573.json",
     "keeps evidence import blocked after value draft metadata is visible"},
    {15,
     "v575",
     "route-preview-value-draft-redaction-boundary",
     "v574",
     "fixtures/release/shard-readiness-v574.json",
     "proves placeholder redaction boundaries preserve no secret, credential, or endpoint values"},
    {16,
     "v576",
     "route-preview-value-draft-no-evidence-import-audit",
     "v575",
     "fixtures/release/shard-readiness-v575.json",
     "audits that no operator evidence value import path is enabled"},
    {17,
     "v577",
     "route-preview-value-draft-no-manual-entry-audit",
     "v576",
     "fixtures/release/shard-readiness-v576.json",
     "audits that manual evidence entry remains blocked from mini-kv storage"},
    {18,
     "v578",
     "route-preview-value-draft-no-runtime-payload-audit",
     "v577",
     "fixtures/release/shard-readiness-v577.json",
     "audits that runtime payloads are not accepted by mini-kv"},
    {19,
     "v579",
     "route-preview-value-draft-no-synthetic-evidence-audit",
     "v578",
     "fixtures/release/shard-readiness-v578.json",
     "audits that synthetic evidence is rejected rather than normalized into accepted data"},
    {20,
     "v580",
     "route-preview-value-draft-no-secret-storage-audit",
     "v579",
     "fixtures/release/shard-readiness-v579.json",
     "audits that value drafts store no credential, secret, or raw endpoint value"},
    {21,
     "v581",
     "route-preview-value-draft-no-router-audit",
     "v580",
     "fixtures/release/shard-readiness-v580.json",
     "audits that value drafts install no active shard router"},
    {22,
     "v582",
     "route-preview-value-draft-no-write-audit",
     "v581",
     "fixtures/release/shard-readiness-v581.json",
     "audits that value drafts route no writes and touch no WAL"},
    {23,
     "v583",
     "route-preview-value-draft-no-execution-audit",
     "v582",
     "fixtures/release/shard-readiness-v582.json",
     "audits that value drafts cannot trigger restore, load, compact, routing, or service execution"},
    {24,
     "v584",
     "route-preview-value-draft-release-readiness-audit",
     "v583",
     "fixtures/release/shard-readiness-v583.json",
     "audits release readiness while preserving value-absent and no-execution boundaries"},
    {25,
     "v585",
     "route-preview-value-draft-release-package",
     "v584",
     "fixtures/release/shard-readiness-v584.json",
     "packages final v561-v585 value draft evidence for read-only downstream consumption"},
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
    return value_draft_stages.at(static_cast<std::size_t>(current_value_draft_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(value_draft_stages,
                                                                current_value_draft_stage_count,
                                                                planned_value_draft_stage_count,
                                                                first_value_draft_release_number);
}

} // namespace

std::string format_value_draft_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(value_draft_stages,
                                                                        current_value_draft_stage_count);
}

std::string format_value_draft_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = shard_route_preview_operator_import_preflight::published_stage_count() == 25;

    return "{\"contract\":\"shard-route-preview-operator-value-draft.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUEDRAFTJSON\""
           ",\"draftMode\":\"controlled-read-only-operator-evidence-value-draft\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceImportPreflightCommand\":\"SHARDROUTEIMPORTPREFLIGHTJSON\""
           ",\"sourceImportPreflightReleaseVersion\":\"v560\""
           ",\"sourceImportPreflightFixturePath\":\"fixtures/release/shard-readiness-v560.json\""
           ",\"sourceImportPreflightArchiveRootHint\":\"e/560/\""
           ",\"sourceImportPreflightPublishedStageCount\":" +
           std::to_string(shard_route_preview_operator_import_preflight::published_stage_count()) +
           ",\"sourceImportPreflightPlannedStageCount\":25"
           ",\"sourceImportPreflightChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceImportPreflightDigestMarker\":" +
           json_string(shard_route_preview_operator_import_preflight::import_preflight_digest_marker()) +
           ",\"valueDraftStage\":" +
           json_string(stage.stage) +
           ",\"valueDraftStageSequence\":" + std::to_string(stage.sequence) +
           ",\"valueDraftReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_value_draft_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_value_draft_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceImportPreflightReleaseRangeStart\":\"v536\""
           ",\"sourceImportPreflightReleaseRangeEnd\":\"v560\""
           ",\"sourceImportPreflightReleaseCount\":25"
           ",\"operatorValueDraftSlotCount\":25"
           ",\"expectedOperatorEvidenceSlotCount\":25"
           ",\"draftSlotCount\":" +
           std::to_string(shard_route_preview_operator_value_draft_slots::planned_value_draft_slot_count()) +
           ",\"publishedValueDraftSlotTemplateCount\":" + std::to_string(current_value_draft_stage_count) +
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"actualValueState\":\"not-supplied\""
           ",\"draftValueState\":\"awaiting-operator-value\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"valueDraftSlotHelper\":\"shard_route_preview_operator_value_draft_slots\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"valueDraftStageCatalog\":" +
           format_value_draft_stage_catalog_json() +
           ",\"valueDraftStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"valueDraftSlots\":" +
           shard_route_preview_operator_value_draft_slots::format_value_draft_slots_json(
               current_value_draft_stage_count) +
           ",\"draftChecks\":" +
           json_string_array({"source import preflight command remains available",
                              "source import preflight chain remains complete",
                              "source import preflight digest marker remains pinned",
                              "v536-v560 source import preflight range remains declared and versioned",
                              "Node v911 value draft plan is referenced as plan evidence only",
                              "operator evidence value draft is declared ready",
                              "actual operator values remain not supplied",
                              "value supply adapter is not installed or enabled",
                              "evidence import is still blocked",
                              "manual evidence entry is still blocked",
                              "runtime payloads are not accepted",
                              "synthetic evidence is not accepted",
                              "credential, secret, and raw endpoint values are not stored",
                              "value draft slots are template labels only",
                              "current SHARDJSON embeds exactly one value draft section",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"draftCheckCount\":16"
           ",\"draftPassedCount\":16"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"valueDraftSlotHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleValueDraftSectionExpected\":true"
           ",\"valueDraftChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"readyForOperatorEvidenceValueDraft\":true"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"operatorValueSupplied\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorEvidenceValuesAccepted\":false"
           ",\"valueSupplyAdapterInstalled\":false"
           ",\"valueSupplyAdapterEnabled\":false"
           ",\"evidenceImportAllowed\":false"
           ",\"manualEvidenceImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"syntheticEvidenceRejected\":true"
           ",\"secretValueStored\":false"
           ",\"credentialValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"normalizerExecutes\":false"
           ",\"normalizationExecutionAllowed\":false"
           ",\"redactionPreserved\":true"
           ",\"draftValuesPersisted\":false"
           ",\"valueDraftPackageOnly\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"blockedCapabilities\":" +
           json_string_array({"operator evidence value import",
                              "manual evidence entry",
                              "runtime payload acceptance",
                              "synthetic evidence normalization",
                              "credential or secret storage",
                              "raw endpoint storage",
                              "active shard router installation",
                              "write routing",
                              "restore, load, compact, service start, or live read execution"}) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read value draft evidence only",
                              "treat v536-v560 import preflight release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat operator evidence value slots as placeholders, not accepted values",
                              "do not supply or import operator evidence values through mini-kv",
                              "do not normalize synthetic evidence into accepted evidence",
                              "do not store credential, secret, or raw endpoint values",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           route_preview_evidence_boundary::format_no_execution_boundary_fields() + "}";
}

std::string value_draft_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_value_draft_stage_count,
                                                                   planned_value_draft_stage_count);
}

int published_stage_count() {
    return current_value_draft_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_draft
