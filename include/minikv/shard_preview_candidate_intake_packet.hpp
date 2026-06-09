#pragma once

#include <string>

namespace minikv::shard_preview_candidate_intake_packet {

std::string format_candidate_intake_packet_json();
std::string format_candidate_intake_packet_stage_catalog_json();
std::string candidate_intake_packet_digest_marker();
std::string candidate_intake_packet_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_intake_packet
