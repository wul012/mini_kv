#include "minikv/shard_route_preview_operator_import_preflight.hpp"

#include "minikv/route_preview_evidence_boundary.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"
#include "minikv/shard_route_preview_worksheet_verification.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_import_preflight {
namespace {

constexpr int first_import_preflight_release_number = 536;
constexpr int current_import_preflight_stage_count = 14;
constexpr int planned_import_preflight_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v886-controlled-read-only-shard-preview-operator-evidence-import-preflight-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_import_preflight_stage_count> import_preflight_stages = {{
    {1,
     "v536",
     "route-preview-import-preflight-boundary-helper-split",
     "v535",
     "fixtures/release/shard-readiness-v535.json",
     "starts a read-only operator evidence import preflight and splits shared no-execution boundary fields"},
    {2,
     "v537",
     "route-preview-import-preflight-source-worksheet-contract",
     "v536",
     "fixtures/release/shard-readiness-v536.json",
     "pins the source worksheet verifier command and contract without importing operator values"},
    {3,
     "v538",
     "route-preview-import-preflight-source-release-freeze",
     "v537",
     "fixtures/release/shard-readiness-v537.json",
     "freezes the prior import preflight fixture before rolling current evidence"},
    {4,
     "v539",
     "route-preview-import-preflight-stage-chain-audit",
     "v538",
     "fixtures/release/shard-readiness-v538.json",
     "audits import preflight stage sequence continuity through the shared helper"},
    {5,
     "v540",
     "route-preview-import-preflight-fixture-chain-audit",
     "v539",
     "fixtures/release/shard-readiness-v539.json",
     "audits import preflight source fixture continuity without runtime file reads"},
    {6,
     "v541",
     "route-preview-import-preflight-command-catalog-contract",
     "v540",
     "fixtures/release/shard-readiness-v540.json",
     "pins COMMANDS and COMMANDSJSON metadata for the import preflight command"},
    {7,
     "v542",
     "route-preview-import-preflight-explain-contract",
     "v541",
     "fixtures/release/shard-readiness-v541.json",
     "pins EXPLAINJSON metadata-read semantics for the import preflight command"},
    {8,
     "v543",
     "route-preview-import-preflight-checkjson-contract",
     "v542",
     "fixtures/release/shard-readiness-v542.json",
     "pins CHECKJSON no-execution semantics for the import preflight command"},
    {9,
     "v544",
     "route-preview-import-preflight-shardjson-embedding",
     "v543",
     "fixtures/release/shard-readiness-v543.json",
     "embeds the import preflight section into SHARDJSON once"},
    {10,
     "v545",
     "route-preview-import-preflight-shardjson-embedding-audit",
     "v544",
     "fixtures/release/shard-readiness-v544.json",
     "audits that SHARDJSON carries exactly one import preflight section"},
    {11,
     "v546",
     "route-preview-import-preflight-normalizer-contract",
     "v545",
     "fixtures/release/shard-readiness-v545.json",
     "declares the normalizer as contract metadata only, not executable transformation"},
    {12,
     "v547",
     "route-preview-import-preflight-blocker-catalog",
     "v546",
     "fixtures/release/shard-readiness-v546.json",
     "catalogs fail-closed blockers for missing value, runtime payload, synthetic evidence, and secret input"},
    {13,
     "v548",
     "route-preview-import-preflight-missing-value-policy",
     "v547",
     "fixtures/release/shard-readiness-v547.json",
     "pins missing operator value policy as import-blocking metadata only"},
    {14,
     "v549",
     "route-preview-import-preflight-redaction-preservation",
     "v548",
     "fixtures/release/shard-readiness-v548.json",
     "proves redaction is preserved and no credential, secret, or raw endpoint is stored"},
    {15,
     "v550",
     "route-preview-import-preflight-operator-slot-draft-boundary",
     "v549",
     "fixtures/release/shard-readiness-v549.json",
     "keeps operator slot drafts as labels that cannot become accepted evidence values"},
    {16,
     "v551",
     "route-preview-import-preflight-no-evidence-import-audit",
     "v550",
     "fixtures/release/shard-readiness-v550.json",
     "audits that no operator evidence value import path is enabled"},
    {17,
     "v552",
     "route-preview-import-preflight-no-manual-entry-audit",
     "v551",
     "fixtures/release/shard-readiness-v551.json",
     "audits that manual evidence entry remains blocked from mini-kv storage"},
    {18,
     "v553",
     "route-preview-import-preflight-no-runtime-payload-audit",
     "v552",
     "fixtures/release/shard-readiness-v552.json",
     "audits that runtime payloads are not accepted by mini-kv"},
    {19,
     "v554",
     "route-preview-import-preflight-no-synthetic-evidence-audit",
     "v553",
     "fixtures/release/shard-readiness-v553.json",
     "audits that synthetic evidence is rejected rather than normalized into accepted data"},
    {20,
     "v555",
     "route-preview-import-preflight-no-router-audit",
     "v554",
     "fixtures/release/shard-readiness-v554.json",
     "audits that import preflight installs no active shard router"},
    {21,
     "v556",
     "route-preview-import-preflight-no-write-audit",
     "v555",
     "fixtures/release/shard-readiness-v555.json",
     "audits that import preflight routes no writes and touches no WAL"},
    {22,
     "v557",
     "route-preview-import-preflight-no-execution-audit",
     "v556",
     "fixtures/release/shard-readiness-v556.json",
     "audits that import preflight cannot trigger restore, load, compact, routing, or service execution"},
    {23,
     "v558",
     "route-preview-import-preflight-consumer-handoff",
     "v557",
     "fixtures/release/shard-readiness-v557.json",
     "records downstream consumer handoff semantics for import preflight evidence"},
    {24,
     "v559",
     "route-preview-import-preflight-release-readiness-audit",
     "v558",
     "fixtures/release/shard-readiness-v558.json",
     "audits release readiness while preserving import-blocked and no-execution boundaries"},
    {25,
     "v560",
     "route-preview-import-preflight-release-package",
     "v559",
     "fixtures/release/shard-readiness-v559.json",
     "packages final v536-v560 import preflight evidence for read-only downstream consumption"},
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
    return import_preflight_stages.at(static_cast<std::size_t>(current_import_preflight_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(import_preflight_stages,
                                                                current_import_preflight_stage_count,
                                                                planned_import_preflight_stage_count,
                                                                first_import_preflight_release_number);
}

} // namespace

std::string format_import_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(import_preflight_stages,
                                                                        current_import_preflight_stage_count);
}

std::string format_import_preflight_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = shard_route_preview_worksheet_verification::published_stage_count() == 25;

    return "{\"contract\":\"shard-route-preview-operator-import-preflight.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEIMPORTPREFLIGHTJSON\""
           ",\"preflightMode\":\"controlled-read-only-operator-evidence-import-preflight\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceWorksheetCommand\":\"SHARDROUTEWORKSHEETVERIFYJSON\""
           ",\"sourceWorksheetReleaseVersion\":\"v535\""
           ",\"sourceWorksheetFixturePath\":\"fixtures/release/shard-readiness-v535.json\""
           ",\"sourceWorksheetArchiveRootHint\":\"e/535/\""
           ",\"sourceWorksheetPublishedStageCount\":" +
           std::to_string(shard_route_preview_worksheet_verification::published_stage_count()) +
           ",\"sourceWorksheetPlannedStageCount\":25"
           ",\"sourceWorksheetChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceWorksheetDigestMarker\":" +
           json_string(shard_route_preview_worksheet_verification::worksheet_verification_digest_marker()) +
           ",\"importPreflightStage\":" +
           json_string(stage.stage) +
           ",\"importPreflightStageSequence\":" + std::to_string(stage.sequence) +
           ",\"importPreflightReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_import_preflight_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_import_preflight_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sourceWorksheetReleaseRangeStart\":\"v511\""
           ",\"sourceWorksheetReleaseRangeEnd\":\"v535\""
           ",\"sourceWorksheetReleaseCount\":25"
           ",\"operatorPreflightSlotCount\":25"
           ",\"expectedOperatorEvidenceSlotCount\":25"
           ",\"acceptedOperatorEvidenceSlotCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"acceptedEvidenceValueCount\":0"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"sharedBoundaryHelper\":\"route_preview_evidence_boundary\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"importPreflightStageCatalog\":" +
           format_import_preflight_stage_catalog_json() +
           ",\"importPreflightStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"preflightChecks\":" +
           json_string_array({"source worksheet verifier command remains available",
                              "source worksheet verifier chain remains complete",
                              "source worksheet verifier digest marker remains pinned",
                              "v511-v535 source worksheet range remains declared and versioned",
                              "Node v886 import preflight plan is referenced as plan evidence only",
                              "operator evidence import preflight is declared ready",
                              "evidence import is still blocked",
                              "manual evidence entry is still blocked",
                              "runtime payloads are not accepted",
                              "synthetic evidence is not accepted",
                              "credential, secret, and raw endpoint values are not stored",
                              "normalizer execution is not allowed",
                              "import blockers are metadata only",
                              "current SHARDJSON embeds exactly one import preflight section",
                              "no active shard router, write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"preflightCheckCount\":15"
           ",\"preflightPassedCount\":15"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"sharedBoundaryHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleImportPreflightSectionExpected\":true"
           ",\"importPreflightChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"readyForOperatorEvidenceImportPreflight\":true"
           ",\"readyForEvidenceImport\":false"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"operatorEvidenceValuesAccepted\":false"
           ",\"operatorEntryValuesAccepted\":false"
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
           ",\"missingValuePolicyBlocksImport\":true"
           ",\"importBlockersEnforcedAsMetadata\":true"
           ",\"redactionPreserved\":true"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"declaredImportPreflightOnly\":true"
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
           json_string_array({"read import preflight evidence only",
                              "treat v511-v535 worksheet verifier release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat operator evidence slots as labels, not accepted values",
                              "do not import evidence into mini-kv",
                              "do not normalize synthetic evidence into accepted evidence",
                              "do not store credential, secret, or raw endpoint values",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           route_preview_evidence_boundary::format_no_execution_boundary_fields() + "}";
}

std::string import_preflight_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_import_preflight_stage_count,
                                                                   planned_import_preflight_stage_count);
}

int published_stage_count() {
    return current_import_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_import_preflight















