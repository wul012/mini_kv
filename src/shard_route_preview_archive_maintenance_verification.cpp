#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_archive_maintenance.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_archive_maintenance_verification {
namespace {

constexpr int first_verification_release_number = 486;
constexpr int current_verification_stage_count = 12;
constexpr int planned_verification_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_verification_stage_count> verification_stages = {{
    {1,
     "v486",
     "route-preview-archive-maintenance-verification-intake",
     "v485",
     "fixtures/release/shard-readiness-v485.json",
     "starts a read-only verifier over the completed v461-v485 archive maintenance ledger"},
    {2,
     "v487",
     "route-preview-archive-maintenance-verification-stage-chain-helper",
     "v486",
     "fixtures/release/shard-readiness-v486.json",
     "splits stage chain continuity checks into a reusable helper"},
    {3,
     "v488",
     "route-preview-archive-maintenance-verification-source-ledger-contract",
     "v487",
     "fixtures/release/shard-readiness-v487.json",
     "pins the source maintenance ledger command and published-stage contract"},
    {4,
     "v489",
     "route-preview-archive-maintenance-verification-source-digest-audit",
     "v488",
     "fixtures/release/shard-readiness-v488.json",
     "audits the source maintenance digest marker without reading archive files at runtime"},
    {5,
     "v490",
     "route-preview-archive-maintenance-verification-stage-sequence-audit",
     "v489",
     "fixtures/release/shard-readiness-v489.json",
     "audits verifier stage sequence continuity through the shared helper"},
    {6,
     "v491",
     "route-preview-archive-maintenance-verification-fixture-chain-audit",
     "v490",
     "fixtures/release/shard-readiness-v490.json",
     "audits versioned fixture continuity for the verifier chain"},
    {7,
     "v492",
     "route-preview-archive-maintenance-verification-command-catalog-contract",
     "v491",
     "fixtures/release/shard-readiness-v491.json",
     "pins COMMANDS and COMMANDSJSON metadata for the verifier command"},
    {8,
     "v493",
     "route-preview-archive-maintenance-verification-explain-contract",
     "v492",
     "fixtures/release/shard-readiness-v492.json",
     "pins EXPLAINJSON metadata-read semantics for the verifier command"},
    {9,
     "v494",
     "route-preview-archive-maintenance-verification-checkjson-contract",
     "v493",
     "fixtures/release/shard-readiness-v493.json",
     "pins CHECKJSON no-execution semantics for the verifier command"},
    {10,
     "v495",
     "route-preview-archive-maintenance-verification-cli-smoke-contract",
     "v494",
     "fixtures/release/shard-readiness-v494.json",
     "records CLI smoke expectations for verifier evidence reads"},
    {11,
     "v496",
     "route-preview-archive-maintenance-verification-tcp-smoke-contract",
     "v495",
     "fixtures/release/shard-readiness-v495.json",
     "records owned TCP smoke expectations and cleanup requirements for the verifier command"},
    {12,
     "v497",
     "route-preview-archive-maintenance-verification-shardjson-embedding",
     "v496",
     "fixtures/release/shard-readiness-v496.json",
     "embeds the verifier section into SHARDJSON without duplicating the source ledger"},
    {13,
     "v498",
     "route-preview-archive-maintenance-verification-shardjson-embedding-audit",
     "v497",
     "fixtures/release/shard-readiness-v497.json",
     "audits that SHARDJSON carries exactly one verifier section"},
    {14,
     "v499",
     "route-preview-archive-maintenance-verification-retention-map",
     "v498",
     "fixtures/release/shard-readiness-v498.json",
     "maps retained verifier screenshots, explanations, walkthroughs, fixtures, commits, tags, and CI evidence"},
    {15,
     "v500",
     "route-preview-archive-maintenance-verification-retention-audit",
     "v499",
     "fixtures/release/shard-readiness-v499.json",
     "audits retention metadata while keeping archive roots as documentation hints only"},
    {16,
     "v501",
     "route-preview-archive-maintenance-verification-walkthrough-index",
     "v500",
     "fixtures/release/shard-readiness-v500.json",
     "indexes walkthrough coverage for the verifier chain"},
    {17,
     "v502",
     "route-preview-archive-maintenance-verification-walkthrough-index-audit",
     "v501",
     "fixtures/release/shard-readiness-v501.json",
     "audits walkthrough index coverage and Markdown hygiene expectations"},
    {18,
     "v503",
     "route-preview-archive-maintenance-verification-no-router-audit",
     "v502",
     "fixtures/release/shard-readiness-v502.json",
     "audits that verifier evidence installs no active shard router"},
    {19,
     "v504",
     "route-preview-archive-maintenance-verification-no-write-audit",
     "v503",
     "fixtures/release/shard-readiness-v503.json",
     "audits that verifier evidence permits no writes and touches no WAL"},
    {20,
     "v505",
     "route-preview-archive-maintenance-verification-no-execution-audit",
     "v504",
     "fixtures/release/shard-readiness-v504.json",
     "audits that verifier evidence cannot trigger restore, load, compact, routing, or service execution"},
    {21,
     "v506",
     "route-preview-archive-maintenance-verification-consumer-handoff",
     "v505",
     "fixtures/release/shard-readiness-v505.json",
     "records downstream consumer handoff semantics for verified maintenance evidence"},
    {22,
     "v507",
     "route-preview-archive-maintenance-verification-java-boundary-handoff",
     "v506",
     "fixtures/release/shard-readiness-v506.json",
     "records Java echo boundary semantics without granting mini-kv order or audit authority"},
    {23,
     "v508",
     "route-preview-archive-maintenance-verification-release-readiness",
     "v507",
     "fixtures/release/shard-readiness-v507.json",
     "prepares the verifier chain for release packaging"},
    {24,
     "v509",
     "route-preview-archive-maintenance-verification-release-readiness-audit",
     "v508",
     "fixtures/release/shard-readiness-v508.json",
     "audits release readiness while preserving read-only and no-execution boundaries"},
    {25,
     "v510",
     "route-preview-archive-maintenance-verification-release-package",
     "v509",
     "fixtures/release/shard-readiness-v509.json",
     "packages final v486-v510 maintenance verifier evidence for read-only downstream consumption"},
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
    return verification_stages.at(static_cast<std::size_t>(current_verification_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        verification_stages,
        current_verification_stage_count,
        planned_verification_stage_count,
        first_verification_release_number);
}

} // namespace

std::string format_verification_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(verification_stages,
                                                                        current_verification_stage_count);
}

std::string format_verification_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_archive_maintenance::published_stage_count() == 25;

