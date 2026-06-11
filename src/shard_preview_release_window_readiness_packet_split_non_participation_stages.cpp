#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1476;

constexpr std::array<StageRecord, 15> stage_records = {{
    {1, "v1476", "route-preview-release-window-readiness-packet-split-non-participation-source-freeze", "v1475", "fixtures/release/shard-readiness-v1475.json", "freezes the v1475 implementation-plan upstream echo closeout non-participation fixture as source"},
    {2, "v1477", "route-preview-release-window-readiness-packet-split-non-participation-node-plan-intake", "v1476", "fixtures/release/shard-readiness-v1476.json", "records Node v1935-v1937 release-window readiness packet split as Node-owned maintenance context"},
    {3, "v1478", "route-preview-release-window-readiness-packet-split-non-participation-types-boundary", "v1477", "fixtures/release/shard-readiness-v1477.json", "keeps release-window packet profile step message forbidden-operation Java fixture and mini-kv evidence types outside C++"},
    {4, "v1479", "route-preview-release-window-readiness-packet-split-non-participation-evidence-boundary", "v1478", "fixtures/release/shard-readiness-v1478.json", "keeps frozen Java v61 and mini-kv v70 evidence modules as read-only Node references"},
    {5, "v1480", "route-preview-release-window-readiness-packet-split-non-participation-policy-boundary", "v1479", "fixtures/release/shard-readiness-v1479.json", "keeps readiness checks release-window steps forbidden operations blockers warnings and recommendations unexecuted by mini-kv"},
    {6, "v1481", "route-preview-release-window-readiness-packet-split-non-participation-stable-entrypoint-no-import", "v1480", "fixtures/release/shard-readiness-v1480.json", "keeps the Node public packet entrypoint stable without importing it into mini-kv"},
    {7, "v1482", "route-preview-release-window-readiness-packet-split-non-participation-loader-renderer-no-execution", "v1481", "fixtures/release/shard-readiness-v1481.json", "keeps Node packet loader renderer summary projection digesting and Markdown rendering unexecuted"},
    {8, "v1483", "route-preview-release-window-readiness-packet-split-non-participation-frozen-java-v61-reference-only", "v1482", "fixtures/release/shard-readiness-v1482.json", "records Java v61 rollback approval fixture evidence as historical metadata only"},
    {9, "v1484", "route-preview-release-window-readiness-packet-split-non-participation-frozen-minikv-v70-reference-only", "v1483", "fixtures/release/shard-readiness-v1483.json", "records mini-kv v70 restore drill evidence as historical metadata without replaying restore"},
    {10, "v1485", "route-preview-release-window-readiness-packet-split-non-participation-release-dry-run-envelope-no-execution", "v1484", "fixtures/release/shard-readiness-v1484.json", "keeps production release dry-run envelope consumption outside mini-kv execution"},
    {11, "v1486", "route-preview-release-window-readiness-packet-split-non-participation-status-routes-no-execution", "v1485", "fixtures/release/shard-readiness-v1485.json", "keeps Node status routes and packet route projection unexecuted by mini-kv"},
    {12, "v1487", "route-preview-release-window-readiness-packet-split-non-participation-endpoint-credential-boundary-closed", "v1486", "fixtures/release/shard-readiness-v1486.json", "keeps endpoint handles raw endpoints credentials secrets and managed connections unread"},
    {13, "v1488", "route-preview-release-window-readiness-packet-split-non-participation-router-write-wal-execution-disabled", "v1487", "fixtures/release/shard-readiness-v1487.json", "keeps routers writes WAL load restore compact and runtime execution disabled"},
    {14, "v1489", "route-preview-release-window-readiness-packet-split-non-participation-command-shardjson-fixture-parity", "v1488", "fixtures/release/shard-readiness-v1488.json", "keeps standalone command current SHARDJSON and v1476-v1490 fixtures aligned"},
    {15, "v1490", "route-preview-release-window-readiness-packet-split-non-participation-clean-workspace-ci-closeout", "v1489", "fixtures/release/shard-readiness-v1489.json", "closes the release-window readiness packet split follow-up with clean CI and no Node import endpoint credential router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> release_window_readiness_packet_split_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_release_window_readiness_packet_split_non_participation_release_number() {
    return first_release_number;
}

int planned_release_window_readiness_packet_split_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_stages
