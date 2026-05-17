#include "minikv/command_response_formatters.hpp"

#include "minikv/managed_audit_receipts.hpp"
#include "minikv/runtime_evidence_receipts.hpp"
#include "minikv/string_utils.hpp"
#include "minikv/version.hpp"

#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace minikv {
namespace {

constexpr std::size_t key_inventory_limit = 1000;
constexpr int runtime_identity_schema_version = 1;
constexpr int runtime_metrics_schema_version = 1;
constexpr int runtime_smoke_schema_version = 1;
constexpr int storage_evidence_schema_version = 1;

std::string format_yes_no(bool value) {
    return value ? "yes" : "no";
}

std::string format_json_bool(bool value) {
    return value ? "true" : "false";
}

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

std::string format_json_string_array(const std::vector<std::string>& values) {
    std::string response = "[";
    bool first = true;
    for (const auto& value : values) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += json_string(value);
    }
    response += "]";
    return response;
}

std::string format_protocol_json_array(std::string_view protocol) {
    std::string response = "[";
    bool first = true;
    while (!protocol.empty()) {
        const auto comma = protocol.find(',');
        const std::string token = trim_copy(protocol.substr(0, comma));
        if (!token.empty()) {
            if (!first) {
                response += ",";
            }
            first = false;
            response += json_string(token);
        }

        if (comma == std::string_view::npos) {
            break;
        }
        protocol.remove_prefix(comma + 1);
    }
    response += "]";
    return response;
}

std::string format_connection_stats(const CommandConnectionStats& stats) {
    if (!stats.available) {
        return " connection_stats_available=no";
    }

    return " connection_stats_available=yes active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}

} // namespace

std::string format_command_metrics_json(const CommandProcessorMetrics& metrics) {
    const std::uint64_t average_latency_ns =
        metrics.total_commands == 0 ? 0 : metrics.total_latency_ns / metrics.total_commands;

    std::string response = "\"commands\":{\"total_commands\":" + std::to_string(metrics.total_commands) +
                           ",\"successful_commands\":" + std::to_string(metrics.successful_commands) +
                           ",\"error_commands\":" + std::to_string(metrics.error_commands) +
                           ",\"total_latency_ns\":" + std::to_string(metrics.total_latency_ns) +
                           ",\"avg_latency_ns\":" + std::to_string(average_latency_ns) +
                           ",\"max_latency_ns\":" + std::to_string(metrics.max_latency_ns) +
                           ",\"breakdown\":[";

    bool first = true;
    for (const auto& command_metrics : metrics.command_breakdown) {
        const std::uint64_t command_average_latency_ns =
            command_metrics.total_commands == 0 ? 0 : command_metrics.total_latency_ns / command_metrics.total_commands;
        if (!first) {
            response += ",";
        }
        first = false;

        response += "{\"command\":" + json_string(command_metrics.command) +
                    ",\"total_commands\":" + std::to_string(command_metrics.total_commands) +
                    ",\"successful_commands\":" + std::to_string(command_metrics.successful_commands) +
                    ",\"error_commands\":" + std::to_string(command_metrics.error_commands) +
                    ",\"total_latency_ns\":" + std::to_string(command_metrics.total_latency_ns) +
                    ",\"avg_latency_ns\":" + std::to_string(command_average_latency_ns) +
                    ",\"max_latency_ns\":" + std::to_string(command_metrics.max_latency_ns) + "}";
    }

    response += "]}";
    return response;
}

std::string format_command_metrics(const CommandProcessorMetrics& metrics) {
    const std::uint64_t average_latency_ns =
        metrics.total_commands == 0 ? 0 : metrics.total_latency_ns / metrics.total_commands;

    std::string result = "total_commands=" + std::to_string(metrics.total_commands) +
                         " successful_commands=" + std::to_string(metrics.successful_commands) +
                         " error_commands=" + std::to_string(metrics.error_commands) +
                         " total_latency_ns=" + std::to_string(metrics.total_latency_ns) +
                         " avg_latency_ns=" + std::to_string(average_latency_ns) +
                         " max_latency_ns=" + std::to_string(metrics.max_latency_ns) +
                         " command_breakdown=";

    if (metrics.command_breakdown.empty()) {
        return result + "none";
    }

    bool first = true;
    for (const auto& command_metrics : metrics.command_breakdown) {
        const std::uint64_t command_average_latency_ns =
            command_metrics.total_commands == 0 ? 0 : command_metrics.total_latency_ns / command_metrics.total_commands;
        if (!first) {
            result += ";";
        }
        first = false;

        result += command_metrics.command + ":" +
                  std::to_string(command_metrics.total_commands) + "/" +
                  std::to_string(command_metrics.successful_commands) + "/" +
                  std::to_string(command_metrics.error_commands) + "/" +
                  std::to_string(command_metrics.total_latency_ns) + "/" +
                  std::to_string(command_average_latency_ns) + "/" +
                  std::to_string(command_metrics.max_latency_ns);
    }

    return result;
}

