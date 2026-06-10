#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1226;

constexpr std::array<StageRecord, 35> stage_catalog = {{
    {1, "v1226", "route-preview-production-live-capture-archive-verification-non-participation-source-freeze",
     "v1225", "fixtures/release/shard-readiness-v1225.json",
     "freezes v1225 archive non-participation evidence before observing Node v1657 verification profile and message contracts"},
    {2, "v1227", "route-preview-production-live-capture-archive-verification-non-participation-entrypoint-reexport-boundary",
     "v1226", "fixtures/release/shard-readiness-v1226.json",
     "records Node v1658 stable verification entrypoint reexports without importing Node code"},
    {3, "v1228", "route-preview-production-live-capture-archive-verification-non-participation-build-parts-boundary",
     "v1227", "fixtures/release/shard-readiness-v1227.json",
     "keeps Node v1659 verification build-parts split outside mini-kv runtime"},
    {4, "v1229", "route-preview-production-live-capture-archive-verification-non-participation-expected-digest-boundary",
     "v1228", "fixtures/release/shard-readiness-v1228.json",
     "records Node v1660 expected capture archive digest recomputation as sibling verification metadata only"},
    {5, "v1230", "route-preview-production-live-capture-archive-verification-non-participation-generated-digest-boundary",
     "v1229", "fixtures/release/shard-readiness-v1229.json",
     "keeps Node v1661 verification digest generation outside mini-kv"},
    {6, "v1231", "route-preview-production-live-capture-archive-verification-non-participation-reference-constant-boundary",
     "v1230", "fixtures/release/shard-readiness-v1230.json",
     "records Node v1662 Java v50 and mini-kv v59 reference constants without making mini-kv authoritative"},
    {7, "v1232", "route-preview-production-live-capture-archive-verification-non-participation-endpoint-constant-boundary",
     "v1231", "fixtures/release/shard-readiness-v1231.json",
     "keeps Node v1663 verification evidence endpoint constants away from raw endpoint reads"},
    {8, "v1233", "route-preview-production-live-capture-archive-verification-non-participation-check-assembly-boundary",
     "v1232", "fixtures/release/shard-readiness-v1232.json",
     "records Node v1664 verification check assembly without creating a mini-kv verifier"},
    {9, "v1234", "route-preview-production-live-capture-archive-verification-non-participation-ready-state-boundary",
     "v1233", "fixtures/release/shard-readiness-v1233.json",
     "keeps Node v1665 ready-for-read-only-capture-archive-verification evaluation outside mini-kv"},
    {10, "v1235", "route-preview-production-live-capture-archive-verification-non-participation-state-resolution-boundary",
     "v1234", "fixtures/release/shard-readiness-v1234.json",
     "records Node v1666 verification-state resolution without accepting verification state in mini-kv"},
    {11, "v1236", "route-preview-production-live-capture-archive-verification-non-participation-digest-match-boundary",
     "v1235", "fixtures/release/shard-readiness-v1235.json",
     "keeps Node v1667 archive digest match validation outside mini-kv"},
    {12, "v1237", "route-preview-production-live-capture-archive-verification-non-participation-profile-readiness-boundary",
     "v1236", "fixtures/release/shard-readiness-v1236.json",
     "records Node v1668 archive profile version and archive readiness checks without importing archive profiles"},
    {13, "v1238", "route-preview-production-live-capture-archive-verification-non-participation-upstream-reference-boundary",
     "v1237", "fixtures/release/shard-readiness-v1237.json",
     "keeps Node v1669 Java and mini-kv upstream reference readiness checks as read-only sibling context"},
    {14, "v1239", "route-preview-production-live-capture-archive-verification-non-participation-skipped-mixed-policy-boundary",
     "v1238", "fixtures/release/shard-readiness-v1238.json",
     "records Node v1670 skipped and mixed capture non-pass policy checks without changing mini-kv behavior"},
    {15, "v1240", "route-preview-production-live-capture-archive-verification-non-participation-upstream-action-safety-boundary",
     "v1239", "fixtures/release/shard-readiness-v1239.json",
     "keeps Node v1671 upstream-action and automatic-start safety checks from starting mini-kv"},
    {16, "v1241", "route-preview-production-live-capture-archive-verification-non-participation-blocker-boundary",
     "v1240", "fixtures/release/shard-readiness-v1240.json",
     "records Node v1672 production blocker collection without mini-kv participation"},
    {17, "v1242", "route-preview-production-live-capture-archive-verification-non-participation-warning-boundary",
     "v1241", "fixtures/release/shard-readiness-v1241.json",
     "keeps Node v1673 warning generation inside Node"},
    {18, "v1243", "route-preview-production-live-capture-archive-verification-non-participation-recommendation-boundary",
     "v1242", "fixtures/release/shard-readiness-v1242.json",
     "keeps Node v1674 recommendation generation inside Node"},
    {19, "v1244", "route-preview-production-live-capture-archive-verification-non-participation-summary-count-boundary",
     "v1243", "fixtures/release/shard-readiness-v1243.json",
     "records Node v1675 verification summary counts without importing verification records"},
    {20, "v1245", "route-preview-production-live-capture-archive-verification-non-participation-markdown-renderer-boundary",
     "v1244", "fixtures/release/shard-readiness-v1244.json",
     "keeps Node v1676 verification Markdown renderer outside mini-kv"},
    {21, "v1246", "route-preview-production-live-capture-archive-verification-non-participation-renderer-reexport-compatibility",
     "v1245", "fixtures/release/shard-readiness-v1245.json",
     "records Node v1677 stable Markdown renderer reexport as metadata only"},
    {22, "v1247", "route-preview-production-live-capture-archive-verification-non-participation-loader-orchestration-boundary",
     "v1246", "fixtures/release/shard-readiness-v1246.json",
     "keeps Node v1678 loader orchestration outside mini-kv execution"},
    {23, "v1248", "route-preview-production-live-capture-archive-verification-non-participation-renderer-regression-boundary",
     "v1247", "fixtures/release/shard-readiness-v1247.json",
     "records Node v1679 renderer reexport regression coverage without running Node from mini-kv"},
    {24, "v1249", "route-preview-production-live-capture-archive-verification-non-participation-skipped-behavior-boundary",
     "v1248", "fixtures/release/shard-readiness-v1248.json",
     "keeps Node v1680 skipped behavior coverage from opening mini-kv routes"},
    {25, "v1250", "route-preview-production-live-capture-archive-verification-non-participation-blocked-behavior-boundary",
     "v1249", "fixtures/release/shard-readiness-v1249.json",
     "keeps Node v1681 blocked behavior coverage from enabling writes or execution"},
    {26, "v1251", "route-preview-production-live-capture-archive-verification-non-participation-route-coverage-boundary",
     "v1250", "fixtures/release/shard-readiness-v1250.json",
     "records Node v1682 JSON and Markdown route coverage without installing a mini-kv route"},
    {27, "v1252", "route-preview-production-live-capture-archive-verification-non-participation-release-review-consumer-boundary",
     "v1251", "fixtures/release/shard-readiness-v1251.json",
     "keeps Node v1683 release evidence review consumer validation as read-only sibling context"},
    {28, "v1253", "route-preview-production-live-capture-archive-verification-non-participation-typecheck-build-boundary",
     "v1252", "fixtures/release/shard-readiness-v1252.json",
     "records Node v1684 typecheck focused adjacent and build validation without requiring fresh mini-kv evidence"},
    {29, "v1254", "route-preview-production-live-capture-archive-verification-non-participation-http-smoke-cleanup-boundary",
     "v1253", "fixtures/release/shard-readiness-v1253.json",
     "keeps Node v1685 HTTP smoke segmented full tests and cleanup outside mini-kv"},
    {30, "v1255", "route-preview-production-live-capture-archive-verification-non-participation-node-closeout-boundary",
     "v1254", "fixtures/release/shard-readiness-v1254.json",
     "records Node v1686 closeout commits tags push and GitHub Actions without mini-kv service startup"},
    {31, "v1256", "route-preview-production-live-capture-archive-verification-non-participation-command-catalog-parity",
     "v1255", "fixtures/release/shard-readiness-v1255.json",
     "hardens mini-kv command catalog parity for archive verification non-participation evidence"},
    {32, "v1257", "route-preview-production-live-capture-archive-verification-non-participation-fixture-export-parity",
     "v1256", "fixtures/release/shard-readiness-v1256.json",
     "hardens fixture export parity for v1226-v1260 archive verification non-participation evidence"},
    {33, "v1258", "route-preview-production-live-capture-archive-verification-non-participation-smoke-parity",
     "v1257", "fixtures/release/shard-readiness-v1257.json",
     "proves CLI and TCP smoke can read the evidence without writes WAL touch service orchestration or execution authority"},
    {34, "v1259", "route-preview-production-live-capture-archive-verification-non-participation-archive-explanation-parity",
     "v1258", "fixtures/release/shard-readiness-v1258.json",
     "keeps command screenshot explanations aligned with the no router no write no execution contract"},
    {35, "v1260", "route-preview-production-live-capture-archive-verification-non-participation-closeout-summary",
     "v1259", "fixtures/release/shard-readiness-v1259.json",
     "closes archive verification non-participation with no verification endpoint read, payload import, router, write route, WAL touch, or execution authority"},
}};

} // namespace

std::span<const StageRecord> production_live_capture_archive_verification_non_participation_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_production_live_capture_archive_verification_non_participation_release_number() {
    return first_release_number_value;
}

int planned_production_live_capture_archive_verification_non_participation_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_stages
