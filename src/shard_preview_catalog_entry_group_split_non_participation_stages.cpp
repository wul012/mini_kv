#include "minikv/shard_preview_catalog_entry_group_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1331;

constexpr std::array<StageRecord, 30> stage_records = {
{
    {1, "v1331", "route-preview-catalog-entry-group-split-non-participation-follow-up-type-module-catalog-types-boundary", "v1330", "fixtures/release/shard-readiness-v1330.json", "records Node v1722 type module catalog interfaces as Node-owned TypeScript types only"},
    {2, "v1332", "route-preview-catalog-entry-group-split-non-participation-follow-up-stable-profile-entrypoint-constant-boundary", "v1331", "fixtures/release/shard-readiness-v1331.json", "records Node v1723 public stable profile entrypoint constant without importing Node constants"},
    {3, "v1333", "route-preview-catalog-entry-group-split-non-participation-follow-up-foundation-live-window-entry-group-boundary", "v1332", "fixtures/release/shard-readiness-v1332.json", "keeps Node v1724 foundation and live read-only window entry group outside mini-kv runtime"},
    {4, "v1334", "route-preview-catalog-entry-group-split-non-participation-follow-up-operator-evidence-supply-entry-group-boundary", "v1333", "fixtures/release/shard-readiness-v1333.json", "keeps Node v1725 operator evidence supply entry group outside mini-kv"},
    {5, "v1335", "route-preview-catalog-entry-group-split-non-participation-follow-up-signed-approval-capture-draft-entry-group-boundary", "v1334", "fixtures/release/shard-readiness-v1334.json", "records Node v1726 signed approval capture draft entry group without opening approval execution"},
    {6, "v1336", "route-preview-catalog-entry-group-split-non-participation-follow-up-text-package-preflight-entry-group-boundary", "v1335", "fixtures/release/shard-readiness-v1335.json", "keeps Node v1727 text package preflight entries as Node catalog data"},
    {7, "v1337", "route-preview-catalog-entry-group-split-non-participation-follow-up-text-package-evidence-entry-group-boundary", "v1336", "fixtures/release/shard-readiness-v1336.json", "keeps Node v1728 text package evidence entries outside mini-kv"},
    {8, "v1338", "route-preview-catalog-entry-group-split-non-participation-follow-up-candidate-document-profile-section-entry-group-boundary", "v1337", "fixtures/release/shard-readiness-v1337.json", "records Node v1729 candidate document and profile section group without storing catalog records"},
    {9, "v1339", "route-preview-catalog-entry-group-split-non-participation-follow-up-aggregate-entry-group-registry-boundary", "v1338", "fixtures/release/shard-readiness-v1338.json", "keeps Node v1730 aggregate entry group registry from becoming a mini-kv registry"},
    {10, "v1340", "route-preview-catalog-entry-group-split-non-participation-follow-up-stable-public-catalog-api-boundary", "v1339", "fixtures/release/shard-readiness-v1339.json", "records Node v1731 stable public catalog entrypoint API while mini-kv keeps no API dependency"},
    {11, "v1341", "route-preview-catalog-entry-group-split-non-participation-follow-up-defensive-copy-behavior-boundary", "v1340", "fixtures/release/shard-readiness-v1340.json", "records Node v1732 defensive copy behavior without importing mutable catalog data"},
    {12, "v1342", "route-preview-catalog-entry-group-split-non-participation-follow-up-module-count-reexport-count-boundary", "v1341", "fixtures/release/shard-readiness-v1341.json", "records Node v1733 computed module counts and stable re-export counts as read-only plan context"},
    {13, "v1343", "route-preview-catalog-entry-group-split-non-participation-follow-up-catalog-validation-check-boundary", "v1342", "fixtures/release/shard-readiness-v1342.json", "keeps Node v1734 catalog validation checks inside Node"},
    {14, "v1344", "route-preview-catalog-entry-group-split-non-participation-follow-up-contiguous-entry-group-regression-boundary", "v1343", "fixtures/release/shard-readiness-v1343.json", "records Node v1735 contiguous split entry group regression coverage without executing Node tests"},
    {15, "v1345", "route-preview-catalog-entry-group-split-non-participation-follow-up-entry-group-id-order-boundary", "v1344", "fixtures/release/shard-readiness-v1344.json", "records Node v1736 entry group id order as maintenance metadata only"},
    {16, "v1346", "route-preview-catalog-entry-group-split-non-participation-follow-up-markdown-renderer-stability-boundary", "v1345", "fixtures/release/shard-readiness-v1345.json", "keeps Node v1737 Markdown renderer stability outside mini-kv rendering"},
    {17, "v1347", "route-preview-catalog-entry-group-split-non-participation-follow-up-controlled-preview-route-test-boundary", "v1346", "fixtures/release/shard-readiness-v1346.json", "records Node v1738 controlled read-only shard preview route tests without installing a route"},
    {18, "v1348", "route-preview-catalog-entry-group-split-non-participation-follow-up-review-artifact-barrel-test-boundary", "v1347", "fixtures/release/shard-readiness-v1347.json", "records Node v1739 review artifact barrel tests without importing review artifacts"},
    {19, "v1349", "route-preview-catalog-entry-group-split-non-participation-follow-up-typecheck-focused-catalog-test-boundary", "v1348", "fixtures/release/shard-readiness-v1348.json", "records Node v1740 typecheck and focused catalog tests without requiring fresh mini-kv evidence"},
    {20, "v1350", "route-preview-catalog-entry-group-split-non-participation-follow-up-adjacent-route-artifact-test-boundary", "v1349", "fixtures/release/shard-readiness-v1349.json", "keeps Node v1741 adjacent catalog route and review artifact tests Node-owned"},
    {21, "v1351", "route-preview-catalog-entry-group-split-non-participation-follow-up-build-after-split-boundary", "v1350", "fixtures/release/shard-readiness-v1350.json", "Node v1742 build after split does not start mini-kv services"},
    {22, "v1352", "route-preview-catalog-entry-group-split-non-participation-follow-up-segmented-vitest-boundary", "v1351", "fixtures/release/shard-readiness-v1351.json", "keeps Node v1743 segmented Vitest from triggering mini-kv execution"},
    {23, "v1353", "route-preview-catalog-entry-group-split-non-participation-follow-up-no-fresh-java-evidence-context", "v1352", "fixtures/release/shard-readiness-v1352.json", "Node v1744 requires no fresh Java evidence for this split"},
    {24, "v1354", "route-preview-catalog-entry-group-split-non-participation-follow-up-no-fresh-mini-kv-evidence-context", "v1353", "fixtures/release/shard-readiness-v1353.json", "Node v1745 requires no fresh mini-kv evidence for this split"},
    {25, "v1355", "route-preview-catalog-entry-group-split-non-participation-follow-up-no-service-startup-context", "v1354", "fixtures/release/shard-readiness-v1354.json", "Node v1746 records no sibling service startup requirement"},
    {26, "v1356", "route-preview-catalog-entry-group-split-non-participation-follow-up-entry-group-ownership-doc-context", "v1355", "fixtures/release/shard-readiness-v1355.json", "keeps entry group ownership docs in Node"},
    {27, "v1357", "route-preview-catalog-entry-group-split-non-participation-follow-up-future-growth-stop-context", "v1356", "fixtures/release/shard-readiness-v1356.json", "future catalog growth stop condition does not alter mini-kv storage behavior"},
    {28, "v1358", "route-preview-catalog-entry-group-split-non-participation-follow-up-catalog-size-guard-context", "v1357", "fixtures/release/shard-readiness-v1357.json", "catalog giant-file guard is Node maintainability evidence only"},
    {29, "v1359", "route-preview-catalog-entry-group-split-non-participation-follow-up-focused-entry-group-size-context", "v1358", "fixtures/release/shard-readiness-v1358.json", "focused entry group size remains Node maintenance evidence"},
    {30, "v1360", "route-preview-catalog-entry-group-split-non-participation-follow-up-github-actions-closeout-summary", "v1359", "fixtures/release/shard-readiness-v1359.json", "closes catalog entry group split non-participation follow-up without opening catalog endpoint import registry route write routing WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> catalog_entry_group_split_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_catalog_entry_group_split_non_participation_release_number() {
    return first_release_number;
}

int planned_catalog_entry_group_split_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages
