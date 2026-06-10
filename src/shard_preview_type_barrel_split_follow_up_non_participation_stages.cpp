#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1386;

constexpr std::array<StageRecord, 20> stage_records = {{
    {1, "v1386", "route-preview-type-barrel-split-follow-up-non-participation-source-fixture-freeze", "v1385", "fixtures/release/shard-readiness-v1385.json", "freezes v1385 type barrel split non-participation as the source fixture before follow-up evidence"},
    {2, "v1387", "route-preview-type-barrel-split-follow-up-non-participation-command-catalog-continuity", "v1386", "fixtures/release/shard-readiness-v1386.json", "keeps command catalog exposure read-only while adding follow-up metadata"},
    {3, "v1388", "route-preview-type-barrel-split-follow-up-non-participation-shardjson-current-rollover", "v1387", "fixtures/release/shard-readiness-v1387.json", "rolls current SHARDJSON status to the follow-up section without changing storage behavior"},
    {4, "v1389", "route-preview-type-barrel-split-follow-up-non-participation-versioned-fixture-chain", "v1388", "fixtures/release/shard-readiness-v1388.json", "keeps every follow-up release represented as a versioned fixture"},
    {5, "v1390", "route-preview-type-barrel-split-follow-up-non-participation-profile-endpoint-no-read", "v1389", "fixtures/release/shard-readiness-v1389.json", "proves Node profile boundary endpoints remain unread by mini-kv"},
    {6, "v1391", "route-preview-type-barrel-split-follow-up-non-participation-node-type-import-guard", "v1390", "fixtures/release/shard-readiness-v1390.json", "keeps stable barrels profile types and aggregate re-exports outside mini-kv includes"},
    {7, "v1392", "route-preview-type-barrel-split-follow-up-non-participation-route-renderer-no-execution", "v1391", "fixtures/release/shard-readiness-v1391.json", "records that route and renderer tests are not executed from mini-kv"},
    {8, "v1393", "route-preview-type-barrel-split-follow-up-non-participation-node-validation-no-execution", "v1392", "fixtures/release/shard-readiness-v1392.json", "keeps Node typecheck build and Vitest outside mini-kv validation"},
    {9, "v1394", "route-preview-type-barrel-split-follow-up-non-participation-tcp-smoke-boundary", "v1393", "fixtures/release/shard-readiness-v1393.json", "uses TCP smoke only for standalone read command metadata"},
    {10, "v1395", "route-preview-type-barrel-split-follow-up-non-participation-large-payload-cli-only", "v1394", "fixtures/release/shard-readiness-v1394.json", "keeps huge SHARDJSON payload verification on CLI and fixture parity checks"},
    {11, "v1396", "route-preview-type-barrel-split-follow-up-non-participation-rolling-current-test-hardening", "v1395", "fixtures/release/shard-readiness-v1395.json", "hardens tests so rolling current status changes preserve historical meaning"},
    {12, "v1397", "route-preview-type-barrel-split-follow-up-non-participation-source-digest-pin", "v1396", "fixtures/release/shard-readiness-v1396.json", "pins the v1385 source digest marker as follow-up context only"},
    {13, "v1398", "route-preview-type-barrel-split-follow-up-non-participation-archive-screenshot-retention", "v1397", "fixtures/release/shard-readiness-v1397.json", "retains archive screenshot evidence without walking runtime archive folders"},
    {14, "v1399", "route-preview-type-barrel-split-follow-up-non-participation-walkthrough-index-continuity", "v1398", "fixtures/release/shard-readiness-v1398.json", "keeps code walkthrough indexing as documentation-only evidence"},
    {15, "v1400", "route-preview-type-barrel-split-follow-up-non-participation-generated-validation-cleanup", "v1399", "fixtures/release/shard-readiness-v1399.json", "documents generated validation cleanup without deleting retained evidence"},
    {16, "v1401", "route-preview-type-barrel-split-follow-up-non-participation-no-service-startup", "v1400", "fixtures/release/shard-readiness-v1400.json", "proves the follow-up does not start mini-kv Java or Node services"},
    {17, "v1402", "route-preview-type-barrel-split-follow-up-non-participation-router-write-wal-guard", "v1401", "fixtures/release/shard-readiness-v1401.json", "keeps active routers write routing load restore compact and WAL touch disabled"},
    {18, "v1403", "route-preview-type-barrel-split-follow-up-non-participation-fixture-parity-current", "v1402", "fixtures/release/shard-readiness-v1402.json", "proves current fixture parity after the follow-up section is embedded"},
    {19, "v1404", "route-preview-type-barrel-split-follow-up-non-participation-whitespace-quality-gate", "v1403", "fixtures/release/shard-readiness-v1403.json", "keeps documentation whitespace quality checks in the closeout path"},
    {20, "v1405", "route-preview-type-barrel-split-follow-up-non-participation-clean-workspace-ci-closeout", "v1404", "fixtures/release/shard-readiness-v1404.json", "closes the follow-up with clean workspace CI evidence and no Node type import endpoint read router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> type_barrel_split_follow_up_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_type_barrel_split_follow_up_non_participation_release_number() {
    return first_release_number;
}

int planned_type_barrel_split_follow_up_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_stages
