#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_code_walkthrough_quality_gate_non_participation_stages;

    const auto records = stages::code_walkthrough_quality_gate_non_participation_stages();
    assert(records.size() == 6);
    assert(stages::first_code_walkthrough_quality_gate_non_participation_release_number() == 1586);
    assert(stages::planned_code_walkthrough_quality_gate_non_participation_stage_count() == 6);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == "v1586");
    assert(records.front().stage == "code-walkthrough-quality-gate-non-participation-plan-intake");
    assert(records.front().source_frozen_release_version == "v1585");
    assert(records.front().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1585.json");

    assert(records.back().sequence == 6);
    assert(records.back().release_version == "v1591");
    assert(records.back().stage == "code-walkthrough-quality-gate-non-participation-clean-ci-closeout");
    assert(records.back().source_frozen_release_version == "v1590");
    assert(records.back().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1590.json");

    for (std::size_t index = 0; index < records.size(); ++index) {
        const int expected_release = 1586 + static_cast<int>(index);
        assert(records[index].sequence == static_cast<int>(index + 1));
        assert(records[index].release_version == "v" + std::to_string(expected_release));
        assert(records[index].source_frozen_release_version == "v" + std::to_string(expected_release - 1));
        assert(records[index].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    return 0;
}
