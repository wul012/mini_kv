#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_stages.hpp"

#include <array>

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1406;

constexpr std::array<StageRecord, 12> stage_records = {{
    {1, "v1406", "route-preview-type-barrel-split-follow-up-fixture-audit-source-follow-up-freeze", "v1405", "fixtures/release/shard-readiness-v1405.json", "freezes the v1405 type barrel split follow-up non-participation fixture as a versioned source"},
    {2, "v1407", "route-preview-type-barrel-split-follow-up-fixture-audit-current-fixture-parity", "v1406", "fixtures/release/shard-readiness-v1406.json", "keeps current shard-readiness fixture parity after the audit section is embedded"},
    {3, "v1408", "route-preview-type-barrel-split-follow-up-fixture-audit-versioned-chain-continuity", "v1407", "fixtures/release/shard-readiness-v1407.json", "continues the versioned fixture chain without reading live sibling services"},
    {4, "v1409", "route-preview-type-barrel-split-follow-up-fixture-audit-command-catalog-read-only", "v1408", "fixtures/release/shard-readiness-v1408.json", "adds command catalog visibility as read-only metadata only"},
    {5, "v1410", "route-preview-type-barrel-split-follow-up-fixture-audit-cli-payload-boundary", "v1409", "fixtures/release/shard-readiness-v1409.json", "keeps large payload checks on CLI and fixture parity evidence without enabling execution"},
    {6, "v1411", "route-preview-type-barrel-split-follow-up-fixture-audit-tcp-standalone-command", "v1410", "fixtures/release/shard-readiness-v1410.json", "uses TCP smoke only for standalone read command metadata"},
    {7, "v1412", "route-preview-type-barrel-split-follow-up-fixture-audit-archive-screenshot-retention", "v1411", "fixtures/release/shard-readiness-v1411.json", "retains command screenshot evidence as archive material without runtime archive walks"},
    {8, "v1413", "route-preview-type-barrel-split-follow-up-fixture-audit-walkthrough-index-retention", "v1412", "fixtures/release/shard-readiness-v1412.json", "keeps code walkthrough indexing as documentation-only evidence"},
    {9, "v1414", "route-preview-type-barrel-split-follow-up-fixture-audit-node-test-only-shell-no-import", "v1413", "fixtures/release/shard-readiness-v1413.json", "proves Node test-only shell split modules are not imported or executed by mini-kv"},
    {10, "v1415", "route-preview-type-barrel-split-follow-up-fixture-audit-router-write-wal-guard", "v1414", "fixtures/release/shard-readiness-v1414.json", "keeps active router write routing load restore compact and WAL touch disabled"},
    {11, "v1416", "route-preview-type-barrel-split-follow-up-fixture-audit-whitespace-quality-gate", "v1415", "fixtures/release/shard-readiness-v1415.json", "keeps documentation and fixture closeout under git whitespace quality checks"},
    {12, "v1417", "route-preview-type-barrel-split-follow-up-fixture-audit-clean-workspace-ci-closeout", "v1416", "fixtures/release/shard-readiness-v1416.json", "closes the audit with clean workspace CI evidence and no Node import router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> type_barrel_split_follow_up_fixture_audit_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_type_barrel_split_follow_up_fixture_audit_release_number() {
    return first_release_number;
}

int planned_type_barrel_split_follow_up_fixture_audit_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_stages
