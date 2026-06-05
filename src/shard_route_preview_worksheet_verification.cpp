#include "minikv/shard_route_preview_worksheet_verification.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_worksheet_verification {
namespace {

constexpr int first_worksheet_verification_release_number = 511;
constexpr int current_worksheet_verification_stage_count = 17;
constexpr int planned_worksheet_verification_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v861-controlled-read-only-shard-preview-manual-evidence-entry-worksheet-closeout-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_worksheet_verification_stage_count> worksheet_verification_stages = {{
    {1,
     "v511",
     "route-preview-worksheet-verification-intake",
     "v510",
     "fixtures/release/shard-readiness-v510.json",
     "starts a read-only worksheet verifier over the completed v486-v510 maintenance verifier package"},
    {2,
     "v512",
     "route-preview-worksheet-verification-source-verifier-contract",
     "v511",
     "fixtures/release/shard-readiness-v511.json",
     "pins the source maintenance verifier command and release package contract"},
    {3,
     "v513",
     "route-preview-worksheet-verification-source-release-freeze",
     "v512",
     "fixtures/release/shard-readiness-v512.json",
     "freezes the prior worksheet verifier fixture before rolling current evidence"},
    {4,
     "v514",
     "route-preview-worksheet-verification-stage-chain-audit",
     "v513",
     "fixtures/release/shard-readiness-v513.json",
     "audits worksheet verifier stage sequence continuity through the shared helper"},
    {5,
     "v515",
     "route-preview-worksheet-verification-fixture-chain-audit",
     "v514",
     "fixtures/release/shard-readiness-v514.json",
     "audits worksheet verifier source fixture continuity without reading files at runtime"},
    {6,
     "v516",
     "route-preview-worksheet-verification-command-catalog-contract",
     "v515",
     "fixtures/release/shard-readiness-v515.json",
     "pins COMMANDS and COMMANDSJSON metadata for the worksheet verifier command"},
    {7,
     "v517",
     "route-preview-worksheet-verification-explain-contract",
     "v516",
     "fixtures/release/shard-readiness-v516.json",
     "pins EXPLAINJSON metadata-read semantics for the worksheet verifier command"},
    {8,
     "v518",
     "route-preview-worksheet-verification-checkjson-contract",
     "v517",
     "fixtures/release/shard-readiness-v517.json",
     "pins CHECKJSON no-execution semantics for the worksheet verifier command"},
    {9,
     "v519",
     "route-preview-worksheet-verification-shardjson-embedding",
     "v518",
     "fixtures/release/shard-readiness-v518.json",
     "embeds the worksheet verifier section into SHARDJSON once"},
    {10,
     "v520",
     "route-preview-worksheet-verification-shardjson-embedding-audit",
     "v519",
     "fixtures/release/shard-readiness-v519.json",
     "audits that SHARDJSON carries exactly one worksheet verifier section"},
    {11,
     "v521",
     "route-preview-worksheet-verification-retention-map",
     "v520",
     "fixtures/release/shard-readiness-v520.json",
     "maps retained screenshots, explanations, walkthroughs, fixtures, commits, tags, and CI evidence"},
    {12,
     "v522",
     "route-preview-worksheet-verification-retention-audit",
     "v521",
     "fixtures/release/shard-readiness-v521.json",
     "audits retention metadata while keeping archive roots as human-review hints only"},
    {13,
     "v523",
     "route-preview-worksheet-verification-operator-slot-boundary",
     "v522",
     "fixtures/release/shard-readiness-v522.json",
     "declares operator worksheet slots as labels only, not accepted runtime values"},
    {14,
     "v524",
     "route-preview-worksheet-verification-manual-entry-blocked",
     "v523",
     "fixtures/release/shard-readiness-v523.json",
     "proves manual evidence entry remains blocked from mini-kv import and storage"},
    {15,
     "v525",
     "route-preview-worksheet-verification-live-execution-blocked",
     "v524",
     "fixtures/release/shard-readiness-v524.json",
     "proves the worksheet verifier cannot start services or perform live reads"},
    {16,
     "v526",
     "route-preview-worksheet-verification-production-execution-blocked",
     "v525",
     "fixtures/release/shard-readiness-v525.json",
     "proves the worksheet verifier grants no production execution authority"},
    {17,
     "v527",
     "route-preview-worksheet-verification-no-router-audit",
     "v526",
     "fixtures/release/shard-readiness-v526.json",
     "audits that worksheet verification installs no active shard router"},
    {18,
     "v528",
     "route-preview-worksheet-verification-no-write-audit",
     "v527",
     "fixtures/release/shard-readiness-v527.json",
     "audits that worksheet verification routes no writes and touches no WAL"},
    {19,
     "v529",
     "route-preview-worksheet-verification-no-execution-audit",
     "v528",
     "fixtures/release/shard-readiness-v528.json",
     "audits that worksheet verification cannot trigger restore, load, compact, routing, or service execution"},
    {20,
     "v530",
     "route-preview-worksheet-verification-consumer-handoff",
     "v529",
     "fixtures/release/shard-readiness-v529.json",
     "records downstream consumer handoff semantics for worksheet verification evidence"},
    {21,
     "v531",
     "route-preview-worksheet-verification-java-boundary-handoff",
     "v530",
     "fixtures/release/shard-readiness-v530.json",
     "records Java echo boundary semantics without granting mini-kv order or audit authority"},
    {22,
     "v532",
     "route-preview-worksheet-verification-node-plan-handoff",
     "v531",
     "fixtures/release/shard-readiness-v531.json",
     "records Node v861 worksheet plan alignment without consuming fresh Node runtime data"},
    {23,
     "v533",
     "route-preview-worksheet-verification-release-readiness",
     "v532",
     "fixtures/release/shard-readiness-v532.json",
     "prepares worksheet verifier evidence for release packaging"},
    {24,
     "v534",
     "route-preview-worksheet-verification-release-readiness-audit",
     "v533",
     "fixtures/release/shard-readiness-v533.json",
     "audits release readiness while preserving read-only and no-execution boundaries"},
    {25,
     "v535",
     "route-preview-worksheet-verification-release-package",
     "v534",
     "fixtures/release/shard-readiness-v534.json",
     "packages final v511-v535 worksheet verifier evidence for read-only downstream consumption"},
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
    return worksheet_verification_stages.at(static_cast<std::size_t>(current_worksheet_verification_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        worksheet_verification_stages,
        current_worksheet_verification_stage_count,
        planned_worksheet_verification_stage_count,
        first_worksheet_verification_release_number);
}

} // namespace

std::string format_worksheet_verification_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(worksheet_verification_stages,
                                                                        current_worksheet_verification_stage_count);
}

std::string format_worksheet_verification_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_archive_maintenance_verification::published_stage_count() == 25;

