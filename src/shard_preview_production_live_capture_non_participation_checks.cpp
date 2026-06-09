#include "minikv/shard_preview_production_live_capture_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_live_capture_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ProductionLiveCaptureNonParticipationCheck, 25> check_catalog = {{
    {1, "source-integrity-fixture-frozen", "fixtures/release/shard-readiness-v1170.json",
     "v1170 remains the immutable source integrity fixture"},
    {2, "node-live-capture-profile-type-context", "Node v1607",
     "live capture profile types remain Node-owned metadata"},
    {3, "node-stable-entrypoint-context", "Node v1608",
     "stable entrypoint re-export does not become a mini-kv command"},
    {4, "node-build-parts-context", "Node v1609",
     "live capture build parts are not imported into mini-kv"},
    {5, "node-record-conversion-context", "Node v1610",
     "captured-record conversion does not run in mini-kv"},
    {6, "node-readiness-check-context", "Node v1611",
     "readiness and smoke checks do not start mini-kv services"},
    {7, "node-ready-state-context", "Node v1612",
     "ready-for-live-capture state is not evaluated by mini-kv"},
    {8, "node-pass-candidate-context", "Node v1613",
     "production pass candidate gating requires no fresh mini-kv evidence"},
    {9, "node-capture-state-context", "Node v1614",
     "capture-state resolution does not trigger mini-kv live reads"},
    {10, "node-blocker-context", "Node v1615",
     "blocker collection stays inside Node"},
    {11, "node-warning-context", "Node v1616",
     "warning generation stays inside Node"},
    {12, "node-recommendation-context", "Node v1617",
     "recommendation generation stays inside Node"},
    {13, "node-digest-input-context", "Node v1618",
     "stable digest input shape is not imported as runtime payload"},
    {14, "node-record-count-context", "Node v1619",
     "record-count summary assembly stays outside mini-kv"},
    {15, "node-endpoint-constant-context", "Node v1620",
     "endpoint constants are not raw endpoint values read by mini-kv"},
    {16, "node-markdown-renderer-context", "Node v1621",
     "Markdown renderer extraction does not execute Node from mini-kv"},
    {17, "node-record-markdown-context", "Node v1622",
     "record Markdown formatting remains private to Node"},
    {18, "node-route-markdown-compatibility-context", "Node v1623",
     "route Markdown compatibility is read-only sibling context"},
    {19, "node-type-only-import-context", "Node v1624",
     "type-only imports are not imported into mini-kv"},
    {20, "node-entrypoint-size-context", "Node v1625",
     "entrypoint size control is maintenance context only"},
    {21, "node-renderer-reexport-test-context", "Node v1626",
     "renderer re-export regression testing remains Node-owned"},
    {22, "node-focused-test-context", "Node v1627",
     "focused live capture tests do not become mini-kv tests"},
    {23, "node-typecheck-build-context", "Node v1628",
     "Node typecheck/build require no mini-kv startup"},
    {24, "node-adjacent-chain-context", "Node v1629-v1630",
     "adjacent read-only window checks require no sibling services"},
    {25, "no-live-capture-chain-created", "Node v1631",
     "closeout does not open live capture, endpoint reads, services, writes, WAL, or execution"},
}};

std::string format_check_json(const ProductionLiveCaptureNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"nonParticipationOnly\":true"
           ",\"sourceIntegrityFixtureFrozen\":true"
           ",\"nodeModuleSplitObserved\":true"
           ",\"nodeLiveCaptureExecuted\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvLiveCaptureStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"liveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ProductionLiveCaptureNonParticipationCheck>
production_live_capture_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_production_live_capture_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_production_live_capture_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{"published production live capture check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_production_live_capture_non_participation_checks
