#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_release_window_readiness_packet_split_non_participation_stages;

    const auto records = stages::release_window_readiness_packet_split_non_participation_stages();
    assert(stages::first_release_window_readiness_packet_split_non_participation_release_number() == 1476);
    assert(stages::planned_release_window_readiness_packet_split_non_participation_stage_count() == 15);
    assert(records.size() == 15);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1476"});
    assert(records.front().source_frozen_release_version == std::string{"v1475"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1475.json"});
    assert(records.back().sequence == 15);
    assert(records.back().release_version == std::string{"v1490"});
    assert(records.back().stage ==
           std::string{
               "route-preview-release-window-readiness-packet-split-non-participation-clean-workspace-ci-closeout"});

    int expected_release = 1476;
    int expected_source = 1475;
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
