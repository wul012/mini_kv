#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages;

    const auto records =
        stages::disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages();
    assert(stages::
               first_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_release_number() ==
           1491);
    assert(stages::
               planned_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stage_count() ==
           15);
    assert(records.size() == 15);

    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1491"});
    assert(records.front().source_frozen_release_version == std::string{"v1490"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1490.json"});
    assert(records.back().sequence == 15);
    assert(records.back().release_version == std::string{"v1505"});
    assert(records.back().stage == std::string{
                                       "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-clean-workspace-ci-closeout"});

    int expected_release = 1491;
    int expected_source = 1490;
    int expected_sequence = 1;
    for (const auto& record : records) {
        assert(record.sequence == expected_sequence);
        assert(record.release_version == "v" + std::to_string(expected_release));
        assert(record.source_frozen_release_version == "v" + std::to_string(expected_source));
        assert(record.source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_source) + ".json");
        ++expected_release;
        ++expected_source;
        ++expected_sequence;
    }

    return 0;
}
