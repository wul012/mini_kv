#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace bundle = minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle;
    namespace stages = minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages;

    const auto records = stages::production_shard_execution_mini_kv_owner_receipt_bundle_stages();
    assert(records.size() == 5);
    assert(stages::first_production_shard_execution_mini_kv_owner_receipt_bundle_release_number() == 1597);
    assert(stages::planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count() == 5);
    assert(bundle::published_stage_count() == 5);

    assert(records.front().release_version == std::string{"v1597"});
    assert(records.front().stage ==
           std::string{"production-shard-execution-mini-kv-owner-receipt-bundle-plan-intake"});
    assert(records.front().source_frozen_release_version == std::string{"v1596"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1596.json"});

    assert(records.back().release_version == std::string{"v1601"});
    assert(records.back().stage ==
           std::string{"production-shard-execution-mini-kv-owner-receipt-bundle-clean-ci-closeout"});
    assert(records.back().source_frozen_release_version == std::string{"v1600"});
    assert(records.back().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1600.json"});

    for (int index = 0; index < 5; ++index) {
        const int expected_release = 1597 + index;
        assert(records[static_cast<std::size_t>(index)].sequence == index + 1);
        assert(records[static_cast<std::size_t>(index)].release_version ==
               "v" + std::to_string(expected_release));
        assert(records[static_cast<std::size_t>(index)].source_frozen_release_version ==
               "v" + std::to_string(expected_release - 1));
        assert(records[static_cast<std::size_t>(index)].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    const auto catalog = bundle::format_production_shard_execution_mini_kv_owner_receipt_bundle_stage_catalog_json();
    assert(catalog.find("\"releaseVersion\":\"v1601\"") != std::string::npos);
    assert(catalog.find("cleanup-no-start-proof") != std::string::npos);

    return 0;
}
