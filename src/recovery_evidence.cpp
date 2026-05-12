#include "minikv/recovery_evidence.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace minikv {
namespace {

constexpr std::string_view evidence_version = "mini-kv-restart-recovery.v1";
constexpr std::string_view digest_schema = "mini-kv-recovery-digest-v1";
constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ull;
constexpr std::uint64_t fnv_prime = 1099511628211ull;

std::string json_string(std::string_view value) {
    constexpr char hex[] = "0123456789ABCDEF";
    std::string result = "\"";
    for (const unsigned char ch : value) {
        switch (ch) {
        case '"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        case '\b':
            result += "\\b";
            break;
        case '\f':
            result += "\\f";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        default:
            if (ch < 0x20) {
                result += "\\u00";
                result.push_back(hex[(ch >> 4) & 0x0F]);
                result.push_back(hex[ch & 0x0F]);
            } else {
                result.push_back(static_cast<char>(ch));
            }
            break;
        }
    }
    result.push_back('"');
    return result;
}

std::string format_json_bool(bool value) {
    return value ? "true" : "false";
}

std::string format_json_string_array(const std::vector<std::string_view>& values) {
    std::string response = "[";
    bool first = true;
    for (const auto value : values) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += json_string(value);
    }
    response += "]";
    return response;
}

std::string format_expires_at(std::optional<Store::TimePoint> expires_at) {
    if (!expires_at.has_value()) {
        return "-";
    }

    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at->time_since_epoch()).count();
    return std::to_string(epoch_millis);
}

void append_digest_part(std::string& source, std::string_view value) {
    source += std::to_string(value.size());
    source.push_back(':');
    source += value;
    source.push_back(';');
}

std::uint64_t fnv1a64(std::string_view text) {
    std::uint64_t hash = fnv_offset_basis;
    for (const unsigned char ch : text) {
        hash ^= static_cast<std::uint64_t>(ch);
        hash *= fnv_prime;
    }
    return hash;
}

std::string format_hex64(std::uint64_t value) {
    std::ostringstream output;
    output << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << value;
    return output.str();
}

std::string format_state_json(const RecoveryEvidenceState& state) {
    return "{\"key_count\":" + std::to_string(state.key_count) +
           ",\"digest\":" + json_string(state.digest) + "}";
}

} // namespace

RecoveryEvidenceState recovery_evidence_state(const Store& store) {
    const auto items = store.snapshot_items();

    std::string digest_source;
    append_digest_part(digest_source, digest_schema);
    append_digest_part(digest_source, std::to_string(items.size()));
    for (const auto& item : items) {
        append_digest_part(digest_source, item.key);
        append_digest_part(digest_source, item.value);
        append_digest_part(digest_source, format_expires_at(item.expires_at));
    }

    return RecoveryEvidenceState{
        items.size(),
        "fnv1a64:" + format_hex64(fnv1a64(digest_source)),
    };
}

RestartRecoveryEvidence make_restart_recovery_evidence(const Store& before_restart,
                                                       const Store& after_restart,
                                                       std::size_t snapshot_saved_keys,
                                                       const WalReplayReport& wal_replay) {
    RestartRecoveryEvidence evidence;
    evidence.before = recovery_evidence_state(before_restart);
    evidence.after = recovery_evidence_state(after_restart);
    evidence.digests_match = evidence.before.digest == evidence.after.digest;
    evidence.recovered = evidence.digests_match && evidence.before.key_count == evidence.after.key_count &&
                         wal_replay.skipped_records == 0 && wal_replay.truncated_records == 0 &&
                         wal_replay.checksum_failed_records == 0;
    evidence.snapshot_saved_keys = snapshot_saved_keys;
    evidence.wal_replay = wal_replay;
    return evidence;
}

std::string format_restart_recovery_evidence_json(const RestartRecoveryEvidence& evidence) {
    const std::vector<std::string_view> notes = {
        "restart_recovery_sample",
        "manual_snapshot_baseline",
        "wal_replay_verified",
        "not_order_authoritative",
    };

    return "{\"evidence_version\":" + json_string(evidence_version) +
           ",\"read_only\":true,\"execution_allowed\":false" +
           ",\"recovered\":" + format_json_bool(evidence.recovered) +
           ",\"digests_match\":" + format_json_bool(evidence.digests_match) +
           ",\"before\":" + format_state_json(evidence.before) +
           ",\"after\":" + format_state_json(evidence.after) +
           ",\"snapshot\":{\"supported\":true,\"mode\":\"manual\",\"saved_keys\":" +
           std::to_string(evidence.snapshot_saved_keys) + "}" +
           ",\"wal\":{\"enabled\":true,\"replay\":{\"applied_records\":" +
           std::to_string(evidence.wal_replay.applied_records) +
           ",\"skipped_records\":" + std::to_string(evidence.wal_replay.skipped_records) +
           ",\"truncated_records\":" + std::to_string(evidence.wal_replay.truncated_records) +
           ",\"checksum_failed_records\":" + std::to_string(evidence.wal_replay.checksum_failed_records) + "}}" +
           ",\"diagnostics\":{\"order_authoritative\":false,\"write_commands_executed_by_evidence\":false,"
           "\"notes\":" + format_json_string_array(notes) + "}}";
}

} // namespace minikv
