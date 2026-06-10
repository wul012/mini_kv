#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1418;

constexpr std::array<StageRecord, 25> stage_records = {{
    {1, "v1418", "route-preview-disabled-precheck-upstream-echo-non-participation-source-fixture-audit-freeze", "v1417", "fixtures/release/shard-readiness-v1417.json", "freezes the v1417 fixture-audit evidence before disabled-precheck upstream echo non-participation starts"},
    {2, "v1419", "route-preview-disabled-precheck-upstream-echo-non-participation-node-plan-intake", "v1418", "fixtures/release/shard-readiness-v1418.json", "records Node v1867-v1878 as module split maintenance context only"},
    {3, "v1420", "route-preview-disabled-precheck-upstream-echo-non-participation-command-catalog-read-only", "v1419", "fixtures/release/shard-readiness-v1419.json", "adds command catalog visibility as read-only metadata only"},
    {4, "v1421", "route-preview-disabled-precheck-upstream-echo-non-participation-current-shardjson-rollover", "v1420", "fixtures/release/shard-readiness-v1420.json", "rolls current SHARDJSON status to the disabled-precheck upstream echo boundary"},
    {5, "v1422", "route-preview-disabled-precheck-upstream-echo-non-participation-versioned-fixture-chain-continuity", "v1421", "fixtures/release/shard-readiness-v1421.json", "continues the versioned fixture chain without live sibling reads"},
    {6, "v1423", "route-preview-disabled-precheck-upstream-echo-non-participation-stable-barrel-no-import", "v1422", "fixtures/release/shard-readiness-v1422.json", "proves the Node stable service barrel is not imported by mini-kv"},
    {7, "v1424", "route-preview-disabled-precheck-upstream-echo-non-participation-constants-reference-no-import", "v1423", "fixtures/release/shard-readiness-v1423.json", "keeps Node constants and reference builders outside mini-kv runtime"},
    {8, "v1425", "route-preview-disabled-precheck-upstream-echo-non-participation-checks-messages-no-execution", "v1424", "fixtures/release/shard-readiness-v1424.json", "keeps Node readiness checks blockers warnings and recommendations unexecuted"},
    {9, "v1426", "route-preview-disabled-precheck-upstream-echo-non-participation-core-loader-no-execution", "v1425", "fixtures/release/shard-readiness-v1425.json", "proves the Node core loader is not executed by mini-kv"},
    {10, "v1427", "route-preview-disabled-precheck-upstream-echo-non-participation-renderer-export-stability", "v1426", "fixtures/release/shard-readiness-v1426.json", "records renderer export stability without importing TypeScript renderers"},
    {11, "v1428", "route-preview-disabled-precheck-upstream-echo-non-participation-in-process-node-test-boundary", "v1427", "fixtures/release/shard-readiness-v1427.json", "keeps Node typecheck build and Vitest as Node-owned checks"},
    {12, "v1429", "route-preview-disabled-precheck-upstream-echo-non-participation-no-sibling-service-startup", "v1428", "fixtures/release/shard-readiness-v1428.json", "records no Java or mini-kv sibling service startup requirement"},
    {13, "v1430", "route-preview-disabled-precheck-upstream-echo-non-participation-endpoint-handle-no-read", "v1429", "fixtures/release/shard-readiness-v1429.json", "keeps endpoint handles unread by mini-kv"},
    {14, "v1431", "route-preview-disabled-precheck-upstream-echo-non-participation-raw-endpoint-no-parse", "v1430", "fixtures/release/shard-readiness-v1430.json", "keeps raw endpoint URL parsing disabled"},
    {15, "v1432", "route-preview-disabled-precheck-upstream-echo-non-participation-credential-secret-no-read", "v1431", "fixtures/release/shard-readiness-v1431.json", "keeps credential and secret values unread"},
    {16, "v1433", "route-preview-disabled-precheck-upstream-echo-non-participation-no-active-router", "v1432", "fixtures/release/shard-readiness-v1432.json", "keeps active router installation disabled"},
    {17, "v1434", "route-preview-disabled-precheck-upstream-echo-non-participation-no-write-routing", "v1433", "fixtures/release/shard-readiness-v1433.json", "keeps write routing and store mutation disabled"},
    {18, "v1435", "route-preview-disabled-precheck-upstream-echo-non-participation-no-wal-load-restore-compact", "v1434", "fixtures/release/shard-readiness-v1434.json", "keeps WAL touch load restore and compact execution disabled"},
    {19, "v1436", "route-preview-disabled-precheck-upstream-echo-non-participation-cli-standalone-read-smoke", "v1435", "fixtures/release/shard-readiness-v1435.json", "uses CLI smoke only for standalone read metadata"},
    {20, "v1437", "route-preview-disabled-precheck-upstream-echo-non-participation-tcp-standalone-read-smoke", "v1436", "fixtures/release/shard-readiness-v1436.json", "uses TCP smoke only for standalone read metadata"},
    {21, "v1438", "route-preview-disabled-precheck-upstream-echo-non-participation-fixture-parity-current", "v1437", "fixtures/release/shard-readiness-v1437.json", "keeps current fixture parity aligned with v1438 versioned evidence"},
    {22, "v1439", "route-preview-disabled-precheck-upstream-echo-non-participation-archive-screenshot-retention", "v1438", "fixtures/release/shard-readiness-v1438.json", "retains command screenshot evidence without runtime archive walks"},
    {23, "v1440", "route-preview-disabled-precheck-upstream-echo-non-participation-walkthrough-index-retention", "v1439", "fixtures/release/shard-readiness-v1439.json", "keeps walkthrough indexing as documentation-only evidence"},
    {24, "v1441", "route-preview-disabled-precheck-upstream-echo-non-participation-whitespace-quality-gate", "v1440", "fixtures/release/shard-readiness-v1440.json", "keeps docs fixtures and generated evidence under git whitespace checks"},
    {25, "v1442", "route-preview-disabled-precheck-upstream-echo-non-participation-clean-workspace-ci-closeout", "v1441", "fixtures/release/shard-readiness-v1441.json", "closes the batch with clean workspace CI evidence and no Node import router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> disabled_precheck_upstream_echo_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_disabled_precheck_upstream_echo_non_participation_release_number() {
    return first_release_number;
}

int planned_disabled_precheck_upstream_echo_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_stages
