#include "minikv/shard_preview_type_barrel_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_type_barrel_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1361;

constexpr std::array<StageRecord, 25> stage_records = {{
    {1, "v1361", "route-preview-type-barrel-split-non-participation-stable-barrel-boundary", "v1360", "fixtures/release/shard-readiness-v1360.json", "records Node v1822 stable type-barrel split boundary without importing TypeScript symbols"},
    {2, "v1362", "route-preview-type-barrel-split-non-participation-aggregate-reexport-module-boundary", "v1361", "fixtures/release/shard-readiness-v1361.json", "keeps Node v1823 aggregate downstream type re-exports in Node"},
    {3, "v1363", "route-preview-type-barrel-split-non-participation-profile-only-module-boundary", "v1362", "fixtures/release/shard-readiness-v1362.json", "records Node v1824 profile-only module without storing profile types"},
    {4, "v1364", "route-preview-type-barrel-split-non-participation-compatibility-barrel-boundary", "v1363", "fixtures/release/shard-readiness-v1363.json", "keeps Node v1825 legacy entrypoint compatibility as a Node import surface only"},
    {5, "v1365", "route-preview-type-barrel-split-non-participation-type-regression-coverage-boundary", "v1364", "fixtures/release/shard-readiness-v1364.json", "records Node v1826 type-level regression coverage without executing Node tests"},
    {6, "v1366", "route-preview-type-barrel-split-non-participation-reads-profile-boundary", "v1365", "fixtures/release/shard-readiness-v1365.json", "keeps Node v1827 reads profile boundary outside mini-kv runtime"},
    {7, "v1367", "route-preview-type-barrel-split-non-participation-preview-graph-profile-boundary", "v1366", "fixtures/release/shard-readiness-v1366.json", "keeps Node v1828 preview graph profile boundary outside mini-kv"},
    {8, "v1368", "route-preview-type-barrel-split-non-participation-evidence-endpoint-profile-boundary", "v1367", "fixtures/release/shard-readiness-v1367.json", "records Node v1829 evidence endpoint profile boundary without reading endpoints"},
    {9, "v1369", "route-preview-type-barrel-split-non-participation-narrow-boundary-export-surface", "v1368", "fixtures/release/shard-readiness-v1368.json", "records Node v1830 narrow boundaries exposed through the stable barrel"},
    {10, "v1370", "route-preview-type-barrel-split-non-participation-focused-coverage-boundary", "v1369", "fixtures/release/shard-readiness-v1369.json", "keeps Node v1831 focused type-barrel split coverage Node-owned"},
    {11, "v1371", "route-preview-type-barrel-split-non-participation-typecheck-boundary", "v1370", "fixtures/release/shard-readiness-v1370.json", "records Node v1832 typecheck across profile imports without mini-kv execution"},
    {12, "v1372", "route-preview-type-barrel-split-non-participation-java-parallel-no-fresh-evidence-boundary", "v1371", "fixtures/release/shard-readiness-v1371.json", "records Node v1833 Java parallel review without consuming new Java evidence"},
    {13, "v1373", "route-preview-type-barrel-split-non-participation-mini-kv-parallel-no-fresh-evidence-boundary", "v1372", "fixtures/release/shard-readiness-v1372.json", "records Node v1834 mini-kv parallel review without requiring new mini-kv evidence"},
    {14, "v1374", "route-preview-type-barrel-split-non-participation-giant-aggregator-growth-guard", "v1373", "fixtures/release/shard-readiness-v1373.json", "records Node v1835 guard against growing another giant type aggregator"},
    {15, "v1375", "route-preview-type-barrel-split-non-participation-route-renderer-test-boundary", "v1374", "fixtures/release/shard-readiness-v1374.json", "keeps Node v1836 route and renderer related tests out of mini-kv"},
    {16, "v1376", "route-preview-type-barrel-split-non-participation-build-after-split-boundary", "v1375", "fixtures/release/shard-readiness-v1375.json", "records Node v1837 build after profile-boundary split without service startup"},
    {17, "v1377", "route-preview-type-barrel-split-non-participation-segmented-vitest-boundary", "v1376", "fixtures/release/shard-readiness-v1376.json", "keeps Node v1838 segmented Vitest coverage outside mini-kv"},
    {18, "v1378", "route-preview-type-barrel-split-non-participation-explanation-archive-boundary", "v1377", "fixtures/release/shard-readiness-v1377.json", "records Node v1839 explanation archive as documentation-only context"},
    {19, "v1379", "route-preview-type-barrel-split-non-participation-code-walkthrough-archive-boundary", "v1378", "fixtures/release/shard-readiness-v1378.json", "records Node v1840 code walkthrough archive without importing docs"},
    {20, "v1380", "route-preview-type-barrel-split-non-participation-validation-output-cleanup-boundary", "v1379", "fixtures/release/shard-readiness-v1379.json", "records Node v1841 generated validation cleanup without deleting mini-kv files"},
    {21, "v1381", "route-preview-type-barrel-split-non-participation-diff-line-ownership-inspection", "v1380", "fixtures/release/shard-readiness-v1380.json", "records Node v1842 diff size line count and ownership review"},
    {22, "v1382", "route-preview-type-barrel-split-non-participation-commit-batch-boundary", "v1381", "fixtures/release/shard-readiness-v1381.json", "records Node v1843 commit boundary without mini-kv committing Node work"},
    {23, "v1383", "route-preview-type-barrel-split-non-participation-tag-range-boundary", "v1382", "fixtures/release/shard-readiness-v1382.json", "records Node v1844 tag range boundary as read-only context"},
    {24, "v1384", "route-preview-type-barrel-split-non-participation-push-ci-polling-boundary", "v1383", "fixtures/release/shard-readiness-v1383.json", "records Node v1845 push and CI polling without polling from mini-kv"},
    {25, "v1385", "route-preview-type-barrel-split-non-participation-clean-workspace-ci-closeout", "v1384", "fixtures/release/shard-readiness-v1384.json", "closes Node v1846 type-barrel split context with no Node type import endpoint read router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> type_barrel_split_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_type_barrel_split_non_participation_release_number() {
    return first_release_number;
}

int planned_type_barrel_split_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_type_barrel_split_non_participation_stages
