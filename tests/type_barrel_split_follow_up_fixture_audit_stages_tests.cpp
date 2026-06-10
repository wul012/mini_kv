#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_stages;

    const auto records = stages::type_barrel_split_follow_up_fixture_audit_stages();
    assert(stages::first_type_barrel_split_follow_up_fixture_audit_release_number() == 1406);
    assert(stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count() == 12);
    assert(records.size() == 12);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1406"});
    assert(records.front().source_frozen_release_version == std::string{"v1405"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1405.json"});
    assert(records.back().sequence == 12);
    assert(records.back().release_version == std::string{"v1417"});
    assert(records.back().stage ==
           std::string{"route-preview-type-barrel-split-follow-up-fixture-audit-clean-workspace-ci-closeout"});

    int expected_release = 1406;
    int expected_source = 1405;
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
