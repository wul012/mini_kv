#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_stages;

    const auto records =
        stages::blocked_execution_rehearsal_split_non_participation_stages();
    assert(stages::first_blocked_execution_rehearsal_split_non_participation_release_number() == 1506);
    assert(stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count() == 20);
    assert(records.size() == 20);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1506"});
    assert(records.front().stage == std::string{
                                      "route-preview-blocked-execution-rehearsal-split-non-participation-entrypoint-boundary"});
    assert(records.front().source_frozen_release_version == std::string{"v1505"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1505.json"});
    assert(records.back().sequence == 20);
    assert(records.back().release_version == std::string{"v1525"});
    assert(records.back().stage == std::string{
                                     "route-preview-blocked-execution-rehearsal-split-non-participation-clean-workspace-ci-closeout"});

    int expected_release = 1506;
    int expected_source = 1505;
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
