#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks {

struct ProductionShardExecutionMiniKvOwnerReceiptBundleCheck {
    int sequence;
    std::string_view check_code;
    std::string_view receipt_slot;
    std::string_view protects;
};

std::span<const ProductionShardExecutionMiniKvOwnerReceiptBundleCheck>
production_shard_execution_mini_kv_owner_receipt_bundle_checks();
int planned_production_shard_execution_mini_kv_owner_receipt_bundle_check_count();
std::string format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks
