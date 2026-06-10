#include "minikv/shard_preview_catalog_entry_group_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<CatalogEntryGroupSplitNonParticipationCheck, 30> check_catalog = {{
    {1, "source-release-evidence-review-fixture-frozen", "fixtures/release/shard-readiness-v1295.json",
     "v1295 remains the immutable source release evidence review non-participation fixture"},
    {2, "node-catalog-types-module-context", "Node v1722",
     "catalog interfaces stay in Node TypeScript and are not compiled into mini-kv"},
    {3, "node-stable-entrypoint-constant-context", "Node v1723",
     "stable profile entrypoint constants are not imported by mini-kv"},
    {4, "node-foundation-live-window-group-context", "Node v1724",
     "foundation and live read-only window entries remain Node catalog data"},
    {5, "node-operator-evidence-supply-group-context", "Node v1725",
     "operator evidence supply entries remain Node catalog data"},
    {6, "node-signed-approval-capture-draft-group-context", "Node v1726",
     "signed approval capture draft entries do not grant mini-kv approval authority"},
    {7, "node-text-package-preflight-group-context", "Node v1727",
     "text package preflight entries are not imported into mini-kv"},
    {8, "node-text-package-evidence-group-context", "Node v1728",
     "text package evidence entries remain Node-owned"},
    {9, "node-candidate-profile-group-context", "Node v1729",
     "candidate document and profile section entries are not stored in mini-kv"},
    {10, "node-aggregate-registry-context", "Node v1730",
     "aggregate entry group registry is not executed by mini-kv"},
    {11, "node-stable-public-api-context", "Node v1731",
     "public catalog entrypoint remains Node API surface only"},
    {12, "node-defensive-copy-context", "Node v1732",
     "defensive copy behavior does not create mutable mini-kv state"},
    {13, "node-count-reexport-context", "Node v1733",
     "computed module counts and re-export counts stay Node-owned"},
    {14, "node-catalog-validation-context", "Node v1734",
     "catalog validation checks are not mini-kv validators"},
    {15, "node-contiguous-group-regression-context", "Node v1735",
     "contiguous group regression coverage remains Node test behavior"},
    {16, "node-entry-group-id-order-context", "Node v1736",
     "entry group id order remains Node maintenance metadata"},
    {17, "node-markdown-renderer-context", "Node v1737",
     "Markdown renderer output is not rendered by mini-kv"},
    {18, "node-controlled-preview-route-context", "Node v1738",
     "controlled read-only shard preview route tests do not install mini-kv routes"},
    {19, "node-review-artifact-barrel-context", "Node v1739",
     "review artifact barrel tests do not import Node artifacts into mini-kv"},
    {20, "node-typecheck-focused-catalog-context", "Node v1740",
     "typecheck and focused catalog tests require no fresh mini-kv evidence"},
    {21, "node-adjacent-route-artifact-context", "Node v1741",
     "adjacent route and artifact tests are not executed by mini-kv"},
    {22, "node-build-after-split-context", "Node v1742",
     "Node build after split does not start mini-kv services"},
    {23, "node-segmented-vitest-context", "Node v1743",
     "segmented Vitest does not trigger mini-kv execution"},
    {24, "node-no-fresh-java-evidence-context", "Node v1744",
     "Node requires no fresh Java evidence for this split"},
    {25, "node-no-fresh-mini-kv-evidence-context", "Node v1745",
     "Node requires no fresh mini-kv evidence for this split"},
    {26, "node-no-service-startup-context", "Node v1746",
     "Node records no sibling service startup requirement"},
    {27, "node-entry-group-ownership-doc-context", "Node v1747",
     "entry group ownership docs stay in Node"},
    {28, "node-future-growth-stop-context", "Node v1748",
     "future catalog growth stop condition does not alter mini-kv storage behavior"},
    {29, "node-catalog-size-guard-context", "Node v1749",
     "catalog giant-file guard is Node maintainability evidence only"},
    {30, "github-actions-closeout-summary", "Node v1750-v1756 / mini-kv v1360",
     "closeout keeps the follow-up chain read-only and opens no catalog endpoint import registry route write routing WAL or execution authority"},
}};

std::string format_check_json(const CatalogEntryGroupSplitNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"catalogEntryGroupSplitNonParticipationOnly\":true"
           ",\"sourceReleaseEvidenceReviewNonParticipationFixtureFrozen\":true"
           ",\"nodeCatalogEntryGroupSplitObserved\":true"
           ",\"nodeCatalogEntryRecordsImportedByMiniKv\":false"
           ",\"nodeCatalogRegistryExecutedByMiniKv\":false"
           ",\"nodeCatalogMarkdownRenderedByMiniKv\":false"
           ",\"nodeCatalogEndpointReadByMiniKv\":false"
           ",\"nodeRouteTestsExecutedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCatalogSplitStarted\":false"
           ",\"miniKvConsumesCatalogRecords\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"catalogEndpointRead\":false"
           ",\"catalogRecordsImported\":false"
           ",\"catalogRegistryExecuted\":false"
           ",\"catalogMarkdownRendered\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"catalogRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const CatalogEntryGroupSplitNonParticipationCheck> catalog_entry_group_split_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_catalog_entry_group_split_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_catalog_entry_group_split_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("catalog entry group split non-participation check count out of range");
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

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_checks
