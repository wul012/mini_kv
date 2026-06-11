#pragma once

#include <string>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet {

std::string format_production_shard_execution_owner_receipt_request_packet_json();
std::string format_production_shard_execution_owner_receipt_request_packet_stage_catalog_json();
std::string production_shard_execution_owner_receipt_request_packet_digest_marker();
std::string production_shard_execution_owner_receipt_request_packet_status();
int published_stage_count();

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet
