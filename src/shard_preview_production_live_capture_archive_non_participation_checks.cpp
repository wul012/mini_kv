#include "minikv/shard_preview_production_live_capture_archive_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_live_capture_archive_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ProductionLiveCaptureArchiveNonParticipationCheck, 30> check_catalog = {{
    {1, "source-live-capture-non-participation-fixture-frozen", "fixtures/release/shard-readiness-v1195.json",
     "v1195 remains the immutable source live capture non-participation fixture"},
    {2, "node-archive-entrypoint-reexport-context", "Node v1633",
     "stable archive entrypoint type re-exports remain Node-owned metadata"},
    {3, "node-archive-reference-constant-context", "Node v1634",
     "Java and mini-kv archive reference constants are not imported as runtime authority"},
    {4, "node-archive-endpoint-constant-context", "Node v1635",
     "archive endpoint constants are not raw endpoint values read by mini-kv"},
    {5, "node-archive-build-parts-context", "Node v1636",
     "archive build parts are not imported into mini-kv"},
    {6, "node-archive-production-pass-context", "Node v1637",
     "archived production pass evaluation remains Node-owned"},
    {7, "node-archive-check-assembly-context", "Node v1638",
     "archive check assembly does not become a mini-kv command"},
    {8, "node-ready-for-archive-context", "Node v1639",
     "ready-for-read-only-capture-archive state does not start mini-kv services"},
    {9, "node-archive-state-context", "Node v1640",
     "archive-state resolution does not trigger mini-kv archive reads"},
    {10, "node-archive-digest-context", "Node v1641",
     "stable digest input shape is not imported as archive payload"},
    {11, "node-archive-blocker-context", "Node v1642",
     "blocker collection stays inside Node"},
    {12, "node-archive-warning-context", "Node v1643",
     "warning generation stays inside Node"},
    {13, "node-archive-recommendation-context", "Node v1644",
     "recommendation generation stays inside Node"},
    {14, "node-archive-summary-count-context", "Node v1645",
     "summary counts do not become mini-kv archive records"},
    {15, "node-archive-markdown-renderer-context", "Node v1646",
     "Markdown renderer extraction does not execute Node from mini-kv"},
    {16, "node-archive-renderer-reexport-context", "Node v1647",
     "stable Markdown renderer re-export is read-only sibling context"},
    {17, "node-archive-loader-orchestration-context", "Node v1648",
     "loader orchestration remains Node-owned and is not executed by mini-kv"},
    {18, "node-archive-renderer-regression-context", "Node v1649",
     "renderer re-export regression testing remains Node-owned"},
    {19, "node-archive-skipped-behavior-context", "Node v1650",
     "skipped behavior coverage does not become a mini-kv runtime branch"},
    {20, "node-archive-blocked-behavior-context", "Node v1651",
     "blocked behavior coverage does not open mini-kv execution"},
    {21, "node-archive-route-coverage-context", "Node v1652",
     "JSON and Markdown route coverage does not install a mini-kv route"},
    {22, "node-archive-adjacent-consumer-context", "Node v1653",
     "archive verification and release review consumers remain read-only sibling consumers"},
    {23, "node-archive-typecheck-build-smoke-context", "Node v1654",
     "typecheck build and HTTP smoke require no fresh mini-kv service startup"},
    {24, "node-archive-segmented-full-test-context", "Node v1655",
     "segmented full tests and cleanup validation remain Node-owned"},
    {25, "node-archive-closeout-context", "Node v1656",
     "closeout commits tags push and GitHub Actions are recorded without mini-kv participation"},
    {26, "mini-kv-command-catalog-parity", "mini-kv v1221",
     "command catalog declares read-only no-WAL stable evidence behavior"},
    {27, "mini-kv-fixture-export-parity", "mini-kv v1222",
     "versioned fixtures export the archive non-participation section without changing source evidence"},
    {28, "mini-kv-ci-build-stability-guard", "mini-kv v1223",
     "CI stability evidence stays separated from runtime authority"},
    {29, "mini-kv-cli-smoke-parity", "mini-kv v1224",
     "real CLI smoke reads evidence without services writes WAL or execution"},
    {30, "no-archive-non-participation-chain-created", "mini-kv v1225",
     "closeout opens no archive router, raw endpoint, runtime payload, write routing, WAL, or execution"},
}};

std::string format_check_json(const ProductionLiveCaptureArchiveNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"nonParticipationOnly\":true"
           ",\"sourceLiveCaptureNonParticipationFixtureFrozen\":true"
           ",\"nodeArchiveModuleSplitObserved\":true"
           ",\"nodeCaptureArchiveExecuted\":false"
           ",\"nodeArchiveLoaderExecuted\":false"
           ",\"nodeArchiveRendererExecuted\":false"
           ",\"nodeArchiveVerificationExecuted\":false"
           ",\"nodeReleaseReviewExecuted\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCaptureArchiveStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"archiveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"archivePayloadImported\":false"
           ",\"archivePayloadAccepted\":false"
           ",\"archiveFixtureImported\":false"
           ",\"archiveStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ProductionLiveCaptureArchiveNonParticipationCheck>
production_live_capture_archive_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_production_live_capture_archive_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_production_live_capture_archive_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published production live capture archive check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_production_live_capture_archive_non_participation_checks