namespace command_response_formatters {

std::string format_keys(const std::vector<std::string>& keys) {
    std::string response = "key_count=" + std::to_string(keys.size()) + " keys=";
    bool first = true;
    for (const auto& key : keys) {
        if (!first) {
            response.push_back(' ');
        }
        first = false;
        response += key;
    }
    return response;
}

std::string format_prefixed_keys(std::string_view prefix, const std::vector<std::string>& keys) {
    std::string response = "key_count=" + std::to_string(keys.size()) + " prefix=" + std::string{prefix} + " keys=";
    bool first = true;
    for (const auto& key : keys) {
        if (!first) {
            response.push_back(' ');
        }
        first = false;
        response += key;
    }
    return response;
}

std::string format_keys_json(std::optional<std::string_view> prefix, std::vector<std::string> keys) {
    bool truncated = false;
    if (keys.size() > key_inventory_limit) {
        keys.resize(key_inventory_limit);
        truncated = true;
    }

    std::string response = "{\"prefix\":";
    if (prefix.has_value()) {
        response += json_string(*prefix);
    } else {
        response += "null";
    }

    response += ",\"key_count\":" + std::to_string(keys.size()) + ",\"keys\":[";
    bool first = true;
    for (const auto& key : keys) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += json_string(key);
    }
    response += "],\"truncated\":" + format_json_bool(truncated) +
                ",\"limit\":" + std::to_string(key_inventory_limit) + "}";
    return response;
}

std::string format_walinfo(const WalMaintenanceReport& report) {
    return "wal_bytes=" + std::to_string(report.bytes) + " wal_records=" + std::to_string(report.records) +
           " live_keys=" + std::to_string(report.live_keys) +
           " compact_recommended=" + format_yes_no(report.compact_recommended) +
           " compact_min_records=" + std::to_string(report.options.compact_min_records) +
           " compact_record_ratio=" + std::to_string(report.options.compact_record_ratio) +
           " compact_min_bytes=" + std::to_string(report.options.compact_min_bytes) +
           " manual_compactions=" + std::to_string(report.compaction_stats.manual_compactions) +
           " auto_compactions=" + std::to_string(report.compaction_stats.auto_compactions) +
           " repair_compactions=" + std::to_string(report.compaction_stats.repair_compactions) +
           " compacted_keys=" + std::to_string(report.compaction_stats.compacted_keys) +
           " records_removed=" + std::to_string(report.compaction_stats.records_removed) +
           " bytes_saved=" + std::to_string(report.compaction_stats.bytes_saved);
}

std::string format_stats(std::size_t live_keys,
                         WriteAheadLog* wal,
                         const std::optional<WalMaintenanceReport>& wal_report,
                         const CommandProcessorMetrics& command_metrics,
                         const CommandConnectionStats& stats) {
    std::string response = "live_keys=" + std::to_string(live_keys) +
                           " wal_enabled=" + format_yes_no(wal != nullptr);

    if (wal_report.has_value()) {
        response += " wal_bytes=" + std::to_string(wal_report->bytes) +
                    " wal_records=" + std::to_string(wal_report->records) +
                    " wal_live_keys=" + std::to_string(wal_report->live_keys) +
                    " compact_recommended=" + format_yes_no(wal_report->compact_recommended) +
                    " compact_min_records=" + std::to_string(wal_report->options.compact_min_records) +
                    " compact_record_ratio=" + std::to_string(wal_report->options.compact_record_ratio) +
                    " compact_min_bytes=" + std::to_string(wal_report->options.compact_min_bytes) +
                    " manual_compactions=" + std::to_string(wal_report->compaction_stats.manual_compactions) +
                    " auto_compactions=" + std::to_string(wal_report->compaction_stats.auto_compactions) +
                    " repair_compactions=" + std::to_string(wal_report->compaction_stats.repair_compactions) +
                    " compacted_keys=" + std::to_string(wal_report->compaction_stats.compacted_keys) +
                    " records_removed=" + std::to_string(wal_report->compaction_stats.records_removed) +
                    " bytes_saved=" + std::to_string(wal_report->compaction_stats.bytes_saved);
    }

    response += " " + format_command_metrics(command_metrics);
    response += format_connection_stats(stats);
    return response;
}

std::string format_health(std::size_t live_keys,
                          WriteAheadLog* wal,
                          const std::optional<WalMaintenanceReport>& wal_report,
                          const CommandProcessorMetrics& command_metrics,
                          const CommandConnectionStats& stats) {
    std::string response = "OK live_keys=" + std::to_string(live_keys) +
                           " wal_enabled=" + format_yes_no(wal != nullptr);

    if (wal_report.has_value()) {
        response += " compact_recommended=" + format_yes_no(wal_report->compact_recommended);
    } else {
        response += " compact_recommended=na";
    }

    response += " " + format_command_metrics(command_metrics);
    response += format_connection_stats(stats);
    return response;
}

std::string format_info(std::size_t live_keys,
                        WriteAheadLog* wal,
                        const CommandRuntimeInfo& runtime_info) {
    const auto now = std::chrono::steady_clock::now();
    const auto uptime =
        now >= runtime_info.started_at
            ? std::chrono::duration_cast<std::chrono::seconds>(now - runtime_info.started_at).count()
            : 0;

    return "version=" + std::string{version} +
           " protocol=" + runtime_info.protocol +
           " uptime_seconds=" + std::to_string(uptime) +
           " live_keys=" + std::to_string(live_keys) +
           " wal_enabled=" + format_yes_no(wal != nullptr) +
           " metrics_enabled=" + format_yes_no(runtime_info.metrics_enabled) +
           " max_request_bytes=" + std::to_string(runtime_info.max_request_bytes);
}

std::string format_info_json(std::size_t live_keys,
                             WriteAheadLog* wal,
                             const CommandRuntimeInfo& runtime_info) {
    const auto now = std::chrono::steady_clock::now();
    const auto uptime =
        now >= runtime_info.started_at
            ? std::chrono::duration_cast<std::chrono::seconds>(now - runtime_info.started_at).count()
            : 0;

    return "{\"schema_version\":" + std::to_string(runtime_identity_schema_version) +
           ",\"read_only\":true,\"execution_allowed\":false,\"order_authoritative\":false" +
           ",\"evidence_type\":\"runtime_identity\"" +
           ",\"version\":" + json_string(version) +
           ",\"server\":{\"protocol\":" + format_protocol_json_array(runtime_info.protocol) +
           ",\"uptime_seconds\":" + std::to_string(uptime) +
           ",\"max_request_bytes\":" + std::to_string(runtime_info.max_request_bytes) +
           "},\"store\":{\"live_keys\":" + std::to_string(live_keys) +
           "},\"wal\":{\"enabled\":" + format_json_bool(wal != nullptr) +
           "},\"metrics\":{\"enabled\":" + format_json_bool(runtime_info.metrics_enabled) +
           "},\"ci_evidence\":" + runtime_evidence_receipts::format_runtime_ci_evidence_hint_json() +
           ",\"artifact_retention\":" +
           runtime_evidence_receipts::format_runtime_artifact_retention_evidence_json() +
           ",\"binary_provenance\":" +
           runtime_evidence_receipts::format_runtime_binary_provenance_hint_json() +
           ",\"retention_provenance_check\":" +
           runtime_evidence_receipts::format_runtime_retention_provenance_check_json() +
           ",\"retention_provenance_replay_marker\":" +
           runtime_evidence_receipts::format_runtime_retention_provenance_replay_marker_json() +
           ",\"managed_audit_adapter_restore_boundary_receipt\":" +
           managed_audit_receipts::format_restore_boundary_receipt_json() +
           ",\"managed_audit_adapter_non_authoritative_storage_receipt\":" +
           managed_audit_receipts::format_non_authoritative_storage_receipt_json() +
           ",\"command_dispatch_quality_receipt\":" +
           managed_audit_receipts::format_command_dispatch_quality_receipt_json() +
           ",\"adapter_shell_non_storage_guard_receipt\":" +
           managed_audit_receipts::format_adapter_shell_non_storage_guard_receipt_json() +
           ",\"managed_audit_external_adapter_non_participation_receipt\":" +
           managed_audit_receipts::format_external_adapter_non_participation_receipt_json() +
           ",\"managed_audit_sandbox_adapter_non_participation_receipt\":" +
           managed_audit_receipts::format_sandbox_adapter_non_participation_receipt_json() +
           ",\"managed_audit_sandbox_connection_receipt_echo_marker\":" +
           managed_audit_receipts::format_sandbox_connection_receipt_echo_marker_json() +
           ",\"diagnostics\":{\"write_commands_executed\":false,\"dynamic_fields\":[\"server.uptime_seconds\"]}}";
}

