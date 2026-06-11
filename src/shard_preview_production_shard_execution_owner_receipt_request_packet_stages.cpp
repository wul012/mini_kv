#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 5> stages{{
    {1, "v1592", "production-shard-execution-owner-receipt-request-packet-plan-intake", "v1591",
     "fixtures/release/shard-readiness-v1591.json",
     "records Node v2087 owner receipt request packet direction without claiming signed receipts exist"},
    {2, "v1593", "production-shard-execution-owner-receipt-request-packet-mini-kv-slot-catalog", "v1592",
     "fixtures/release/shard-readiness-v1592.json",
     "records mini-kv abort rollback lifecycle ownership and cleanup no-start proof receipt request slots"},
    {3, "v1594", "production-shard-execution-owner-receipt-request-packet-java-cleanup-slot-catalog", "v1593",
     "fixtures/release/shard-readiness-v1593.json",
     "records Java owner receipt and cross-project cleanup reconciliation slots as request-only evidence"},
    {4, "v1595", "production-shard-execution-owner-receipt-request-packet-unsigned-boundary-validation", "v1594",
     "fixtures/release/shard-readiness-v1594.json",
     "proves missing signed sibling receipts keep production shard execution and managed audit binding closed"},
    {5, "v1596", "production-shard-execution-owner-receipt-request-packet-clean-ci-closeout", "v1595",
     "fixtures/release/shard-readiness-v1595.json",
     "closes the request packet with split per-version archives, clean tests, and no execution authority"},
}};

} // namespace

std::span<const StageRecord> production_shard_execution_owner_receipt_request_packet_stages() {
    return stages;
}

int first_production_shard_execution_owner_receipt_request_packet_release_number() {
    return 1592;
}

int planned_production_shard_execution_owner_receipt_request_packet_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_stages
