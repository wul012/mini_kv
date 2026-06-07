#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages.hpp"
#include "test_support.hpp"

#include <cassert>
#include <cstddef>
#include <string>

int main() {
    namespace stages =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages;

    const auto catalog = stages::signed_approval_capture_artifact_preflight_stages();
    assert(stages::first_signed_approval_capture_artifact_preflight_release_number() == 711);
    assert(stages::planned_signed_approval_capture_artifact_preflight_stage_count() == 25);
    assert(catalog.size() == 25);

    assert(catalog.front().sequence == 1);
    assert(catalog.front().release_version == "v711");
    assert(catalog.front().source_frozen_release_version == "v710");
    assert(catalog.front().source_frozen_fixture_path == "fixtures/release/shard-readiness-v710.json");

    assert(catalog.back().sequence == 25);
    assert(catalog.back().release_version == "v735");
    assert(catalog.back().source_frozen_release_version == "v734");
    assert(catalog.back().source_frozen_fixture_path == "fixtures/release/shard-readiness-v734.json");

    for (std::size_t index = 1; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        const std::string expected_source_version = "v" + std::to_string(710 + index);
        const std::string expected_fixture =
            "fixtures/release/shard-readiness-v" + std::to_string(710 + index) + ".json";
        minikv::test_support::assert_contains(std::string(catalog[index].source_frozen_release_version),
                                              expected_source_version);
        minikv::test_support::assert_contains(std::string(catalog[index].source_frozen_fixture_path),
                                              expected_fixture);
    }

    return 0;
}