std::string format_stats_json(std::size_t live_keys,
                              WriteAheadLog* wal,
                              const std::optional<WalMaintenanceReport>& wal_report,
                              const CommandProcessorMetrics& command_metrics,
                              const CommandConnectionStats& stats) {
    std::string response = "{\"schema_version\":" + std::to_string(runtime_metrics_schema_version) +
                           ",\"read_only\":true,\"execution_allowed\":false,\"order_authoritative\":false" +
                           ",\"evidence_type\":\"runtime_metrics\"" +
                           ",\"live_keys\":" + std::to_string(live_keys) +
                           ",\"wal_enabled\":" + format_json_bool(wal != nullptr);

    if (wal_report.has_value()) {
        response += ",\"wal\":{\"bytes\":" + std::to_string(wal_report->bytes) +
                    ",\"records\":" + std::to_string(wal_report->records) +
                    ",\"live_keys\":" + std::to_string(wal_report->live_keys) +
                    ",\"compact_recommended\":" + format_json_bool(wal_report->compact_recommended) +
                    ",\"compact_min_records\":" + std::to_string(wal_report->options.compact_min_records) +
                    ",\"compact_record_ratio\":" + std::to_string(wal_report->options.compact_record_ratio) +
                    ",\"compact_min_bytes\":" + std::to_string(wal_report->options.compact_min_bytes) +
                    ",\"manual_compactions\":" + std::to_string(wal_report->compaction_stats.manual_compactions) +
                    ",\"auto_compactions\":" + std::to_string(wal_report->compaction_stats.auto_compactions) +
                    ",\"repair_compactions\":" + std::to_string(wal_report->compaction_stats.repair_compactions) +
                    ",\"compacted_keys\":" + std::to_string(wal_report->compaction_stats.compacted_keys) +
                    ",\"records_removed\":" + std::to_string(wal_report->compaction_stats.records_removed) +
                    ",\"bytes_saved\":" + std::to_string(wal_report->compaction_stats.bytes_saved) + "}";
    } else {
        response += ",\"wal\":null";
    }

    response += "," + format_command_metrics_json(command_metrics);
    response += ",\"connection_stats\":{\"available\":" + format_json_bool(stats.available);
    if (stats.available) {
        response += ",\"active_connections\":" + std::to_string(stats.active_connections) +
                    ",\"total_connections\":" + std::to_string(stats.total_connections) +
                    ",\"peak_connections\":" + std::to_string(stats.peak_connections);
    }
    response += "},\"diagnostics\":{\"write_commands_executed\":false,"
                "\"dynamic_fields\":[\"commands.total_latency_ns\",\"commands.avg_latency_ns\","
                "\"commands.max_latency_ns\",\"commands.breakdown[*].*_latency_ns\"]}}";
    return response;
}

