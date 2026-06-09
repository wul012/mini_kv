#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_stages.hpp"

#include <cassert>
#include <cstddef>
#include <string_view>

int main() {
    namespace stages = minikv::shard_preview_candidate_material_submission_precheck_integrity_stages;

    const auto catalog = stages::candidate_material_submission_precheck_integrity_stages();
    assert(catalog.size() == 25);
    assert(stages::first_candidate_material_submission_precheck_integrity_release_number() == 986);
    assert(stages::planned_candidate_material_submission_precheck_integrity_stage_count() == 25);
    assert(catalog.front().release_version == std::string_view{"v986"});
    assert(catalog.front().source_frozen_release_version == std::string_view{"v985"});
    assert(catalog.front().source_frozen_fixture_path == std::string_view{"fixtures/release/shard-readiness-v985.json"});
    assert(catalog.back().release_version == std::string_view{"v1010"});
    assert(catalog.back().source_frozen_release_version == std::string_view{"v1009"});
    assert(catalog.back().stage ==
           std::string_view{"route-preview-candidate-material-submission-precheck-integrity-closeout-summary"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
    }
}
