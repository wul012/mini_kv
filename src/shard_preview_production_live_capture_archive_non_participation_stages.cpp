#include "minikv/shard_preview_production_live_capture_archive_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_live_capture_archive_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1196;

constexpr std::array<StageRecord, 30> stage_catalog = {{
    {1, "v1196", "route-preview-production-live-capture-archive-non-participation-source-freeze-profile-types",
     "v1195", "fixtures/release/shard-readiness-v1195.json",
     "freezes v1195 live capture non-participation evidence while recording Node v1632 profile and upstream reference types"},
    {2, "v1197", "route-preview-production-live-capture-archive-non-participation-entrypoint-reexport-lock",
     "v1196", "fixtures/release/shard-readiness-v1196.json",
     "records Node v1633 stable entrypoint type re-exports as metadata only"},
    {3, "v1198", "route-preview-production-live-capture-archive-non-participation-reference-constant-boundary",
     "v1197", "fixtures/release/shard-readiness-v1197.json",
     "keeps Node v1634 Java v50 and mini-kv v59 archive reference constants outside mini-kv execution"},
    {4, "v1199", "route-preview-production-live-capture-archive-non-participation-endpoint-constant-boundary",
     "v1198", "fixtures/release/shard-readiness-v1198.json",
     "records Node v1635 capture archive endpoint constants without reading raw endpoint values"},
    {5, "v1200", "route-preview-production-live-capture-archive-non-participation-build-parts-boundary",
     "v1199", "fixtures/release/shard-readiness-v1199.json",
     "keeps Node v1636 archive build-parts split outside mini-kv"},
    {6, "v1201", "route-preview-production-live-capture-archive-non-participation-production-pass-boundary",
     "v1200", "fixtures/release/shard-readiness-v1200.json",
     "records Node v1637 archived-as-production-pass evaluation as sibling metadata only"},
    {7, "v1202", "route-preview-production-live-capture-archive-non-participation-check-assembly-boundary",
     "v1201", "fixtures/release/shard-readiness-v1201.json",
     "keeps Node v1638 capture archive check assembly outside mini-kv"},
    {8, "v1203", "route-preview-production-live-capture-archive-non-participation-readiness-state-boundary",
     "v1202", "fixtures/release/shard-readiness-v1202.json",
     "records Node v1639 ready-for-read-only-capture-archive evaluation without mini-kv service startup"},
    {9, "v1204", "route-preview-production-live-capture-archive-non-participation-archive-state-boundary",
     "v1203", "fixtures/release/shard-readiness-v1203.json",
     "keeps Node v1640 archive-state resolution outside mini-kv"},
    {10, "v1205", "route-preview-production-live-capture-archive-non-participation-digest-input-lock",
     "v1204", "fixtures/release/shard-readiness-v1204.json",
     "records Node v1641 digest generation over stable input without importing archive payloads"},
    {11, "v1206", "route-preview-production-live-capture-archive-non-participation-blocker-boundary",
     "v1205", "fixtures/release/shard-readiness-v1205.json",
     "keeps Node v1642 blocker collection outside mini-kv"},
    {12, "v1207", "route-preview-production-live-capture-archive-non-participation-warning-boundary",
     "v1206", "fixtures/release/shard-readiness-v1206.json",
     "keeps Node v1643 warning generation outside mini-kv"},
    {13, "v1208", "route-preview-production-live-capture-archive-non-participation-recommendation-boundary",
     "v1207", "fixtures/release/shard-readiness-v1207.json",
     "keeps Node v1644 recommendation generation outside mini-kv"},
    {14, "v1209", "route-preview-production-live-capture-archive-non-participation-summary-count-boundary",
     "v1208", "fixtures/release/shard-readiness-v1208.json",
     "records Node v1645 summary counts without importing archive records"},
    {15, "v1210", "route-preview-production-live-capture-archive-non-participation-markdown-renderer-boundary",
     "v1209", "fixtures/release/shard-readiness-v1209.json",
     "keeps Node v1646 Markdown renderer extraction outside mini-kv"},
    {16, "v1211", "route-preview-production-live-capture-archive-non-participation-renderer-reexport-compatibility",
     "v1210", "fixtures/release/shard-readiness-v1210.json",
     "records Node v1647 stable Markdown renderer re-export as read-only sibling context"},
    {17, "v1212", "route-preview-production-live-capture-archive-non-participation-loader-orchestration-boundary",
     "v1211", "fixtures/release/shard-readiness-v1211.json",
     "keeps Node v1648 loader orchestration outside mini-kv execution"},
    {18, "v1213", "route-preview-production-live-capture-archive-non-participation-renderer-regression-boundary",
     "v1212", "fixtures/release/shard-readiness-v1212.json",
     "records Node v1649 renderer re-export regression testing as sibling validation only"},
    {19, "v1214", "route-preview-production-live-capture-archive-non-participation-skipped-behavior-boundary",
     "v1213", "fixtures/release/shard-readiness-v1213.json",
     "keeps Node v1650 skipped behavior coverage outside mini-kv"},
    {20, "v1215", "route-preview-production-live-capture-archive-non-participation-blocked-behavior-boundary",
     "v1214", "fixtures/release/shard-readiness-v1214.json",
     "keeps Node v1651 blocked behavior coverage outside mini-kv"},
    {21, "v1216", "route-preview-production-live-capture-archive-non-participation-route-coverage-boundary",
     "v1215", "fixtures/release/shard-readiness-v1215.json",
     "records Node v1652 JSON and Markdown route coverage without opening mini-kv routes"},
    {22, "v1217", "route-preview-production-live-capture-archive-non-participation-adjacent-consumer-boundary",
     "v1216", "fixtures/release/shard-readiness-v1216.json",
     "keeps Node v1653 adjacent archive verification and release review consumers read-only"},
    {23, "v1218", "route-preview-production-live-capture-archive-non-participation-typecheck-build-smoke-boundary",
     "v1217", "fixtures/release/shard-readiness-v1217.json",
     "records Node v1654 typecheck focused adjacent build and HTTP smoke as no fresh mini-kv evidence"},
    {24, "v1219", "route-preview-production-live-capture-archive-non-participation-segmented-full-test-boundary",
     "v1218", "fixtures/release/shard-readiness-v1218.json",
     "keeps Node v1655 segmented full tests and cleanup validation outside mini-kv"},
    {25, "v1220", "route-preview-production-live-capture-archive-non-participation-node-closeout-boundary",
     "v1219", "fixtures/release/shard-readiness-v1219.json",
     "records Node v1656 closeout commits tags push and GitHub Actions as read-only sibling context"},
    {26, "v1221", "route-preview-production-live-capture-archive-non-participation-command-catalog-parity",
     "v1220", "fixtures/release/shard-readiness-v1220.json",
     "hardens the mini-kv command catalog entry for archive non-participation evidence"},
    {27, "v1222", "route-preview-production-live-capture-archive-non-participation-fixture-export-parity",
     "v1221", "fixtures/release/shard-readiness-v1221.json",
     "hardens fixture export parity for v1196-v1225 versioned release evidence"},
    {28, "v1223", "route-preview-production-live-capture-archive-non-participation-ci-build-stability-guard",
     "v1222", "fixtures/release/shard-readiness-v1222.json",
     "keeps the macOS verbose build stability fix visible in the archive evidence chain"},
    {29, "v1224", "route-preview-production-live-capture-archive-non-participation-smoke-parity",
     "v1223", "fixtures/release/shard-readiness-v1223.json",
     "proves real CLI smoke can read archive non-participation evidence without services writes WAL or execution"},
    {30, "v1225", "route-preview-production-live-capture-archive-non-participation-closeout-summary",
     "v1224", "fixtures/release/shard-readiness-v1224.json",
     "closes archive non-participation with no router write route raw endpoint archive payload or execution authority"},
}};

} // namespace

std::span<const StageRecord> production_live_capture_archive_non_participation_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_production_live_capture_archive_non_participation_release_number() {
    return first_release_number_value;
}

int planned_production_live_capture_archive_non_participation_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_production_live_capture_archive_non_participation_stages
