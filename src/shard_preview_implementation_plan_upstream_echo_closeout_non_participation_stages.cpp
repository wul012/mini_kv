#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number = 1473;

constexpr std::array<StageRecord, 3> stage_records = {{
    {1, "v1473", "route-preview-implementation-plan-upstream-echo-closeout-non-participation-source-freeze", "v1472", "fixtures/release/shard-readiness-v1472.json", "freezes the v1472 sandbox endpoint credential resolver upstream echo non-participation fixture as source"},
    {2, "v1474", "route-preview-implementation-plan-upstream-echo-closeout-non-participation-node-closeout-map", "v1473", "fixtures/release/shard-readiness-v1473.json", "records Node v1934 implementation-plan upstream echo split closeout as documentation-only maintenance context"},
    {3, "v1475", "route-preview-implementation-plan-upstream-echo-closeout-non-participation-clean-fixture-closeout", "v1474", "fixtures/release/shard-readiness-v1474.json", "closes the three-version follow-up with current fixture parity and no Node import endpoint credential router write WAL or execution authority"},
}};

} // namespace

std::span<const StageRecord> implementation_plan_upstream_echo_closeout_non_participation_stages() {
    return {stage_records.data(), stage_records.size()};
}

int first_implementation_plan_upstream_echo_closeout_non_participation_release_number() {
    return first_release_number;
}

int planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count() {
    return static_cast<int>(stage_records.size());
}

} // namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages
