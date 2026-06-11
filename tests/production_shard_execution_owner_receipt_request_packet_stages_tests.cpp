#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_stages.hpp"

#include <cassert>
#include <string>

int main() {
    namespace packet = minikv::shard_preview_production_shard_execution_owner_receipt_request_packet;
    namespace stages =
        minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_stages;

    const auto records = stages::production_shard_execution_owner_receipt_request_packet_stages();
    assert(records.size() == 5);
    assert(stages::first_production_shard_execution_owner_receipt_request_packet_release_number() == 1592);
    assert(stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count() == 5);
    assert(packet::published_stage_count() == 5);

    assert(records.front().release_version == std::string{"v1592"});
    assert(records.front().stage ==
           std::string{"production-shard-execution-owner-receipt-request-packet-plan-intake"});
    assert(records.front().source_frozen_release_version == std::string{"v1591"});
    assert(records.front().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1591.json"});

    assert(records.back().release_version == std::string{"v1596"});
    assert(records.back().stage ==
           std::string{"production-shard-execution-owner-receipt-request-packet-clean-ci-closeout"});
    assert(records.back().source_frozen_release_version == std::string{"v1595"});
    assert(records.back().source_frozen_fixture_path ==
           std::string{"fixtures/release/shard-readiness-v1595.json"});

    for (int index = 0; index < 5; ++index) {
        const int expected_release = 1592 + index;
        assert(records[static_cast<std::size_t>(index)].sequence == index + 1);
        assert(records[static_cast<std::size_t>(index)].release_version ==
               "v" + std::to_string(expected_release));
        assert(records[static_cast<std::size_t>(index)].source_frozen_release_version ==
               "v" + std::to_string(expected_release - 1));
        assert(records[static_cast<std::size_t>(index)].source_frozen_fixture_path ==
               "fixtures/release/shard-readiness-v" + std::to_string(expected_release - 1) + ".json");
    }

    const auto catalog = packet::format_production_shard_execution_owner_receipt_request_packet_stage_catalog_json();
    assert(catalog.find("\"sequence\":5") != std::string::npos);
    assert(catalog.find("\"releaseVersion\":\"v1596\"") != std::string::npos);
    assert(catalog.find("clean-ci-closeout") != std::string::npos);

    return 0;
}
