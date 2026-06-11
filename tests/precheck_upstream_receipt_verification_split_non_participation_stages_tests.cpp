#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages;

    const auto records = stages::precheck_upstream_receipt_verification_split_non_participation_stages();
    assert(records.size() == 20);
    assert(stages::first_precheck_upstream_receipt_verification_split_non_participation_release_number() == 1526);
    assert(stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count() == 20);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == "v1526");
    assert(records.front().stage ==
           "route-preview-precheck-upstream-receipt-verification-split-non-participation-entrypoint-export-boundary");
    assert(records.front().source_frozen_release_version == "v1525");
    assert(records.front().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1525.json");

    assert(records.back().sequence == 20);
    assert(records.back().release_version == "v1545");
    assert(records.back().stage ==
           "route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout");
    assert(records.back().source_frozen_release_version == "v1544");
    assert(records.back().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1544.json");

    for (std::size_t index = 0; index < records.size(); ++index) {
        const int expected_release = 1526 + static_cast<int>(index);
        assert(records[index].sequence == static_cast<int>(index + 1));
        assert(records[index].release_version == "v" + std::to_string(expected_release));
        assert(records[index].source_frozen_release_version == "v" + std::to_string(expected_release - 1));
        assert(records[index].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    return 0;
}