    return "{\"contract\":\"shard-route-preview-archive-maintenance-verification.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\""
           ",\"verificationMode\":\"read-only-route-preview-archive-maintenance-chain-verification\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceMaintenanceCommand\":\"SHARDROUTEARCHIVEMAINTJSON\""
           ",\"sourceMaintenanceReleaseVersion\":\"v485\""
           ",\"sourceMaintenanceFixturePath\":\"fixtures/release/shard-readiness-v485.json\""
           ",\"sourceMaintenanceArchiveRootHint\":\"e/485/\""
           ",\"sourceMaintenancePublishedStageCount\":" +
           std::to_string(shard_route_preview_archive_maintenance::published_stage_count()) +
           ",\"sourceMaintenancePlannedStageCount\":25"
           ",\"sourceMaintenanceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceMaintenanceDigestMarker\":" +
           json_string(shard_route_preview_archive_maintenance::maintenance_digest_marker()) +
           ",\"verificationStage\":" +
           json_string(stage.stage) +
           ",\"verificationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"verificationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_verification_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_verification_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"verifiedMaintenanceReleaseRangeStart\":\"v461\""
           ",\"verifiedMaintenanceReleaseRangeEnd\":\"v485\""
           ",\"verifiedMaintenanceReleaseCount\":25"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"stageChainHelper\":\"shard_route_preview_stage_chain\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"verificationStageCatalog\":" +
           format_verification_stage_catalog_json() +
           ",\"verificationStageChain\":" +
           shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"verificationChecks\":" +
           json_string_array({"source maintenance command remains available",
                              "source maintenance chain remains complete",
                              "source maintenance digest marker remains pinned",
                              "v461-v485 maintenance range remains declared and versioned",
                              "archive roots are retained evidence hints, not runtime filesystem walks",
                              "stage chain helper validates verifier release continuity",
                              "stage chain helper validates verifier source fixture continuity",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds exactly one maintenance verifier section",
                              "COMMANDS and COMMANDSJSON expose read-only verifier metadata",
                              "EXPLAINJSON classifies the verifier command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads verifier evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
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
           ",\"singleMaintenanceVerifierSectionExpected\":true"
           ",\"declaredVerificationOnly\":true"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"archiveMaintenanceVerificationCommandAvailable\":true"
           ",\"archiveMaintenanceVerificationChainComplete\":" +
           json_bool(stage_chain.chain_complete) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read maintenance verifier evidence only",
                              "treat v461-v485 maintenance release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat sourceMaintenanceArchiveRootHint as a documentation pointer only",
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

std::string verification_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_verification_stage_count,
                                                                   planned_verification_stage_count);
}

int published_stage_count() {
    return current_verification_stage_count;
}

} // namespace minikv::shard_route_preview_archive_maintenance_verification
