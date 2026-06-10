#include "minikv/shard_preview_catalog_entry_group_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_catalog_entry_group_split_non_participation_stages;

    const auto records = stages::catalog_entry_group_split_non_participation_stages();
    assert(stages::first_catalog_entry_group_split_non_participation_release_number() == 1331);
    assert(stages::planned_catalog_entry_group_split_non_participation_stage_count() == 30);
    assert(records.size() == 30);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1331"});
    assert(records.front().source_frozen_release_version == std::string{"v1330"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1330.json"});
    assert(records.back().sequence == 30);
    assert(records.back().release_version == std::string{"v1360"});
    assert(records.back().stage ==
           std::string{
               "route-preview-catalog-entry-group-split-non-participation-follow-up-github-actions-closeout-summary"});

    int expected_release = 1331;
    int expected_source = 1330;
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
