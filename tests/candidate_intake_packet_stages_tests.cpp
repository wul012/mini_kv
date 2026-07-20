#include "minikv/shard_preview_candidate_intake_packet_stages.hpp"

#include <cassert>
#include <string>
#include <string_view>

int main() {
    namespace stages = minikv::shard_preview_candidate_intake_packet_stages;

    const auto catalog = stages::candidate_intake_packet_stages();
    assert(catalog.size() == 25);
    assert(stages::first_candidate_intake_packet_release_number() == 931);
    assert(stages::planned_candidate_intake_packet_stage_count() == 25);
    assert(catalog.front().release_version == std::string{"v931"});
    assert(catalog.front().source_frozen_release_version == std::string{"v930"});
    assert(catalog.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v930.json"});
    assert(catalog.back().release_version == std::string{"v955"});
    assert(catalog.back().stage == std::string{"route-preview-candidate-intake-packet-closeout-summary"});
    assert(catalog.back().source_frozen_release_version == std::string{"v954"});
    assert(catalog.back().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v954.json"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        assert(catalog[index].source_frozen_fixture_path.find("fixtures/release/shard-readiness-v") !=
               std::string_view::npos);
    }
    return 0;
}
