#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 15> stages{{
    {1, "v1491",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-entrypoint-boundary",
     "v1490", "fixtures/release/shard-readiness-v1490.json",
     "freezes v1490 before observing the Node disabled fake harness contract upstream echo verification split"},
    {2, "v1492",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-constants-boundary",
     "v1491", "fixtures/release/shard-readiness-v1491.json",
     "keeps Node route plan evidence path and contract shape constants outside C++"},
    {3, "v1493",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-node-v288-projection",
     "v1492", "fixtures/release/shard-readiness-v1492.json",
     "records Node v288 disabled fake harness contract projection as Node-owned metadata"},
    {4, "v1494",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-java-reference-boundary",
     "v1493", "fixtures/release/shard-readiness-v1493.json",
     "keeps Java v122-v126 evidence reference construction outside mini-kv"},
    {5, "v1495",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-minikv-v127-reference",
     "v1494", "fixtures/release/shard-readiness-v1494.json",
     "keeps mini-kv v127 non-participation receipt parsing as historical Node reference only"},
    {6, "v1496",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-snippet-builder-boundary",
     "v1495", "fixtures/release/shard-readiness-v1495.json",
     "keeps Java and mini-kv expected snippet builders unexecuted by mini-kv"},
    {7, "v1497",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-reference-helper-boundary",
     "v1496", "fixtures/release/shard-readiness-v1496.json",
     "keeps evidence parsing helpers in Node references module"},
    {8, "v1498",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-core-assembly-boundary",
     "v1497", "fixtures/release/shard-readiness-v1497.json",
     "keeps echo verification assembly in Node core without mini-kv execution"},
    {9, "v1499",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-summary-boundary",
     "v1498", "fixtures/release/shard-readiness-v1498.json",
     "keeps summary aggregation in Node core and out of mini-kv runtime"},
    {10, "v1500",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-readiness-policy-boundary",
     "v1499", "fixtures/release/shard-readiness-v1499.json",
     "keeps readiness checks in Node policy without opening managed audit connection"},
    {11, "v1501",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-blocker-policy-boundary",
     "v1500", "fixtures/release/shard-readiness-v1500.json",
     "keeps blocker message collection in Node policy and read-only"},
    {12, "v1502",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-warning-recommendation-boundary",
     "v1501", "fixtures/release/shard-readiness-v1501.json",
     "keeps warnings and recommendations in Node policy without mini-kv routing"},
    {13, "v1503",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-focused-tests-observed",
     "v1502", "fixtures/release/shard-readiness-v1502.json",
     "records Node focused tests as observed sibling evidence only"},
    {14, "v1504",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-typecheck-build-observed",
     "v1503", "fixtures/release/shard-readiness-v1503.json",
     "records Node typecheck and build as observed sibling evidence only"},
    {15, "v1505",
     "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-clean-workspace-ci-closeout",
     "v1504", "fixtures/release/shard-readiness-v1504.json",
     "closes the split non-participation chain with archives cleanup CI and no Node import endpoint credential router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages() {
    return stages;
}

int first_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_release_number() {
    return 1491;
}

int planned_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages
