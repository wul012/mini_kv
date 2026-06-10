#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<TypeBarrelSplitFollowUpNonParticipationCheck, 20> check_catalog = {{
    {1, "source-type-barrel-split-fixture-freeze", "mini-kv v1385", "v1385 remains the frozen source fixture for the follow-up chain"},
    {2, "command-catalog-read-only-continuity", "mini-kv v1387", "the new follow-up command is read-only catalog metadata"},
    {3, "shardjson-current-rollover-boundary", "mini-kv v1388", "SHARDJSON current status rolls forward without changing storage behavior"},
    {4, "versioned-fixture-chain-continuity", "mini-kv v1389", "each follow-up release has a versioned fixture"},
    {5, "profile-endpoint-no-read", "Node v1827-v1829 context", "profile and evidence endpoints are not read by mini-kv"},
    {6, "node-type-import-guard", "Node v1822-v1830 context", "stable barrels profile types and aggregate re-exports are not imported"},
    {7, "route-renderer-no-execution", "Node v1836 context", "route and renderer tests are not executed by mini-kv"},
    {8, "node-validation-no-execution", "Node v1832/v1837/v1838 context", "typecheck build and Vitest remain Node-owned"},
    {9, "tcp-standalone-read-command-boundary", "mini-kv v1394", "TCP smoke reads only the standalone follow-up command"},
    {10, "large-payload-cli-only-boundary", "mini-kv v1395", "large SHARDJSON payload review stays CLI and fixture based"},
    {11, "rolling-current-test-hardening", "mini-kv v1396", "historical tests keep meaning while current status rolls forward"},
    {12, "source-digest-pin", "mini-kv v1397", "v1385 source digest is pinned as evidence context"},
    {13, "archive-screenshot-retention", "mini-kv v1398", "archive screenshots are retained without runtime archive walks"},
    {14, "walkthrough-index-continuity", "mini-kv v1399", "walkthrough index evidence remains documentation-only"},
    {15, "generated-validation-cleanup", "mini-kv v1400", "temporary validation output is cleaned without deleting retained evidence"},
    {16, "no-service-startup", "mini-kv v1401", "mini-kv Java and Node services are not started"},
    {17, "router-write-wal-guard", "mini-kv v1402", "active routers writes load restore compact and WAL touch stay disabled"},
    {18, "fixture-parity-current", "mini-kv v1403", "current fixture matches runtime SHARDJSON after the follow-up section"},
    {19, "whitespace-quality-gate", "mini-kv v1404", "documentation closeout keeps trailing whitespace out"},
    {20, "clean-workspace-ci-closeout", "mini-kv v1405", "closeout keeps Node imports endpoints routers writes WAL and execution disabled"},
}};

std::string format_check_json(const TypeBarrelSplitFollowUpNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"typeBarrelSplitFollowUpNonParticipationOnly\":true"
           ",\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true"
           ",\"nodeTypeBarrelSplitFollowUpObserved\":true"
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeAggregateReexportsImportedByMiniKv\":false"
           ",\"nodeNamedProfileBoundaryConsumedByMiniKv\":false"
           ",\"nodeProfileBoundaryEndpointReadByMiniKv\":false"
           ",\"nodeRouteRendererTestsExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvTypeBarrelFollowUpStarted\":false"
           ",\"miniKvConsumesNodeTypes\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const TypeBarrelSplitFollowUpNonParticipationCheck>
type_barrel_split_follow_up_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_type_barrel_split_follow_up_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_type_barrel_split_follow_up_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("type barrel split follow-up non-participation check count out of range");
    }

    std::string json = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            json += ",";
        }
        json += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_checks
