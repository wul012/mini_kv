#include "minikv/runtime_evidence_receipts.hpp"

#include "minikv/runtime_evidence.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;

std::string uptime_bucket_for_seconds(std::int64_t uptime_seconds) {
    if (uptime_seconds < 60) {
        return "lt_60s";
    }
    if (uptime_seconds < 300) {
        return "lt_5m";
    }
    if (uptime_seconds < 3600) {
        return "lt_1h";
    }
    return "gte_1h";
}

} // namespace

std::string read_command_list_digest(const std::vector<std::string>& commands) {
    std::vector<DigestPart> parts;
    parts.push_back({std::to_string(commands.size())});
    for (const auto& command : commands) {
        parts.push_back({command});
    }
    return runtime_evidence::digest("mini-kv-read-command-list", parts);
}

std::string format_live_read_session_hint_json(std::int64_t uptime_seconds,
                                               const std::vector<std::string>& read_commands) {
    return "{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\","
           "\"session_id_echo\":\"mini-kv-live-read-v102\","
           "\"server_uptime_bucket\":" +
           runtime_evidence::json_string(uptime_bucket_for_seconds(uptime_seconds)) +
           ",\"read_command_list_digest\":" + runtime_evidence::json_string(read_command_list_digest(read_commands)) +
           ",\"read_command_count\":" + std::to_string(read_commands.size()) +
           ",\"read_commands\":" + runtime_evidence::json_string_array(read_commands) +
           ",\"write_commands_allowed\":false,\"auto_start_allowed\":false,"
           "\"node_action\":\"verify session echo, uptime bucket, and read command digest before real-read execution "
           "packet\"}";
}

} // namespace minikv::runtime_evidence_receipts
