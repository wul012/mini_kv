#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 6> stages{{
    {1, "v1586", "code-walkthrough-quality-gate-non-participation-plan-intake", "v1585",
     "fixtures/release/shard-readiness-v1585.json",
     "records Node v2077 documentation-quality closeout as observed context without reading Node route output"},
    {2, "v1587", "code-walkthrough-quality-gate-non-participation-standard-section-governance", "v1586",
     "fixtures/release/shard-readiness-v1586.json",
     "records the required goal, role, non-goal, entry, evidence, boundary, test, and integration-value sections"},
    {3, "v1588", "code-walkthrough-quality-gate-non-participation-no-placeholder-floor", "v1587",
     "fixtures/release/shard-readiness-v1587.json",
     "records that tiny maintenance may omit walkthroughs, but written walkthroughs cannot be filler"},
    {4, "v1589", "code-walkthrough-quality-gate-non-participation-archive-fixture-alignment", "v1588",
     "fixtures/release/shard-readiness-v1588.json",
     "records screenshots, fixtures, README summaries, and walkthrough claims as one reviewable evidence chain"},
    {5, "v1590", "code-walkthrough-quality-gate-non-participation-test-and-quality-scan", "v1589",
     "fixtures/release/shard-readiness-v1589.json",
     "records focused tests, fixture parsing, whitespace checks, and no-placeholder assertions for the quality gate"},
    {6, "v1591", "code-walkthrough-quality-gate-non-participation-clean-ci-closeout", "v1590",
     "fixtures/release/shard-readiness-v1590.json",
     "closes the six-version quality gate with clean CI, cleanup, and no local long-running processes"},
}};

} // namespace

std::span<const StageRecord> code_walkthrough_quality_gate_non_participation_stages() {
    return stages;
}

int first_code_walkthrough_quality_gate_non_participation_release_number() {
    return 1586;
}

int planned_code_walkthrough_quality_gate_non_participation_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_stages
