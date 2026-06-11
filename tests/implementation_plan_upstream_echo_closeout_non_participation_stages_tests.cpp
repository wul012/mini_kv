#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages;

    const auto records = stages::implementation_plan_upstream_echo_closeout_non_participation_stages();
    assert(stages::first_implementation_plan_upstream_echo_closeout_non_participation_release_number() == 1473);
    assert(stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count() == 3);
    assert(records.size() == 3);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1473"});
    assert(records.front().stage ==
           std::string{
               "route-preview-implementation-plan-upstream-echo-closeout-non-participation-source-freeze"});
    assert(records.front().source_frozen_release_version == std::string{"v1472"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1472.json"});

    assert(records.back().sequence == 3);
    assert(records.back().release_version == std::string{"v1475"});
    assert(records.back().stage ==
           std::string{
               "route-preview-implementation-plan-upstream-echo-closeout-non-participation-clean-fixture-closeout"});

    int expected_release = 1473;
    int expected_source = 1472;
    int expected_sequence = 1;
    for (const auto& record : records) {
        assert(record.sequence == expected_sequence);
        assert(record.release_version == "v" + std::to_string(expected_release));
        assert(record.source_frozen_release_version == "v" + std::to_string(expected_source));
        assert(record.source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_source) + ".json");
        ++expected_release;
        ++expected_source;
        ++expected_sequence;
    }

    return 0;
}
