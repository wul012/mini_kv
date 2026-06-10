#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages;

    const auto records = stages::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages();
    assert(stages::first_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_release_number() ==
           1443);
    assert(stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count() ==
           30);
    assert(records.size() == 30);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v1443"});
    assert(records.front().source_frozen_release_version == std::string{"v1442"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1442.json"});
    assert(records.back().sequence == 30);
    assert(records.back().release_version == std::string{"v1472"});
    assert(records.back().stage ==
           std::string{
               "route-preview-sandbox-endpoint-credential-resolver-upstream-echo-non-participation-clean-workspace-ci-closeout"});

    int expected_release = 1443;
    int expected_source = 1442;
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