std::string format_smoke_json(std::size_t live_keys,
                              WriteAheadLog* wal,
                              const std::optional<WalMaintenanceReport>& wal_report,
                              const CommandProcessorMetrics& command_metrics,
                              const CommandConnectionStats& stats,
                              const CommandRuntimeInfo& runtime_info) {
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const std::vector<std::string> forbidden_commands = {
        "LOAD",
        "COMPACT",
        "SETNXEX",
        "RESTORE",
    };
    const std::vector<std::string> dynamic_fields = {
        "server.uptime_seconds",
        "commands.total_latency_ns",
        "commands.avg_latency_ns",
        "commands.max_latency_ns",
        "commands.breakdown[*].*_latency_ns",
    };
    const std::vector<std::string> notes = {
        "runtime_smoke_evidence",
        "live_read_session_hint",
        "binary_provenance_hint",
        "retention_provenance_check",
        "retention_provenance_replay_marker",
        "managed_audit_adapter_restore_boundary_receipt",
        "managed_audit_adapter_non_authoritative_storage_receipt",
        "command_dispatch_quality_receipt",
        "adapter_shell_non_storage_guard_receipt",
        "managed_audit_external_adapter_non_participation_receipt",
        "managed_audit_sandbox_adapter_non_participation_receipt",
        "managed_audit_sandbox_connection_receipt_echo_marker",
        "read_only_aggregate",
        "not_order_authoritative",
        "does_not_execute_load_compact_setnxex_or_restore",
    };

    const auto now = std::chrono::steady_clock::now();
    const auto uptime =
        now >= runtime_info.started_at
            ? std::chrono::duration_cast<std::chrono::seconds>(now - runtime_info.started_at).count()
            : 0;

    std::string response = "{\"schema_version\":" + std::to_string(runtime_smoke_schema_version) +
                           ",\"read_only\":true,\"execution_allowed\":false" +
                           ",\"restore_execution_allowed\":false,\"order_authoritative\":false" +
                           ",\"evidence_type\":\"runtime_smoke\"" +
                           ",\"version\":" + json_string(version) +
                           ",\"server\":{\"protocol\":" + format_protocol_json_array(runtime_info.protocol) +
                           ",\"uptime_seconds\":" + std::to_string(uptime) +
                           ",\"max_request_bytes\":" + std::to_string(runtime_info.max_request_bytes) +
                           ",\"metrics_enabled\":" + format_json_bool(runtime_info.metrics_enabled) + "}" +
                           ",\"store\":{\"live_keys\":" + std::to_string(live_keys) +
                           ",\"order_authoritative\":false}" +
                           ",\"wal\":{\"enabled\":" + format_json_bool(wal != nullptr);

    if (wal_report.has_value()) {
        response += ",\"status\":\"enabled\",\"compact_recommended\":" +
                    format_json_bool(wal_report->compact_recommended) +
                    ",\"bytes\":" + std::to_string(wal_report->bytes) +
                    ",\"records\":" + std::to_string(wal_report->records) + "}";
    } else {
        response += ",\"status\":\"disabled\",\"compact_recommended\":false}";
    }

    response += "," + format_command_metrics_json(command_metrics);
    response += ",\"connection_stats\":{\"available\":" + format_json_bool(stats.available);
    if (stats.available) {
        response += ",\"active_connections\":" + std::to_string(stats.active_connections) +
                    ",\"total_connections\":" + std::to_string(stats.total_connections) +
                    ",\"peak_connections\":" + std::to_string(stats.peak_connections);
    }
    response += "},\"real_read\":{\"allowed\":true,\"commands\":" +
                format_json_string_array(read_commands) +
                ",\"forbidden_commands\":" + format_json_string_array(forbidden_commands) +
                ",\"write_commands_executed\":false,\"admin_commands_executed\":false," +
                "\"runtime_write_observed\":false}" +
                ",\"live_read_session\":" +
                runtime_evidence_receipts::format_live_read_session_hint_json(uptime, read_commands) +
                ",\"operator_window\":" +
                runtime_evidence_receipts::format_smoke_operator_window_proof_json() +
                ",\"ci_evidence\":" + runtime_evidence_receipts::format_runtime_ci_evidence_hint_json() +
                ",\"artifact_retention\":" +
                runtime_evidence_receipts::format_runtime_artifact_retention_evidence_json() +
                ",\"binary_provenance\":" +
                runtime_evidence_receipts::format_runtime_binary_provenance_hint_json() +
                ",\"retention_provenance_check\":" +
                runtime_evidence_receipts::format_runtime_retention_provenance_check_json() +
                ",\"retention_provenance_replay_marker\":" +
                runtime_evidence_receipts::format_runtime_retention_provenance_replay_marker_json() +
                ",\"managed_audit_adapter_restore_boundary_receipt\":" +
                managed_audit_receipts::format_restore_boundary_receipt_json() +
                ",\"managed_audit_adapter_non_authoritative_storage_receipt\":" +
                managed_audit_receipts::format_non_authoritative_storage_receipt_json() +
                ",\"command_dispatch_quality_receipt\":" +
                managed_audit_receipts::format_command_dispatch_quality_receipt_json() +
                ",\"adapter_shell_non_storage_guard_receipt\":" +
                managed_audit_receipts::format_adapter_shell_non_storage_guard_receipt_json() +
                ",\"managed_audit_external_adapter_non_participation_receipt\":" +
                managed_audit_receipts::format_external_adapter_non_participation_receipt_json() +
                ",\"managed_audit_sandbox_adapter_non_participation_receipt\":" +
                managed_audit_receipts::format_sandbox_adapter_non_participation_receipt_json() +
                ",\"managed_audit_sandbox_connection_receipt_echo_marker\":" +
                managed_audit_receipts::format_sandbox_connection_receipt_echo_marker_json() +
                ",\"failure_taxonomy\":" + runtime_evidence_receipts::format_smoke_failure_taxonomy_json() +
                ",\"diagnostics\":{\"node_consumption\":\"Node v229 may verify the mini-kv sandbox connection receipt echo marker, the v95 sandbox adapter non-participation receipt, the v90 external adapter non-participation receipt, the v89 adapter shell non-storage guard receipt, the v88 command dispatch quality receipt, the v87 managed audit adapter non-authoritative storage receipt, the v86 managed audit adapter restore boundary receipt, runtime evidence retention, binary provenance digest alignment, live-read session echo, uptime bucket, read command digest, taxonomy digest, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence before manual sandbox connection packet verification; mini-kv must already be running and the read-only window must be open\"," +
                "\"dynamic_fields\":" + format_json_string_array(dynamic_fields) +
                ",\"notes\":" + format_json_string_array(notes) + "}}";
    return response;
}

