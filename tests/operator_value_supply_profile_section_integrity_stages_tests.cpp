#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_operator_value_supply_profile_section_integrity_stages;

    const auto records = stages::operator_value_supply_profile_section_integrity_stages();
    assert(stages::first_operator_value_supply_profile_section_integrity_release_number() == 1161);
    assert(stages::planned_operator_value_supply_profile_section_integrity_stage_count() == 10);
    assert(records.size() == 10);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1161"});
    assert(records.front().source_frozen_release_version == std::string{"v1160"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1160.json"});
    assert(records.back().sequence == 10);
    assert(records.back().release_version == std::string{"v1170"});
    assert(records.back().stage ==
           std::string{"route-preview-operator-value-supply-profile-section-integrity-closeout-summary"});

    int expected_release = 1161;
    int expected_source = 1160;
    for (const auto& record : records) {
        assert(record.release_version == "v" + std::to_string(expected_release));
        assert(record.source_frozen_release_version == "v" + std::to_string(expected_source));
        assert(record.source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_source) + ".json");
        ++expected_release;
        ++expected_source;
    }

    return 0;
}
