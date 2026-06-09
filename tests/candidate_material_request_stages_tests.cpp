#include "minikv/shard_preview_candidate_material_request_stages.hpp"

#include <cassert>
#include <string>
#include <string_view>

int main() {
    namespace stages = minikv::shard_preview_candidate_material_request_stages;

    const auto catalog = stages::candidate_material_request_stages();
    assert(catalog.size() == 10);
    assert(stages::first_candidate_material_request_release_number() == 956);
    assert(stages::planned_candidate_material_request_stage_count() == 10);
    assert(catalog.front().release_version == std::string{"v956"});
    assert(catalog.front().source_frozen_release_version == std::string{"v955"});
    assert(catalog.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v955.json"});
    assert(catalog.back().release_version == std::string{"v965"});
    assert(catalog.back().stage == std::string{"route-preview-candidate-material-request-closeout-summary"});
    assert(catalog.back().source_frozen_release_version == std::string{"v964"});
    assert(catalog.back().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v964.json"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        assert(catalog[index].source_frozen_fixture_path.find("fixtures/release/shard-readiness-v") !=
               std::string_view::npos);
    }
}
