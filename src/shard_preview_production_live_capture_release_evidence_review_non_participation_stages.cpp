#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1261;

constexpr std::array<StageRecord, 35> stage_catalog = {{
    {1, "v1261", "route-preview-production-live-capture-release-evidence-review-non-participation-source-freeze",
     "v1260", "fixtures/release/shard-readiness-v1260.json",
     "freezes v1260 archive verification non-participation evidence before observing Node v1687 release evidence review profile, field-guide, and message contracts"},
    {2, "v1262", "route-preview-production-live-capture-release-evidence-review-non-participation-entrypoint-reexport-boundary",
     "v1261", "fixtures/release/shard-readiness-v1261.json",
     "records Node v1688 stable release review entrypoint type reexports without importing Node code"},
    {3, "v1263", "route-preview-production-live-capture-release-evidence-review-non-participation-build-parts-boundary",
     "v1262", "fixtures/release/shard-readiness-v1262.json",
     "keeps Node v1689 release review build-parts split outside mini-kv runtime"},
    {4, "v1264", "route-preview-production-live-capture-release-evidence-review-non-participation-java-field-guide-boundary",
     "v1263", "fixtures/release/shard-readiness-v1263.json",
     "records Node v1690 Java v51 field guide constants without making mini-kv a Java evidence authority"},
    {5, "v1265", "route-preview-production-live-capture-release-evidence-review-non-participation-mini-kv-field-guide-boundary",
     "v1264", "fixtures/release/shard-readiness-v1264.json",
     "records Node v1691 mini-kv v60 field guide constants as sibling metadata only"},
    {6, "v1266", "route-preview-production-live-capture-release-evidence-review-non-participation-endpoint-constant-boundary",
     "v1265", "fixtures/release/shard-readiness-v1265.json",
     "keeps Node v1692 release evidence review endpoint constants away from raw endpoint reads"},
    {7, "v1267", "route-preview-production-live-capture-release-evidence-review-non-participation-release-semantics-boundary",
     "v1266", "fixtures/release/shard-readiness-v1266.json",
     "records Node v1693 release semantics selection without accepting release state in mini-kv"},
    {8, "v1268", "route-preview-production-live-capture-release-evidence-review-non-participation-digest-generation-boundary",
     "v1267", "fixtures/release/shard-readiness-v1267.json",
     "keeps Node v1694 release review digest generation outside mini-kv"},
    {9, "v1269", "route-preview-production-live-capture-release-evidence-review-non-participation-check-assembly-boundary",
     "v1268", "fixtures/release/shard-readiness-v1268.json",
     "records Node v1695 release review check assembly without creating a mini-kv reviewer"},
    {10, "v1270", "route-preview-production-live-capture-release-evidence-review-non-participation-ready-state-boundary",
     "v1269", "fixtures/release/shard-readiness-v1269.json",
     "keeps Node v1696 ready-for-read-only-capture-release-evidence-review evaluation outside mini-kv"},
    {11, "v1271", "route-preview-production-live-capture-release-evidence-review-non-participation-review-state-boundary",
     "v1270", "fixtures/release/shard-readiness-v1270.json",
     "records Node v1697 review-state resolution without accepting review state in mini-kv"},
    {12, "v1272", "route-preview-production-live-capture-release-evidence-review-non-participation-field-guide-readiness-boundary",
     "v1271", "fixtures/release/shard-readiness-v1271.json",
     "keeps Node v1698 field guide readiness validation as Node-owned context"},
    {13, "v1273", "route-preview-production-live-capture-release-evidence-review-non-participation-non-pass-policy-boundary",
     "v1272", "fixtures/release/shard-readiness-v1272.json",
     "records Node v1699 non-pass-only field guide policy checks without changing mini-kv behavior"},
    {14, "v1274", "route-preview-production-live-capture-release-evidence-review-non-participation-skipped-mixed-policy-boundary",
     "v1273", "fixtures/release/shard-readiness-v1273.json",
     "keeps Node v1700 skipped or mixed capture non-pass policy checks outside mini-kv"},
    {15, "v1275", "route-preview-production-live-capture-release-evidence-review-non-participation-upstream-action-safety-boundary",
     "v1274", "fixtures/release/shard-readiness-v1274.json",
     "keeps Node v1701 upstream-action and automatic-start safety checks from starting mini-kv"},
    {16, "v1276", "route-preview-production-live-capture-release-evidence-review-non-participation-no-execution-authorization-boundary",
     "v1275", "fixtures/release/shard-readiness-v1275.json",
     "records Node v1702 no-execution-authorization review checks without adding execution authority"},
    {17, "v1277", "route-preview-production-live-capture-release-evidence-review-non-participation-blocker-boundary",
     "v1276", "fixtures/release/shard-readiness-v1276.json",
     "records Node v1703 production blocker collection without mini-kv participation"},
    {18, "v1278", "route-preview-production-live-capture-release-evidence-review-non-participation-warning-boundary",
     "v1277", "fixtures/release/shard-readiness-v1277.json",
     "keeps Node v1704 warning generation inside Node"},
    {19, "v1279", "route-preview-production-live-capture-release-evidence-review-non-participation-recommendation-boundary",
     "v1278", "fixtures/release/shard-readiness-v1278.json",
     "keeps Node v1705 recommendation generation inside Node"},
    {20, "v1280", "route-preview-production-live-capture-release-evidence-review-non-participation-summary-count-boundary",
     "v1279", "fixtures/release/shard-readiness-v1279.json",
     "records Node v1706 release review summary counts without importing review records"},
    {21, "v1281", "route-preview-production-live-capture-release-evidence-review-non-participation-markdown-renderer-boundary",
     "v1280", "fixtures/release/shard-readiness-v1280.json",
     "keeps Node v1707 release evidence review Markdown rendering outside mini-kv"},
    {22, "v1282", "route-preview-production-live-capture-release-evidence-review-non-participation-renderer-reexport-compatibility",
     "v1281", "fixtures/release/shard-readiness-v1281.json",
     "records Node v1708 stable Markdown renderer reexport as metadata only"},
    {23, "v1283", "route-preview-production-live-capture-release-evidence-review-non-participation-loader-orchestration-boundary",
     "v1282", "fixtures/release/shard-readiness-v1282.json",
     "keeps Node v1709 loader orchestration outside mini-kv execution"},
    {24, "v1284", "route-preview-production-live-capture-release-evidence-review-non-participation-renderer-regression-boundary",
     "v1283", "fixtures/release/shard-readiness-v1283.json",
     "records Node v1710 direct renderer reexport regression coverage without running Node from mini-kv"},
    {25, "v1285", "route-preview-production-live-capture-release-evidence-review-non-participation-field-guide-coverage-boundary",
     "v1284", "fixtures/release/shard-readiness-v1284.json",
     "keeps Node v1711 Java v51 and mini-kv v60 field-guide behavior coverage as read-only sibling context"},
    {26, "v1286", "route-preview-production-live-capture-release-evidence-review-non-participation-blocked-behavior-boundary",
     "v1285", "fixtures/release/shard-readiness-v1285.json",
     "keeps Node v1712 blocked release evidence review behavior from enabling writes or execution"},
    {27, "v1287", "route-preview-production-live-capture-release-evidence-review-non-participation-route-coverage-boundary",
     "v1286", "fixtures/release/shard-readiness-v1286.json",
     "records Node v1713 JSON and Markdown route coverage without installing a mini-kv route"},
    {28, "v1288", "route-preview-production-live-capture-release-evidence-review-non-participation-archive-verification-consumer-boundary",
     "v1287", "fixtures/release/shard-readiness-v1287.json",
     "keeps Node v1714 archive verification upstream consumer validation as read-only sibling context"},
    {29, "v1289", "route-preview-production-live-capture-release-evidence-review-non-participation-status-route-wiring-boundary",
     "v1288", "fixtures/release/shard-readiness-v1288.json",
     "records Node v1715 status live probe route wiring without opening mini-kv services"},
    {30, "v1290", "route-preview-production-live-capture-release-evidence-review-non-participation-typecheck-focused-boundary",
     "v1289", "fixtures/release/shard-readiness-v1289.json",
     "records Node v1716 typecheck and focused release evidence review tests without requiring fresh mini-kv evidence"},
    {31, "v1291", "route-preview-production-live-capture-release-evidence-review-non-participation-adjacent-build-boundary",
     "v1290", "fixtures/release/shard-readiness-v1290.json",
     "keeps Node v1717 adjacent archive verification and release review tests plus build outside mini-kv"},
    {32, "v1292", "route-preview-production-live-capture-release-evidence-review-non-participation-http-smoke-boundary",
     "v1291", "fixtures/release/shard-readiness-v1291.json",
     "records Node v1718 HTTP smoke for release evidence review JSON and Markdown without sibling startup"},
    {33, "v1293", "route-preview-production-live-capture-release-evidence-review-non-participation-segmented-full-vitest-boundary",
     "v1292", "fixtures/release/shard-readiness-v1292.json",
     "keeps Node v1719 segmented full Vitest verification from triggering mini-kv execution"},
    {34, "v1294", "route-preview-production-live-capture-release-evidence-review-non-participation-clean-output-boundary",
     "v1293", "fixtures/release/shard-readiness-v1293.json",
     "records Node v1720 generated validation cleanup and commit preparation without mini-kv file cleanup authority"},
    {35, "v1295", "route-preview-production-live-capture-release-evidence-review-non-participation-closeout-summary",
     "v1294", "fixtures/release/shard-readiness-v1294.json",
     "closes release evidence review non-participation with no release review endpoint read, payload import, router, write route, WAL touch, or execution authority"},
}};

} // namespace

std::span<const StageRecord> production_live_capture_release_evidence_review_non_participation_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_production_live_capture_release_evidence_review_non_participation_release_number() {
    return first_release_number_value;
}

int planned_production_live_capture_release_evidence_review_non_participation_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_stages
