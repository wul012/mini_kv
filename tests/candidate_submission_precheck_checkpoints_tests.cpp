#include "minikv/shard_preview_candidate_submission_precheck_checkpoints.hpp"

#include <cassert>
#include <string>
#include <string_view>

int main() {
    namespace checkpoints = minikv::shard_preview_candidate_submission_precheck_checkpoints;

    const auto catalog = checkpoints::candidate_submission_precheck_checkpoints();
    assert(catalog.size() == 10);
    assert(checkpoints::first_candidate_submission_precheck_release_number() == 921);
    assert(checkpoints::planned_candidate_submission_precheck_checkpoint_count() == 10);

    assert(catalog.front().sequence == 1);
    assert(catalog.front().release_version == std::string{"v921"});
    assert(catalog.front().source_frozen_release_version == std::string{"v920"});
    assert(catalog.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v920.json"});
    assert(catalog.back().sequence == 10);
    assert(catalog.back().release_version == std::string{"v930"});
    assert(catalog.back().stage == std::string{"route-preview-candidate-submission-precheck-closeout-summary"});
    assert(catalog.back().source_frozen_release_version == std::string{"v929"});
    assert(catalog.back().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v929.json"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        assert(catalog[index].source_frozen_fixture_path.find("fixtures/release/shard-readiness-v") !=
               std::string_view::npos);
    }
}
