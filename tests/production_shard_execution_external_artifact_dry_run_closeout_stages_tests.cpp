#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace closeout =
        minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout;
    namespace stages =
        minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages;

    const auto records = stages::production_shard_execution_external_artifact_dry_run_closeout_stages();
    assert(records.size() == 5);
    assert(stages::first_production_shard_execution_external_artifact_dry_run_closeout_release_number() == 1602);
    assert(stages::planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count() == 5);
    assert(closeout::published_stage_count() == 5);

    assert(records.front().release_version == std::string{"v1602"});
    assert(records.front().stage ==
           std::string{"production-shard-execution-external-artifact-dry-run-closeout-plan-intake"});
    assert(records.front().source_frozen_release_version == std::string{"v1601"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1601.json"});

    assert(records.back().release_version == std::string{"v1606"});
    assert(records.back().stage ==
           std::string{"production-shard-execution-external-artifact-dry-run-closeout-clean-ci-closeout"});
    assert(records.back().source_frozen_release_version == std::string{"v1605"});
    assert(records.back().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1605.json"});

    for (int index = 0; index < 5; ++index) {
        const int expected_release = 1602 + index;
        assert(records[static_cast<std::size_t>(index)].sequence == index + 1);
        assert(records[static_cast<std::size_t>(index)].release_version ==
               "v" + std::to_string(expected_release));
        assert(records[static_cast<std::size_t>(index)].source_frozen_release_version ==
               "v" + std::to_string(expected_release - 1));
        assert(records[static_cast<std::size_t>(index)].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    const auto catalog =
        closeout::format_production_shard_execution_external_artifact_dry_run_closeout_stage_catalog_json();
    assert(catalog.find("\"releaseVersion\":\"v1606\"") != std::string::npos);
    assert(catalog.find("missing-artifact-gate") != std::string::npos);

    return 0;
}