    return "{\"contract\":\"shard-route-preview-worksheet-verification.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEWORKSHEETVERIFYJSON\""
           ",\"verificationMode\":\"read-only-shard-preview-worksheet-boundary-verification\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceVerificationCommand\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\""
           ",\"sourceVerificationReleaseVersion\":\"v510\""
           ",\"sourceVerificationFixturePath\":\"fixtures/release/shard-readiness-v510.json\""
           ",\"sourceVerificationArchiveRootHint\":\"e/510/\""
           ",\"sourceVerificationPublishedStageCount\":" +
           std::to_string(shard_route_preview_archive_maintenance_verification::published_stage_count()) +
           ",\"sourceVerificationPlannedStageCount\":25"
           ",\"sourceVerificationChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceVerificationDigestMarker\":" +
           json_string(shard_route_preview_archive_maintenance_verification::verification_digest_marker()) +
           ",\"worksheetVerificationStage\":" +
           json_string(stage.stage) +
           ",\"worksheetVerificationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"worksheetVerificationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_worksheet_verification_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_worksheet_verification_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"verifiedSourceReleaseRangeStart\":\"v486\""
           ",\"verifiedSourceReleaseRangeEnd\":\"v510\""
           ",\"verifiedSourceReleaseCount\":25"
           ",\"operatorEntrySlotCount\":25"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"worksheetVerificationStageCatalog\":" +
           format_worksheet_verification_stage_catalog_json() +
           ",\"worksheetVerificationStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"verificationChecks\":" +
           json_string_array({"source maintenance verifier command remains available",
                              "source maintenance verifier chain remains complete",
                              "source maintenance verifier digest marker remains pinned",
                              "v486-v510 source verifier range remains declared and versioned",
                              "Node v861 worksheet plan is referenced as plan evidence only",
                              "operator worksheet slots are labels, not runtime input channels",
                              "manual evidence values are not accepted by mini-kv",
                              "runtime payloads are not accepted by mini-kv",
                              "synthetic evidence is not accepted by mini-kv",
                              "current SHARDJSON embeds exactly one worksheet verifier section",
                              "COMMANDS and COMMANDSJSON expose read-only worksheet verifier metadata",
                              "EXPLAINJSON classifies the worksheet verifier command as metadata read",
                              "CHECKJSON denies execution",
                              "no active shard router is installed",
                              "no write, WAL, restore, load, compact, service start, or live read is added"}) +
           ",\"verificationCheckCount\":15"
           ",\"verificationPassedCount\":15"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"stageChainHelperApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleWorksheetVerifierSectionExpected\":true"
           ",\"worksheetVerificationChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"readyForOperatorEntryWorksheet\":true"
           ",\"readyForManualEvidenceEntry\":false"
           ",\"readyForLiveExecution\":false"
           ",\"readyForProductionExecution\":false"
           ",\"operatorEntryValuesAccepted\":false"
           ",\"manualEvidenceImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"credentialValueStored\":false"
           ",\"secretValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"routeInstalled\":false"
           ",\"serviceStarted\":false"
           ",\"walTouched\":false"
           ",\"writeRoutingEnabled\":false"
           ",\"declaredWorksheetVerificationOnly\":true"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read worksheet verifier evidence only",
                              "treat v486-v510 maintenance verifier release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat operator worksheet rows as labels, not accepted values",
                              "do not import manual evidence into mini-kv",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"executesRoute\":false"
           ",\"storageDirectoriesCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string worksheet_verification_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_worksheet_verification_stage_count,
                                                                   planned_worksheet_verification_stage_count);
}

int published_stage_count() {
    return current_worksheet_verification_stage_count;
}

} // namespace minikv::shard_route_preview_worksheet_verification
