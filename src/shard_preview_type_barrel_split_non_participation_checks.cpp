#include "minikv/shard_preview_type_barrel_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_type_barrel_split_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<TypeBarrelSplitNonParticipationCheck, 25> check_catalog = {{
    {1, "node-stable-type-barrel-boundary", "Node v1822", "stable barrel remains Node TypeScript surface only"},
    {2, "node-aggregate-reexport-module-context", "Node v1823", "aggregate downstream type re-exports are not imported by mini-kv"},
    {3, "node-profile-only-module-context", "Node v1824", "ControlledReadOnlyShardPreviewProfile stays in Node types"},
    {4, "node-legacy-compatibility-barrel-context", "Node v1825", "legacy type entrypoint compatibility grants no mini-kv API"},
    {5, "node-type-level-regression-context", "Node v1826", "type-level regression tests are not executed by mini-kv"},
    {6, "node-reads-profile-boundary-context", "Node v1827", "reads profile boundary does not become a mini-kv reader"},
    {7, "node-preview-graph-profile-boundary-context", "Node v1828", "preview graph profile boundary does not install routes"},
    {8, "node-evidence-endpoint-profile-boundary-context", "Node v1829", "evidence endpoint profile boundary does not read raw endpoints"},
    {9, "node-narrow-profile-boundary-export-context", "Node v1830", "narrow profile boundary exports remain Node-owned"},
    {10, "node-focused-type-barrel-coverage-context", "Node v1831", "focused split coverage remains Node test behavior"},
    {11, "node-profile-import-typecheck-context", "Node v1832", "typecheck does not require mini-kv evidence refresh"},
    {12, "node-java-parallel-no-fresh-evidence-context", "Node v1833", "Java can continue without fresh evidence consumption by this batch"},
    {13, "node-mini-kv-parallel-no-fresh-evidence-context", "Node v1834", "mini-kv can continue independently without being a Node prerequisite"},
    {14, "node-giant-type-aggregator-guard-context", "Node v1835", "giant type aggregator prevention is a Node maintainability guard"},
    {15, "node-route-renderer-related-test-context", "Node v1836", "route and renderer tests are not executed by mini-kv"},
    {16, "node-build-after-profile-boundary-context", "Node v1837", "Node build after split starts no mini-kv service"},
    {17, "node-segmented-vitest-context", "Node v1838", "segmented Vitest coverage stays Node-owned"},
    {18, "node-explanation-archive-context", "Node v1839", "explanation archive is documentation context only"},
    {19, "node-code-walkthrough-archive-context", "Node v1840", "code walkthrough archive is not imported into mini-kv"},
    {20, "node-generated-validation-cleanup-context", "Node v1841", "generated validation cleanup does not delete mini-kv artifacts"},
    {21, "node-diff-line-ownership-inspection-context", "Node v1842", "line-count and ownership inspection stays Node maintainability evidence"},
    {22, "node-commit-batch-context", "Node v1843", "Node commit boundary is read-only context for mini-kv"},
    {23, "node-tag-range-context", "Node v1844", "Node tag range is not created or consumed by mini-kv"},
    {24, "node-push-ci-polling-context", "Node v1845", "Node push and CI polling are not performed by mini-kv"},
    {25, "node-clean-workspace-ci-closeout", "Node v1846 / mini-kv v1385", "closeout keeps type imports endpoints routers writes WAL and execution disabled"},
}};

std::string format_check_json(const TypeBarrelSplitNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"typeBarrelSplitNonParticipationOnly\":true"
           ",\"sourceCatalogEntryGroupSplitNonParticipationFixtureFrozen\":true"
           ",\"nodeTypeBarrelSplitObserved\":true"
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
           ",\"miniKvTypeBarrelSplitStarted\":false"
           ",\"miniKvConsumesNodeTypes\":false"
           ",\"miniKvConsumesProfileBoundaries\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"evidenceEndpointRead\":false"
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

std::span<const TypeBarrelSplitNonParticipationCheck> type_barrel_split_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_type_barrel_split_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_type_barrel_split_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("type barrel split non-participation check count out of range");
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

} // namespace minikv::shard_preview_type_barrel_split_non_participation_checks
