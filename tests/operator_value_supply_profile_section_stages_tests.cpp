#include "minikv/shard_preview_operator_value_supply_profile_section_stages.hpp"

#include <cassert>
#include <cstddef>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_operator_value_supply_profile_section_stages;

    const auto records = stages::operator_value_supply_profile_section_stages();
    assert(stages::first_operator_value_supply_profile_section_release_number() == 1136);
    assert(stages::planned_operator_value_supply_profile_section_stage_count() == 25);
    assert(records.size() == 25);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1136"});
    assert(records.front().source_frozen_release_version == std::string{"v1135"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1135.json"});
    assert(records.front().stage ==
           std::string{"route-preview-operator-value-supply-profile-section-source-freeze"});
    assert(records.back().sequence == 25);
    assert(records.back().release_version == std::string{"v1160"});
    assert(records.back().source_frozen_release_version == std::string{"v1159"});
    assert(records.back().stage ==
           std::string{"route-preview-operator-value-supply-profile-section-closeout-summary"});

    for (std::size_t index = 0; index < records.size(); ++index) {
        const int expected_sequence = static_cast<int>(index + 1);
        const int expected_release = 1135 + expected_sequence;
        const int expected_source = expected_release - 1;
        assert(records[index].sequence == expected_sequence);
        assert(records[index].release_version == "v" + std::to_string(expected_release));
        assert(records[index].source_frozen_release_version == "v" + std::to_string(expected_source));
        assert(records[index].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_source) + ".json");
    }

    return 0;
}
