#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages;

    const auto catalog = stages::signed_approval_artifact_draft_profile_section_integrity_stages();
    assert(catalog.size() == 25);
    assert(stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count() == 25);
    assert(stages::first_signed_approval_artifact_draft_profile_section_integrity_release_number() == 1061);

    assert(catalog.front().sequence == 1);
    assert(std::string{catalog.front().release_version} == "v1061");
    assert(std::string{catalog.front().source_frozen_release_version} == "v1060");
    assert(std::string{catalog.front().source_frozen_fixture_path} ==
           "fixtures/release/shard-readiness-v1060.json");

    assert(catalog.back().sequence == 25);
    assert(std::string{catalog.back().release_version} == "v1085");
    assert(std::string{catalog.back().source_frozen_release_version} == "v1084");
    assert(std::string{catalog.back().stage} ==
           "route-preview-signed-approval-artifact-draft-profile-section-integrity-closeout-summary");

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
    }
}
