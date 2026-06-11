#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_checks {

struct ProductionShardExecutionOwnerReceiptRequestPacketCheck {
    int sequence;
    std::string_view check_code;
    std::string_view requested_receipt_slot;
    std::string_view protects;
};

std::span<const ProductionShardExecutionOwnerReceiptRequestPacketCheck>
production_shard_execution_owner_receipt_request_packet_checks();
int planned_production_shard_execution_owner_receipt_request_packet_check_count();
std::string format_production_shard_execution_owner_receipt_request_packet_checks_json(int published_check_count);

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_checks
