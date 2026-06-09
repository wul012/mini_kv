#include "minikv/shard_preview_production_live_capture_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_live_capture_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1171;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1171", "route-preview-production-live-capture-non-participation-source-freeze", "v1170",
     "fixtures/release/shard-readiness-v1170.json",
     "freezes the v1170 integrity fixture before live capture non-participation evidence starts"},
    {2, "v1172", "route-preview-production-live-capture-non-participation-profile-type-contract", "v1171",
     "fixtures/release/shard-readiness-v1171.json",
     "records Node v1607 live capture profile type split as metadata only"},
    {3, "v1173", "route-preview-production-live-capture-non-participation-entrypoint-reexport-lock", "v1172",
     "fixtures/release/shard-readiness-v1172.json",
     "keeps Node v1608 stable entrypoint re-export outside mini-kv execution"},
    {4, "v1174", "route-preview-production-live-capture-non-participation-build-parts-boundary", "v1173",
     "fixtures/release/shard-readiness-v1173.json",
     "records Node v1609 build-parts split without importing builders"},
    {5, "v1175", "route-preview-production-live-capture-non-participation-record-conversion-boundary", "v1174",
     "fixtures/release/shard-readiness-v1174.json",
     "keeps Node v1610 captured-record conversion outside mini-kv"},
    {6, "v1176", "route-preview-production-live-capture-non-participation-readiness-check-boundary", "v1175",
     "fixtures/release/shard-readiness-v1175.json",
     "records Node v1611 readiness and smoke check assembly as metadata only"},
    {7, "v1177", "route-preview-production-live-capture-non-participation-ready-state-boundary", "v1176",
     "fixtures/release/shard-readiness-v1176.json",
     "keeps Node v1612 ready-for-live-capture state evaluation outside mini-kv"},
    {8, "v1178", "route-preview-production-live-capture-non-participation-pass-candidate-boundary", "v1177",
     "fixtures/release/shard-readiness-v1177.json",
     "preserves Node v1613 production-pass-candidate gating as no fresh mini-kv evidence"},
    {9, "v1179", "route-preview-production-live-capture-non-participation-capture-state-boundary", "v1178",
     "fixtures/release/shard-readiness-v1178.json",
     "records Node v1614 capture-state resolution without mini-kv live reads"},
    {10, "v1180", "route-preview-production-live-capture-non-participation-blocker-boundary", "v1179",
     "fixtures/release/shard-readiness-v1179.json",
     "keeps Node v1615 blocker collection outside mini-kv"},
    {11, "v1181", "route-preview-production-live-capture-non-participation-warning-boundary", "v1180",
     "fixtures/release/shard-readiness-v1180.json",
     "keeps Node v1616 warning generation outside mini-kv"},
    {12, "v1182", "route-preview-production-live-capture-non-participation-recommendation-boundary", "v1181",
     "fixtures/release/shard-readiness-v1181.json",
     "keeps Node v1617 recommendation generation outside mini-kv"},
    {13, "v1183", "route-preview-production-live-capture-non-participation-digest-input-lock", "v1182",
     "fixtures/release/shard-readiness-v1182.json",
     "records Node v1618 stable digest input shape without importing live capture input"},
    {14, "v1184", "route-preview-production-live-capture-non-participation-record-count-boundary", "v1183",
     "fixtures/release/shard-readiness-v1183.json",
     "keeps Node v1619 record-count summary assembly outside mini-kv"},
    {15, "v1185", "route-preview-production-live-capture-non-participation-endpoint-constant-lock", "v1184",
     "fixtures/release/shard-readiness-v1184.json",
     "records Node v1620 endpoint constants without reading or parsing raw endpoint values"},
    {16, "v1186", "route-preview-production-live-capture-non-participation-markdown-renderer-boundary", "v1185",
     "fixtures/release/shard-readiness-v1185.json",
     "keeps Node v1621 Markdown renderer extraction outside mini-kv"},
    {17, "v1187", "route-preview-production-live-capture-non-participation-record-markdown-boundary", "v1186",
     "fixtures/release/shard-readiness-v1186.json",
     "keeps Node v1622 record Markdown formatting private to Node"},
    {18, "v1188", "route-preview-production-live-capture-non-participation-route-markdown-compatibility", "v1187",
     "fixtures/release/shard-readiness-v1187.json",
     "records Node v1623 route Markdown compatibility as read-only context"},
    {19, "v1189", "route-preview-production-live-capture-non-participation-type-only-import-boundary", "v1188",
     "fixtures/release/shard-readiness-v1188.json",
     "records Node v1624 type-only imports without importing Node types"},
    {20, "v1190", "route-preview-production-live-capture-non-participation-entrypoint-size-control", "v1189",
     "fixtures/release/shard-readiness-v1189.json",
     "records Node v1625 entrypoint size control as maintenance context"},
    {21, "v1191", "route-preview-production-live-capture-non-participation-renderer-reexport-regression", "v1190",
     "fixtures/release/shard-readiness-v1190.json",
     "records Node v1626 renderer re-export regression test without executing Node"},
    {22, "v1192", "route-preview-production-live-capture-non-participation-focused-test-boundary", "v1191",
     "fixtures/release/shard-readiness-v1191.json",
     "records Node v1627 focused tests as sibling validation only"},
    {23, "v1193", "route-preview-production-live-capture-non-participation-typecheck-build-boundary", "v1192",
     "fixtures/release/shard-readiness-v1192.json",
     "records Node v1628 typecheck and build as no fresh mini-kv evidence"},
    {24, "v1194", "route-preview-production-live-capture-non-participation-adjacent-chain-boundary", "v1193",
     "fixtures/release/shard-readiness-v1193.json",
     "records Node v1629-v1630 adjacent chain checks and no sibling service requirement"},
    {25, "v1195", "route-preview-production-live-capture-non-participation-closeout-summary", "v1194",
     "fixtures/release/shard-readiness-v1194.json",
     "closes live capture non-participation without live reads endpoints services writes WAL or execution"},
}};

} // namespace

std::span<const StageRecord> production_live_capture_non_participation_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_production_live_capture_non_participation_release_number() {
    return first_release_number_value;
}

int planned_production_live_capture_non_participation_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_production_live_capture_non_participation_stages
