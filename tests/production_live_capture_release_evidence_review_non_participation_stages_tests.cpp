#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_stages;

    const auto records = stages::production_live_capture_release_evidence_review_non_participation_stages();
    assert(stages::first_production_live_capture_release_evidence_review_non_participation_release_number() == 1261);
    assert(stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count() == 35);
    assert(records.size() == 35);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1261"});
    assert(records.front().source_frozen_release_version == std::string{"v1260"});
    assert(records.front().source_frozen_fixture_path == std::string{"fixtures/release/shard-readiness-v1260.json"});
    assert(records.back().sequence == 35);
    assert(records.back().release_version == std::string{"v1295"});
    assert(records.back().stage ==
           std::string{
               "route-preview-production-live-capture-release-evidence-review-non-participation-closeout-summary"});

    int expected_release = 1261;
    int expected_source = 1260;
    for (const auto& record : records) {
        assert(record.release_version == "v" + std::to_string(expected_release));
        assert(record.source_frozen_release_version == "v" + std::to_string(expected_source));
        assert(record.source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_source) + ".json");
        ++expected_release;
        ++expected_source;
    }

    return 0;
}
