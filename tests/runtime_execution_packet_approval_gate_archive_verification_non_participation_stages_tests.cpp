#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages;

    const auto records =
        stages::runtime_execution_packet_approval_gate_archive_verification_non_participation_stages();
    assert(records.size() == 20);
    assert(stages::first_runtime_execution_packet_approval_gate_archive_verification_non_participation_release_number() ==
           1566);
    assert(stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count() ==
           20);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == "v1566");
    assert(records.front().stage ==
           "runtime-execution-packet-approval-gate-archive-verification-non-participation-plan-intake");
    assert(records.front().source_frozen_release_version == "v1565");
    assert(records.front().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1565.json");

    assert(records.back().sequence == 20);
    assert(records.back().release_version == "v1585");
    assert(records.back().stage ==
           "runtime-execution-packet-approval-gate-archive-verification-non-participation-clean-ci-closeout");
    assert(records.back().source_frozen_release_version == "v1584");
    assert(records.back().source_frozen_fixture_path == "fixtures/release/shard-readiness-v1584.json");

    for (std::size_t index = 0; index < records.size(); ++index) {
        const int expected_release = 1566 + static_cast<int>(index);
        assert(records[index].sequence == static_cast<int>(index + 1));
        assert(records[index].release_version == "v" + std::to_string(expected_release));
        assert(records[index].source_frozen_release_version == "v" + std::to_string(expected_release - 1));
        assert(records[index].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    return 0;
}
