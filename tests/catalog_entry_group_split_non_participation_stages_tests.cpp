#include "minikv/shard_preview_catalog_entry_group_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_catalog_entry_group_split_non_participation_stages;

    const auto records = stages::catalog_entry_group_split_non_participation_stages();
    assert(stages::first_catalog_entry_group_split_non_participation_release_number() == 1296);
    assert(stages::planned_catalog_entry_group_split_non_participation_stage_count() == 35);
    assert(records.size() == 35);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1296"});
    assert(records.front().source_frozen_release_version == std::string{"v1295"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1295.json"});
    assert(records.back().sequence == 35);
    assert(records.back().release_version == std::string{"v1330"});
    assert(records.back().stage ==
           std::string{"route-preview-catalog-entry-group-split-non-participation-github-actions-closeout"});

    int expected_release = 1296;
    int expected_source = 1295;
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
