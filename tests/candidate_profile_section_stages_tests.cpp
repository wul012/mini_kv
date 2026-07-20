#include "minikv/shard_preview_candidate_profile_section_stages.hpp"

#include <cassert>
#include <cstddef>
#include <string_view>

int main() {
    namespace stages = minikv::shard_preview_candidate_profile_section_stages;

    const auto catalog = stages::candidate_profile_section_stages();
    assert(catalog.size() == 25);
    assert(stages::first_candidate_profile_section_release_number() == 1011);
    assert(stages::planned_candidate_profile_section_stage_count() == 25);
    assert(catalog.front().release_version == std::string_view{"v1011"});
    assert(catalog.front().source_frozen_release_version == std::string_view{"v1010"});
    assert(catalog.front().source_frozen_fixture_path ==
           std::string_view{"fixtures/release/shard-readiness-v1010.json"});
    assert(catalog.back().release_version == std::string_view{"v1035"});
    assert(catalog.back().source_frozen_release_version == std::string_view{"v1034"});
    assert(catalog.back().stage == std::string_view{"route-preview-candidate-profile-section-closeout-summary"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
    }
    return 0;
}
