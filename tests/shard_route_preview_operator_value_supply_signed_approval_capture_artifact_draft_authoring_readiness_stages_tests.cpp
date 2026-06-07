#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages.hpp"
#include "test_support.hpp"

#include <cassert>
#include <cstddef>
#include <string>

int main() {
    namespace stages =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages;

    const auto catalog = stages::signed_approval_capture_artifact_draft_authoring_readiness_stages();
    assert(stages::first_signed_approval_capture_artifact_draft_authoring_readiness_release_number() == 736);
    assert(stages::planned_signed_approval_capture_artifact_draft_authoring_readiness_stage_count() == 25);
    assert(catalog.size() == 25);

    assert(catalog.front().sequence == 1);
    assert(catalog.front().release_version == "v736");
    assert(catalog.front().source_frozen_release_version == "v735");
    assert(catalog.front().source_frozen_fixture_path == "fixtures/release/shard-readiness-v735.json");

    assert(catalog.back().sequence == 25);
    assert(catalog.back().release_version == "v760");
    assert(catalog.back().source_frozen_release_version == "v759");
    assert(catalog.back().source_frozen_fixture_path == "fixtures/release/shard-readiness-v759.json");

    for (std::size_t index = 1; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        const std::string expected_source_version = "v" + std::to_string(735 + index);
        const std::string expected_fixture =
            "fixtures/release/shard-readiness-v" + std::to_string(735 + index) + ".json";
        minikv::test_support::assert_contains(std::string(catalog[index].source_frozen_release_version),
                                              expected_source_version);
        minikv::test_support::assert_contains(std::string(catalog[index].source_frozen_fixture_path),
                                              expected_fixture);
    }

    return 0;
}
