#include "minikv/shard_preview_candidate_request_package_integrity_stages.hpp"

#include <cassert>
#include <string>
#include <string_view>

int main() {
    namespace stages = minikv::shard_preview_candidate_request_package_integrity_stages;

    const auto catalog = stages::candidate_request_package_integrity_stages();
    assert(catalog.size() == 25);
    assert(stages::first_candidate_request_package_integrity_release_number() == 896);
    assert(stages::planned_candidate_request_package_integrity_stage_count() == 25);

    assert(catalog.front().sequence == 1);
    assert(catalog.front().release_version == std::string{"v896"});
    assert(catalog.front().source_frozen_release_version == std::string{"v895"});
    assert(catalog.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v895.json"});
    assert(catalog.back().sequence == 25);
    assert(catalog.back().release_version == std::string{"v920"});
    assert(catalog.back().stage == std::string{"route-preview-candidate-request-package-integrity-closeout-summary"});
    assert(catalog.back().source_frozen_release_version == std::string{"v919"});
    assert(catalog.back().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v919.json"});

    for (std::size_t index = 0; index < catalog.size(); ++index) {
        assert(catalog[index].sequence == static_cast<int>(index + 1));
        assert(catalog[index].source_frozen_fixture_path.find("fixtures/release/shard-readiness-v") !=
               std::string_view::npos);
    }
    return 0;
}
