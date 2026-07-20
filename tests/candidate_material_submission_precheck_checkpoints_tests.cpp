#include "minikv/shard_preview_candidate_material_submission_precheck_checkpoints.hpp"

#include <cassert>
#include <string>

int main() {
    namespace checkpoints = minikv::shard_preview_candidate_material_submission_precheck_checkpoints;

    const auto catalog = checkpoints::candidate_material_submission_precheck_checkpoints();
    assert(catalog.size() == 10);
    assert(checkpoints::first_candidate_material_submission_precheck_release_number() == 976);
    assert(checkpoints::planned_candidate_material_submission_precheck_checkpoint_count() == 10);
    assert(catalog.front().sequence == 1);
    assert(catalog.front().release_version == std::string{"v976"});
    assert(catalog.front().source_frozen_release_version == std::string{"v975"});
    assert(catalog.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v975.json"});
    assert(catalog.back().sequence == 10);
    assert(catalog.back().release_version == std::string{"v985"});
    assert(catalog.back().stage ==
           std::string{"route-preview-candidate-material-submission-precheck-closeout-archive-checkpoint"});
    assert(catalog.back().source_frozen_release_version == std::string{"v984"});
    assert(catalog.back().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v984.json"});
    return 0;
}