std::string format_storage_evidence_json(std::size_t live_keys,
                                          WriteAheadLog* wal,
                                          const std::optional<WalMaintenanceReport>& wal_report) {
    const std::vector<std::string> side_effects = {
        "metadata_read",
        "store_read",
        "wal_metadata_read_when_enabled",
    };
    std::vector<std::string> notes = {
        "read_only_storage_evidence",
        "not_order_authoritative",
        "manual_snapshot_only",
    };
    notes.push_back(wal != nullptr ? "wal_enabled" : "wal_disabled");

    std::string response = "{\"schema_version\":" + std::to_string(storage_evidence_schema_version) +
                           ",\"read_only\":true,\"execution_allowed\":false" +
                           ",\"version\":" + json_string(version) +
                           ",\"store\":{\"live_keys\":" + std::to_string(live_keys) +
                           ",\"order_authoritative\":false}" +
                           ",\"wal\":{\"enabled\":" + format_json_bool(wal != nullptr);

    if (wal_report.has_value()) {
        response += ",\"status\":\"enabled\"" +
                    std::string{",\"bytes\":"} + std::to_string(wal_report->bytes) +
                    ",\"records\":" + std::to_string(wal_report->records) +
                    ",\"live_keys\":" + std::to_string(wal_report->live_keys) +
                    ",\"compact_recommended\":" + format_json_bool(wal_report->compact_recommended) +
                    ",\"options\":{\"compact_min_records\":" +
                    std::to_string(wal_report->options.compact_min_records) +
                    ",\"compact_record_ratio\":" + std::to_string(wal_report->options.compact_record_ratio) +
                    ",\"compact_min_bytes\":" + std::to_string(wal_report->options.compact_min_bytes) + "}" +
                    ",\"compactions\":{\"manual\":" +
                    std::to_string(wal_report->compaction_stats.manual_compactions) +
                    ",\"automatic\":" + std::to_string(wal_report->compaction_stats.auto_compactions) +
                    ",\"repair\":" + std::to_string(wal_report->compaction_stats.repair_compactions) +
                    ",\"compacted_keys\":" + std::to_string(wal_report->compaction_stats.compacted_keys) +
                    ",\"records_removed\":" + std::to_string(wal_report->compaction_stats.records_removed) +
                    ",\"bytes_saved\":" + std::to_string(wal_report->compaction_stats.bytes_saved) + "}";
    } else {
        response += ",\"status\":\"disabled\"";
    }

    response += "},\"snapshot\":{\"supported\":true,\"mode\":\"manual\",\"atomic_save\":true,"
                "\"load_replaces_store\":true}" +
                std::string{",\"side_effects\":"} + format_json_string_array(side_effects) +
                ",\"side_effect_count\":" + std::to_string(side_effects.size()) +
                ",\"diagnostics\":{\"read_only_command\":true,\"write_commands_executed\":false,"
                "\"order_authoritative\":false,\"notes\":" +
                format_json_string_array(notes) + "}}";
    return response;
}

} // namespace command_response_formatters
} // namespace minikv
