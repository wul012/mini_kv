#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 5> stages{{
    {1, "v1597", "production-shard-execution-mini-kv-owner-receipt-bundle-plan-intake", "v1596",
     "fixtures/release/shard-readiness-v1596.json",
     "freezes the v1596 request packet as the exact mini-kv owner receipt source"},
    {2, "v1598", "production-shard-execution-mini-kv-owner-receipt-bundle-abort-rollback", "v1597",
     "fixtures/release/shard-readiness-v1597.json",
     "publishes mini-kv abort and rollback owner receipts without executing either path"},
    {3, "v1599", "production-shard-execution-mini-kv-owner-receipt-bundle-lifecycle-no-start", "v1598",
     "fixtures/release/shard-readiness-v1598.json",
     "publishes lifecycle ownership while proving no service startup or runtime lifecycle mutation"},
    {4, "v1600", "production-shard-execution-mini-kv-owner-receipt-bundle-cleanup-no-start-proof", "v1599",
     "fixtures/release/shard-readiness-v1599.json",
     "publishes cleanup no-start proof without cleanup execution or storage deletion"},
    {5, "v1601", "production-shard-execution-mini-kv-owner-receipt-bundle-clean-ci-closeout", "v1600",
     "fixtures/release/shard-readiness-v1600.json",
     "closes the mini-kv owner receipt bundle with f-root archives and no production authority"},
}};

} // namespace

std::span<const StageRecord> production_shard_execution_mini_kv_owner_receipt_bundle_stages() {
    return stages;
}

int first_production_shard_execution_mini_kv_owner_receipt_bundle_release_number() {
    return 1597;
}

int planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages
