#include "minikv/command.hpp"

#include "minikv/snapshot.hpp"
#include "minikv/version.hpp"
#include "minikv/wal.hpp"

#include <chrono>
#include <cctype>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

namespace minikv {
namespace {

std::string trim_copy(std::string_view text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())) != 0) {
        text.remove_prefix(1);
    }

    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())) != 0) {
        text.remove_suffix(1);
    }

    return std::string{text};
}

std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

std::string command_token(std::string_view text) {
    std::istringstream input{std::string{text}};
    std::string command;
    input >> command;
    return to_upper(command);
}

struct CommandCatalogEntry {
    std::string_view name;
    std::string_view category;
    bool mutates_store;
    bool touches_wal;
    bool stable;
    std::string_view description;
};

enum class CommandDispatchFamily {
    Meta,
    Read,
    Write,
    Admin,
    RuntimeEvidence,
    Session,
    Unknown,
};

constexpr CommandCatalogEntry command_catalog[] = {
    {"PING", "meta", false, false, true, "Liveness check with optional echo message"},
    {"SET", "write", true, true, true, "Set or update a key value"},
    {"SETNXEX", "write", true, true, true, "Claim a key with value and TTL only when absent"},
    {"GET", "read", false, false, true, "Read a key value"},
    {"DEL", "write", true, true, true, "Delete a key"},
    {"EXPIRE", "write", true, true, true, "Set a positive TTL on an existing key"},
    {"TTL", "read", false, false, true, "Read a key TTL state"},
    {"SIZE", "read", false, false, true, "Return the live key count"},
    {"KEYS", "read", false, false, true, "List live keys, optionally filtered by prefix"},
    {"KEYSJSON", "read", false, false, true, "List live keys as JSON, optionally filtered by prefix"},
    {"SAVE", "admin", false, false, true, "Save a snapshot file"},
    {"LOAD", "admin", true, false, true, "Load a snapshot file into the store"},
    {"COMPACT", "admin", false, true, true, "Compact the write-ahead log"},
    {"WALINFO", "read", false, true, true, "Read write-ahead log maintenance details"},
    {"STATS", "meta", false, true, true, "Read command and connection metrics as text"},
    {"STATSJSON", "meta", false, true, true, "Read command and connection metrics as JSON"},
    {"SMOKEJSON", "meta", false, true, true, "Read combined runtime smoke evidence as JSON"},
    {"RESETSTATS", "admin", false, false, true, "Reset command metrics"},
    {"HEALTH", "meta", false, true, true, "Read liveness and maintenance health"},
    {"INFO", "meta", false, false, true, "Read server identity metadata as text"},
    {"INFOJSON", "meta", false, false, true, "Read server identity metadata as JSON"},
    {"COMMANDS", "meta", false, false, true, "Read command catalog as text"},
    {"COMMANDSJSON", "meta", false, false, true, "Read command catalog as JSON"},
    {"EXPLAINJSON", "meta", false, false, true, "Explain a command risk profile as JSON without executing it"},
    {"CHECKJSON", "meta", false, false, true, "Read a write command execution contract as JSON without executing it"},
    {"STORAGEJSON", "read", false, true, true, "Read storage evidence as JSON without mutating data"},
    {"HELP", "meta", false, false, true, "Show command help"},
    {"EXIT", "meta", false, false, true, "Close the current client session"},
    {"QUIT", "meta", false, false, true, "Close the current client session"},
};

const CommandCatalogEntry* find_command_catalog_entry(std::string_view command) {
    for (const auto& entry : command_catalog) {
        if (entry.name == command) {
            return &entry;
        }
    }
    return nullptr;
}

CommandDispatchFamily dispatch_family_for(std::string_view command) {
    if (command == "STATS" || command == "STATSJSON" || command == "SMOKEJSON" ||
        command == "STORAGEJSON" || command == "HEALTH" || command == "INFO" ||
        command == "INFOJSON" || command == "COMMANDS" || command == "COMMANDSJSON") {
        return CommandDispatchFamily::RuntimeEvidence;
    }
    if (command == "EXIT" || command == "QUIT") {
        return CommandDispatchFamily::Session;
    }

    const auto* entry = find_command_catalog_entry(command);
    if (entry == nullptr) {
        return CommandDispatchFamily::Unknown;
    }
    if (entry->category == "write") {
        return CommandDispatchFamily::Write;
    }
    if (entry->category == "admin") {
        return CommandDispatchFamily::Admin;
    }
    if (entry->category == "read") {
        return CommandDispatchFamily::Read;
    }
    return CommandDispatchFamily::Meta;
}

bool is_known_command(std::string_view command) {
    return find_command_catalog_entry(command) != nullptr;
}

std::string metrics_command_name(std::string_view command) {
    std::string name = to_upper(command);
    if (name.empty() || !is_known_command(name)) {
        return "UNKNOWN";
    }
    return name;
}

bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}

CommandResult usage(std::string_view command) {
    return {std::string{"ERR usage: "} + std::string{command}};
}

CommandResult wal_error() {
    return {"ERR wal append failed"};
}

CommandResult wal_disabled_error() {
    return {"ERR WAL not enabled"};
}

CommandResult wal_compact_error() {
    return {"ERR wal compact failed"};
}

CommandResult snapshot_error(std::string_view action) {
    return {std::string{"ERR snapshot "} + std::string{action} + " failed"};
}

std::mutex& wal_command_mutex() {
    static std::mutex mutex;
    return mutex;
}

std::optional<std::chrono::seconds> parse_positive_seconds(std::string_view text) {
    std::istringstream input{std::string{text}};
    long long seconds = 0;
    char extra = '\0';

    if (!(input >> seconds) || seconds <= 0 || (input >> extra)) {
        return std::nullopt;
    }

    return std::chrono::seconds{seconds};
}

std::string format_expires_at(std::string_view key, Store::TimePoint expires_at) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"EXPIREAT "} + std::string{key} + " " + std::to_string(epoch_millis);
}

std::string format_setex_at(std::string_view key, Store::TimePoint expires_at, std::string_view value) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"SETEXAT "} + std::string{key} + " " + std::to_string(epoch_millis) + " " + std::string{value};
}

std::string format_yes_no(bool value) {
    return value ? "yes" : "no";
}

std::string format_json_bool(bool value) {
    return value ? "true" : "false";
}

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

constexpr std::size_t key_inventory_limit = 1000;
constexpr int explain_schema_version = 1;
constexpr int runtime_identity_schema_version = 1;
constexpr int runtime_metrics_schema_version = 1;
constexpr int runtime_smoke_schema_version = 1;
constexpr int storage_evidence_schema_version = 1;
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

std::string format_commands() {
    std::string response = "command_count=" + std::to_string(std::size(command_catalog)) + " commands=";
    bool first = true;
    for (const auto& entry : command_catalog) {
        if (!first) {
            response.push_back(';');
        }
        first = false;
        response += std::string{entry.name} + "(" +
                    "category=" + std::string{entry.category} +
                    ",mutates_store=" + format_yes_no(entry.mutates_store) +
                    ",touches_wal=" + format_yes_no(entry.touches_wal) +
                    ",stable=" + format_yes_no(entry.stable) + ")";
    }
    return response;
}

std::string format_commands_json() {
    std::string response = "{\"commands\":[";
    bool first = true;
    for (const auto& entry : command_catalog) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"name\":" + json_string(entry.name) +
                    ",\"category\":" + json_string(entry.category) +
                    ",\"mutates_store\":" + format_json_bool(entry.mutates_store) +
                    ",\"touches_wal\":" + format_json_bool(entry.touches_wal) +
                    ",\"stable\":" + format_json_bool(entry.stable) +
                    ",\"description\":" + json_string(entry.description) + "}";
    }
    response += "]}";
    return response;
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

struct SmokeFailureTaxonomyEntry {
    std::string_view id;
    std::string_view source;
    std::string_view meaning;
    std::string_view node_action;
    bool retryable;
    bool write_risk;
};

constexpr SmokeFailureTaxonomyEntry smoke_failure_taxonomy_entries[] = {
    {"closed-window",
     "operator_window",
     "The real-read adapter window is disabled, so Node should skip mini-kv reads.",
     "Keep the rehearsal skipped until an operator opens the read-only window.",
     true,
     false},
    {"connection-refused",
     "tcp_connect",
     "mini-kv is not listening or the operator has not started it for the window.",
     "Report upstream_unreachable and do not auto-start mini-kv.",
     true,
     false},
    {"timeout",
     "tcp_read",
     "mini-kv did not return the read-only response within the adapter timeout.",
     "Record timeout evidence and keep commands read-only.",
     true,
     false},
    {"invalid-json",
     "SMOKEJSON_parse",
     "The adapter did not receive parseable JSON from SMOKEJSON.",
     "Classify the result before trusting runtime fields.",
     false,
     false},
    {"read-command-failed",
     "runtime_read_command",
     "SMOKEJSON, INFOJSON, STORAGEJSON, HEALTH, or STATSJSON returned an error.",
     "Record the failing command and stop the read window.",
     false,
     false},
    {"unsafe-surface",
     "adapter_command_plan",
     "The adapter attempted LOAD, COMPACT, SETNXEX, RESTORE, or another forbidden command.",
     "Block the rehearsal and require plan review.",
     false,
     true},
    {"unexpected-write-signal",
     "runtime_smoke_diagnostics",
     "Runtime evidence reports write/admin execution or a token created by smoke.",
     "Treat the rehearsal as failed and do not publish pass evidence.",
     false,
     true},
};

struct SmokeOperatorWindowProof {
    std::string_view consumer;
    std::string_view node_action;
    bool identity_neutral_proof;
};

constexpr SmokeOperatorWindowProof smoke_operator_window_proof = {
    "Node v200 real-read window CI archive artifact manifest",
    "verify digest before importing manual window results",
    true,
};

struct RuntimeCiEvidenceHint {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view node_action;
    bool identity_neutral_proof;
    bool no_restore_proof;
    bool upload_allowed;
};

constexpr RuntimeCiEvidenceHint runtime_ci_evidence_hint = {
    "Node v201 real-read window CI artifact manifest verification",
    "c/80/",
    "verify manifest path hints before CI artifact upload dry-run",
    true,
    true,
    false,
};

struct RuntimeArtifactRetentionEvidence {
    std::string_view consumer;
    std::string_view artifact_root;
    std::string_view artifact_path_hint;
    std::string_view retention_mode;
    std::string_view node_action;
    int retention_days;
    bool release_evidence_ready;
    bool github_artifact_upload_attempted;
    bool production_window_allowed;
};

constexpr RuntimeArtifactRetentionEvidence runtime_artifact_retention_evidence = {
    "Node v203 cross-project CI artifact retention gate",
    "c/",
    "c/81/",
    "dry-run-contract-only",
    "verify retention days and artifact path before cross-project retention gate",
    30,
    true,
    false,
    false,
};

struct RuntimeBinaryProvenanceHint {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view runtime_binary_hint;
    std::string_view release_manifest_path;
    std::string_view runtime_smoke_evidence_path;
    std::string_view node_action;
    bool read_only;
    bool load_restore_compact_executed;
    bool production_binary_claimed;
};

constexpr RuntimeBinaryProvenanceHint runtime_binary_provenance_hint = {
    "Node v208 managed audit persistence boundary candidate",
    "c/89/",
    "cmake-build-v89/minikv_server and cmake-build-v89/minikv_client from the current CMake build",
    "fixtures/release/verification-manifest.json",
    "fixtures/release/runtime-smoke-evidence.json",
    "verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work",
    true,
    false,
    false,
};

struct RuntimeRetentionProvenanceCheck {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view release_manifest_path;
    std::string_view runtime_smoke_evidence_path;
    std::string_view retention_source_path_hint;
    std::string_view provenance_source;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool managed_audit_write_executed;
};

constexpr RuntimeRetentionProvenanceCheck runtime_retention_provenance_check = {
    "Node v211 managed audit identity approval provenance dry-run packet",
    "c/89/",
    "fixtures/release/verification-manifest.json",
    "fixtures/release/runtime-smoke-evidence.json",
    "c/81/",
    "binary_provenance.provenance_digest",
    "verify runtime evidence retention and binary provenance alignment before managed audit dry-run packet work",
    true,
    false,
    false,
};

struct RuntimeRetentionProvenanceReplayMarker {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_check_digest;
    std::string_view current_artifact_path_hint;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool replay_executed;
    bool managed_audit_write_executed;
};

constexpr RuntimeRetentionProvenanceReplayMarker runtime_retention_provenance_replay_marker = {
    "Node v213 managed audit packet restore drill plan",
    "Node v211 managed audit identity approval provenance dry-run packet",
    "v84",
    "c/84/",
    "fnv1a64:357cc7e9eec3f223",
    "c/89/",
    "verify v84 retention provenance consumption before managed audit packet restore drill planning",
    true,
    false,
    false,
    false,
};

struct RuntimeManagedAuditAdapterRestoreBoundaryReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_marker_digest;
    std::string_view current_artifact_path_hint;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool adapter_write_allowed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool managed_audit_write_executed;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditAdapterRestoreBoundaryReceipt
    runtime_managed_audit_adapter_restore_boundary_receipt = {
        "Node v215 managed audit dry-run adapter candidate",
        "Node v213 managed audit packet restore drill plan",
        "v85",
        "c/85/",
        "fnv1a64:1ea4570c967cfdb1",
        "c/89/",
        "mini-kv remains a read-only evidence provider for managed audit adapter preparation",
        "verify mini-kv restore/write boundary before managed audit dry-run adapter candidate work",
        true,
        false,
        false,
        false,
        false,
        false,
        false,
    };

struct RuntimeManagedAuditAdapterNonAuthoritativeStorageReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool managed_audit_store;
    bool storage_write_allowed;
    bool admin_commands_allowed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool managed_audit_write_executed;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditAdapterNonAuthoritativeStorageReceipt
    runtime_managed_audit_adapter_non_authoritative_storage_receipt = {
        "Node v217 managed audit adapter production-hardening readiness gate",
        "Node v215 managed audit dry-run adapter candidate",
        "v86",
        "c/86/",
        "fnv1a64:f39d8e3ef98654ea",
        "c/89/",
        "mini-kv is non-authoritative read-only storage evidence, not the managed audit store",
        "verify mini-kv storage is non-authoritative before managed audit adapter production-hardening readiness gate",
        true,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
    };

struct RuntimeCommandDispatchQualityReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view dispatch_family;
    std::string_view split_commands;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool dispatch_split_applied;
    bool handler_table_required;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool wal_snapshot_restore_touched;
    bool behavior_changed;
    bool fixture_contract_preserved;
    bool order_authoritative;
};

constexpr RuntimeCommandDispatchQualityReceipt runtime_command_dispatch_quality_receipt = {
    "Node v219 managed audit adapter implementation precheck packet",
    "Node v217 managed audit adapter production-hardening readiness gate",
    "v87",
    "c/87/",
    "fnv1a64:111f0daf1283eab6",
    "c/89/",
    "runtime_evidence_command_family",
    "STATS,STATSJSON,SMOKEJSON,STORAGEJSON,HEALTH,INFO,INFOJSON,COMMANDS,COMMANDSJSON",
    "command dispatch quality receipt only; no write, admin, WAL, snapshot, restore, or managed audit storage behavior change",
    "verify mini-kv command dispatch read-only quality before managed audit adapter implementation precheck",
    true,
    false,
    true,
    false,
    false,
    false,
    false,
    false,
    true,
    false,
};

struct RuntimeAdapterShellNonStorageGuardReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view adapter_shell;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool adapter_shell_storage_backend;
    bool storage_backend_allowed;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool managed_audit_write_executed;
    bool local_dry_run_records_written;
    bool order_authoritative;
};

constexpr RuntimeAdapterShellNonStorageGuardReceipt
    runtime_adapter_shell_non_storage_guard_receipt = {
        "Node v221 managed audit local adapter candidate dry-run",
        "Node v220 managed audit adapter interface and disabled shell",
        "v88",
        "c/88/",
        "fnv1a64:4aa6d12fb067e2a6",
        "c/89/",
        "ManagedAuditAdapter disabled shell",
        "adapter shell non-storage guard receipt only; mini-kv remains runtime evidence and is not a managed audit adapter storage backend",
        "verify mini-kv is not the adapter shell storage backend before local adapter candidate dry-run",
        true,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
    };

std::uint64_t fnv1a64(std::string_view text);
std::string format_hex64(std::uint64_t value);
void append_digest_part(std::string& source, std::string_view value);

std::string read_command_list_digest(const std::vector<std::string>& commands) {
    std::string source;
    append_digest_part(source, "mini-kv-read-command-list");
    append_digest_part(source, std::to_string(commands.size()));
    for (const auto& command : commands) {
        append_digest_part(source, command);
    }
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

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

std::string format_live_read_session_hint_json(std::int64_t uptime_seconds,
                                               const std::vector<std::string>& read_commands) {
    return "{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\","
           "\"session_id_echo\":\"mini-kv-live-read-v89\","
           "\"server_uptime_bucket\":" + json_string(uptime_bucket_for_seconds(uptime_seconds)) +
           ",\"read_command_list_digest\":" + json_string(read_command_list_digest(read_commands)) +
           ",\"read_command_count\":" + std::to_string(read_commands.size()) +
           ",\"read_commands\":" + format_json_string_array(read_commands) +
           ",\"write_commands_allowed\":false,\"auto_start_allowed\":false,"
           "\"node_action\":\"verify session echo, uptime bucket, and read command digest before real-read execution packet\"}";
}

std::string smoke_failure_taxonomy_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-smoke-failure-taxonomy");
    append_digest_part(source, "1");
    append_digest_part(source, "Node v196 imported window result packet");
    append_digest_part(source, std::to_string(std::size(smoke_failure_taxonomy_entries)));
    for (const auto& entry : smoke_failure_taxonomy_entries) {
        append_digest_part(source, entry.id);
        append_digest_part(source, entry.source);
        append_digest_part(source, entry.meaning);
        append_digest_part(source, entry.node_action);
        append_digest_part(source, format_json_bool(entry.retryable));
        append_digest_part(source, "false");
        append_digest_part(source, format_json_bool(entry.write_risk));
    }
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_smoke_failure_taxonomy_json() {
    const auto digest = smoke_failure_taxonomy_digest();

    std::string response =
        "{\"schema_version\":1,\"consumer\":\"Node v196 imported window result packet\","
        "\"taxonomy_digest\":" + json_string(digest) +
        ",\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\","
        "\"source_command\":\"SMOKEJSON\",\"source_version\":" + json_string(version) +
        ","
        "\"expected_taxonomy_digest\":" + json_string(digest) +
        ",\"read_only\":true,\"execution_allowed\":false,\"restore_execution_allowed\":false,"
        "\"order_authoritative\":false,\"node_action\":\"verify digest before importing manual window results\"},"
        "\"categories\":[";
    for (std::size_t index = 0; index < std::size(smoke_failure_taxonomy_entries); ++index) {
        if (index != 0) {
            response += ",";
        }
        const auto& entry = smoke_failure_taxonomy_entries[index];
        response += "{\"id\":" + json_string(entry.id) +
                    ",\"source\":" + json_string(entry.source) +
                    ",\"meaning\":" + json_string(entry.meaning) +
                    ",\"node_action\":" + json_string(entry.node_action) +
                    ",\"retryable\":" + format_json_bool(entry.retryable) +
                    ",\"safe_to_auto_start\":false" +
                    ",\"write_risk\":" + format_json_bool(entry.write_risk) + "}";
    }
    response += "]}";
    return response;
}

std::string format_smoke_operator_window_proof_json() {
    return "{\"consumer\":" + json_string(smoke_operator_window_proof.consumer) +
           ",\"identity_neutral_proof\":" +
           format_json_bool(smoke_operator_window_proof.identity_neutral_proof) +
           ",\"node_action\":" + json_string(smoke_operator_window_proof.node_action) + "}";
}

std::string format_runtime_ci_evidence_hint_json() {
    return "{\"consumer\":" + json_string(runtime_ci_evidence_hint.consumer) +
           ",\"artifact_path_hint\":" + json_string(runtime_ci_evidence_hint.artifact_path_hint) +
           ",\"identity_neutral_proof\":" +
           format_json_bool(runtime_ci_evidence_hint.identity_neutral_proof) +
           ",\"no_restore_proof\":" + format_json_bool(runtime_ci_evidence_hint.no_restore_proof) +
           ",\"upload_allowed\":" + format_json_bool(runtime_ci_evidence_hint.upload_allowed) +
           ",\"node_action\":" + json_string(runtime_ci_evidence_hint.node_action) + "}";
}

std::string format_runtime_artifact_retention_evidence_json() {
    return "{\"consumer\":" + json_string(runtime_artifact_retention_evidence.consumer) +
           ",\"artifact_root\":" + json_string(runtime_artifact_retention_evidence.artifact_root) +
           ",\"artifact_path_hint\":" +
           json_string(runtime_artifact_retention_evidence.artifact_path_hint) +
           ",\"retention_days\":" +
           std::to_string(runtime_artifact_retention_evidence.retention_days) +
           ",\"retention_mode\":" +
           json_string(runtime_artifact_retention_evidence.retention_mode) +
           ",\"release_evidence_ready\":" +
           format_json_bool(runtime_artifact_retention_evidence.release_evidence_ready) +
           ",\"github_artifact_upload_attempted\":" +
           format_json_bool(runtime_artifact_retention_evidence.github_artifact_upload_attempted) +
           ",\"production_window_allowed\":" +
           format_json_bool(runtime_artifact_retention_evidence.production_window_allowed) +
           ",\"node_action\":" +
           json_string(runtime_artifact_retention_evidence.node_action) + "}";
}

std::string binary_provenance_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-binary-provenance");
    append_digest_part(source, version);
    append_digest_part(source, runtime_binary_provenance_hint.artifact_path_hint);
    append_digest_part(source, runtime_binary_provenance_hint.runtime_binary_hint);
    append_digest_part(source, runtime_binary_provenance_hint.release_manifest_path);
    append_digest_part(source, runtime_binary_provenance_hint.runtime_smoke_evidence_path);
    append_digest_part(source, format_json_bool(runtime_binary_provenance_hint.load_restore_compact_executed));
    append_digest_part(source, format_json_bool(runtime_binary_provenance_hint.production_binary_claimed));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_binary_provenance_hint_json() {
    return "{\"consumer\":" + json_string(runtime_binary_provenance_hint.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"artifact_path_hint\":" + json_string(runtime_binary_provenance_hint.artifact_path_hint) +
           ",\"runtime_binary_hint\":" + json_string(runtime_binary_provenance_hint.runtime_binary_hint) +
           ",\"release_manifest_path\":" + json_string(runtime_binary_provenance_hint.release_manifest_path) +
           ",\"runtime_smoke_evidence_path\":" +
           json_string(runtime_binary_provenance_hint.runtime_smoke_evidence_path) +
           ",\"provenance_digest\":" + json_string(binary_provenance_digest()) +
           ",\"read_only\":" + format_json_bool(runtime_binary_provenance_hint.read_only) +
           ",\"load_restore_compact_executed\":" +
           format_json_bool(runtime_binary_provenance_hint.load_restore_compact_executed) +
           ",\"production_binary_claimed\":" +
           format_json_bool(runtime_binary_provenance_hint.production_binary_claimed) +
           ",\"node_action\":" + json_string(runtime_binary_provenance_hint.node_action) + "}";
}

std::string retention_provenance_check_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-retention-provenance-check");
    append_digest_part(source, version);
    append_digest_part(source, runtime_retention_provenance_check.artifact_path_hint);
    append_digest_part(source, runtime_retention_provenance_check.release_manifest_path);
    append_digest_part(source, runtime_retention_provenance_check.runtime_smoke_evidence_path);
    append_digest_part(source, runtime_retention_provenance_check.retention_source_path_hint);
    append_digest_part(source, binary_provenance_digest());
    append_digest_part(source, format_json_bool(runtime_retention_provenance_check.read_only));
    append_digest_part(source, format_json_bool(runtime_retention_provenance_check.execution_allowed));
    append_digest_part(source, format_json_bool(runtime_retention_provenance_check.managed_audit_write_executed));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_retention_provenance_check_json() {
    return "{\"consumer\":" + json_string(runtime_retention_provenance_check.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"artifact_path_hint\":" +
           json_string(runtime_retention_provenance_check.artifact_path_hint) +
           ",\"release_manifest_path\":" +
           json_string(runtime_retention_provenance_check.release_manifest_path) +
           ",\"runtime_smoke_evidence_path\":" +
           json_string(runtime_retention_provenance_check.runtime_smoke_evidence_path) +
           ",\"retention_source_path_hint\":" +
           json_string(runtime_retention_provenance_check.retention_source_path_hint) +
           ",\"provenance_source\":" +
           json_string(runtime_retention_provenance_check.provenance_source) +
           ",\"expected_binary_provenance_digest\":" + json_string(binary_provenance_digest()) +
           ",\"check_digest\":" + json_string(retention_provenance_check_digest()) +
           ",\"read_only\":" + format_json_bool(runtime_retention_provenance_check.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(runtime_retention_provenance_check.execution_allowed) +
           ",\"managed_audit_write_executed\":" +
           format_json_bool(runtime_retention_provenance_check.managed_audit_write_executed) +
           ",\"node_action\":" + json_string(runtime_retention_provenance_check.node_action) + "}";
}

std::string retention_provenance_replay_marker_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-retention-provenance-replay-marker");
    append_digest_part(source, version);
    append_digest_part(source, runtime_retention_provenance_replay_marker.consumed_by);
    append_digest_part(source, runtime_retention_provenance_replay_marker.consumed_release_version);
    append_digest_part(source, runtime_retention_provenance_replay_marker.consumed_artifact_path_hint);
    append_digest_part(source, runtime_retention_provenance_replay_marker.consumed_check_digest);
    append_digest_part(source, runtime_retention_provenance_replay_marker.current_artifact_path_hint);
    append_digest_part(source, format_json_bool(runtime_retention_provenance_replay_marker.read_only));
    append_digest_part(source, format_json_bool(runtime_retention_provenance_replay_marker.execution_allowed));
    append_digest_part(source, format_json_bool(runtime_retention_provenance_replay_marker.replay_executed));
    append_digest_part(source, format_json_bool(runtime_retention_provenance_replay_marker.managed_audit_write_executed));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_retention_provenance_replay_marker_json() {
    return "{\"consumer\":" + json_string(runtime_retention_provenance_replay_marker.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"consumed_by\":" + json_string(runtime_retention_provenance_replay_marker.consumed_by) +
           ",\"consumed_release_version\":" +
           json_string(runtime_retention_provenance_replay_marker.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           json_string(runtime_retention_provenance_replay_marker.consumed_artifact_path_hint) +
           ",\"consumed_check_digest\":" +
           json_string(runtime_retention_provenance_replay_marker.consumed_check_digest) +
           ",\"current_artifact_path_hint\":" +
           json_string(runtime_retention_provenance_replay_marker.current_artifact_path_hint) +
           ",\"marker_digest\":" + json_string(retention_provenance_replay_marker_digest()) +
           ",\"read_only\":" + format_json_bool(runtime_retention_provenance_replay_marker.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(runtime_retention_provenance_replay_marker.execution_allowed) +
           ",\"replay_executed\":" +
           format_json_bool(runtime_retention_provenance_replay_marker.replay_executed) +
           ",\"managed_audit_write_executed\":" +
           format_json_bool(runtime_retention_provenance_replay_marker.managed_audit_write_executed) +
           ",\"node_action\":" + json_string(runtime_retention_provenance_replay_marker.node_action) + "}";
}

std::string managed_audit_adapter_restore_boundary_receipt_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-managed-audit-adapter-restore-boundary-receipt");
    append_digest_part(source, version);
    append_digest_part(source, runtime_managed_audit_adapter_restore_boundary_receipt.consumed_by);
    append_digest_part(source, runtime_managed_audit_adapter_restore_boundary_receipt.consumed_release_version);
    append_digest_part(source, runtime_managed_audit_adapter_restore_boundary_receipt.consumed_artifact_path_hint);
    append_digest_part(source, runtime_managed_audit_adapter_restore_boundary_receipt.consumed_marker_digest);
    append_digest_part(source, runtime_managed_audit_adapter_restore_boundary_receipt.current_artifact_path_hint);
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.read_only));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.execution_allowed));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.adapter_write_allowed));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.restore_execution_allowed));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.load_restore_compact_executed));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.managed_audit_write_executed));
    append_digest_part(source, format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.order_authoritative));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_managed_audit_adapter_restore_boundary_receipt_json() {
    return "{\"consumer\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"consumed_by\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.consumed_by) +
           ",\"consumed_release_version\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.consumed_artifact_path_hint) +
           ",\"consumed_marker_digest\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.consumed_marker_digest) +
           ",\"current_artifact_path_hint\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.current_artifact_path_hint) +
           ",\"receipt_digest\":" +
           json_string(managed_audit_adapter_restore_boundary_receipt_digest()) +
           ",\"read_only\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.execution_allowed) +
           ",\"adapter_write_allowed\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.adapter_write_allowed) +
           ",\"restore_execution_allowed\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.restore_execution_allowed) +
           ",\"load_restore_compact_executed\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.load_restore_compact_executed) +
           ",\"managed_audit_write_executed\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.managed_audit_write_executed) +
           ",\"order_authoritative\":" +
           format_json_bool(runtime_managed_audit_adapter_restore_boundary_receipt.order_authoritative) +
           ",\"boundary\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.boundary) +
           ",\"node_action\":" +
           json_string(runtime_managed_audit_adapter_restore_boundary_receipt.node_action) + "}";
}

std::string managed_audit_adapter_non_authoritative_storage_receipt_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-managed-audit-adapter-non-authoritative-storage-receipt");
    append_digest_part(source, version);
    append_digest_part(source,
                       runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_by);
    append_digest_part(
        source,
        runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_release_version);
    append_digest_part(
        source,
        runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_artifact_path_hint);
    append_digest_part(
        source,
        runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_receipt_digest);
    append_digest_part(
        source,
        runtime_managed_audit_adapter_non_authoritative_storage_receipt.current_artifact_path_hint);
    append_digest_part(
        source, format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt.read_only));
    append_digest_part(source,
                       format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                                            .execution_allowed));
    append_digest_part(
        source,
        format_json_bool(
            runtime_managed_audit_adapter_non_authoritative_storage_receipt.managed_audit_store));
    append_digest_part(
        source,
        format_json_bool(
            runtime_managed_audit_adapter_non_authoritative_storage_receipt.storage_write_allowed));
    append_digest_part(
        source,
        format_json_bool(
            runtime_managed_audit_adapter_non_authoritative_storage_receipt.admin_commands_allowed));
    append_digest_part(
        source,
        format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                             .restore_execution_allowed));
    append_digest_part(
        source,
        format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                             .load_restore_compact_executed));
    append_digest_part(
        source,
        format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                             .managed_audit_write_executed));
    append_digest_part(source,
                       format_json_bool(
                           runtime_managed_audit_adapter_non_authoritative_storage_receipt
                               .order_authoritative));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_managed_audit_adapter_non_authoritative_storage_receipt_json() {
    return "{\"consumer\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"consumed_by\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_by) +
           ",\"consumed_release_version\":" +
           json_string(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                           .consumed_artifact_path_hint) +
           ",\"consumed_receipt_digest\":" +
           json_string(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.consumed_receipt_digest) +
           ",\"current_artifact_path_hint\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                           .current_artifact_path_hint) +
           ",\"receipt_digest\":" +
           json_string(managed_audit_adapter_non_authoritative_storage_receipt_digest()) +
           ",\"read_only\":" +
           format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.execution_allowed) +
           ",\"managed_audit_store\":" +
           format_json_bool(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.managed_audit_store) +
           ",\"storage_write_allowed\":" +
           format_json_bool(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.storage_write_allowed) +
           ",\"admin_commands_allowed\":" +
           format_json_bool(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.admin_commands_allowed) +
           ",\"restore_execution_allowed\":" +
           format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                                .restore_execution_allowed) +
           ",\"load_restore_compact_executed\":" +
           format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                                .load_restore_compact_executed) +
           ",\"managed_audit_write_executed\":" +
           format_json_bool(runtime_managed_audit_adapter_non_authoritative_storage_receipt
                                .managed_audit_write_executed) +
           ",\"order_authoritative\":" +
           format_json_bool(
               runtime_managed_audit_adapter_non_authoritative_storage_receipt.order_authoritative) +
           ",\"boundary\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt.boundary) +
           ",\"node_action\":" +
           json_string(runtime_managed_audit_adapter_non_authoritative_storage_receipt.node_action) + "}";
}

std::string command_dispatch_quality_receipt_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-command-dispatch-quality-receipt");
    append_digest_part(source, version);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.consumed_by);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.consumed_release_version);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.consumed_artifact_path_hint);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.consumed_receipt_digest);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.current_artifact_path_hint);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.dispatch_family);
    append_digest_part(source, runtime_command_dispatch_quality_receipt.split_commands);
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.read_only));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.execution_allowed));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.dispatch_split_applied));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.handler_table_required));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.write_handler_changed));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.admin_handler_changed));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.wal_snapshot_restore_touched));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.behavior_changed));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.fixture_contract_preserved));
    append_digest_part(source, format_json_bool(runtime_command_dispatch_quality_receipt.order_authoritative));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_command_dispatch_quality_receipt_json() {
    return "{\"consumer\":" + json_string(runtime_command_dispatch_quality_receipt.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"consumed_by\":" + json_string(runtime_command_dispatch_quality_receipt.consumed_by) +
           ",\"consumed_release_version\":" +
           json_string(runtime_command_dispatch_quality_receipt.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           json_string(runtime_command_dispatch_quality_receipt.consumed_artifact_path_hint) +
           ",\"consumed_receipt_digest\":" +
           json_string(runtime_command_dispatch_quality_receipt.consumed_receipt_digest) +
           ",\"current_artifact_path_hint\":" +
           json_string(runtime_command_dispatch_quality_receipt.current_artifact_path_hint) +
           ",\"dispatch_family\":" + json_string(runtime_command_dispatch_quality_receipt.dispatch_family) +
           ",\"split_commands\":" + json_string(runtime_command_dispatch_quality_receipt.split_commands) +
           ",\"receipt_digest\":" + json_string(command_dispatch_quality_receipt_digest()) +
           ",\"read_only\":" + format_json_bool(runtime_command_dispatch_quality_receipt.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.execution_allowed) +
           ",\"dispatch_split_applied\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.dispatch_split_applied) +
           ",\"handler_table_required\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.handler_table_required) +
           ",\"write_handler_changed\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.write_handler_changed) +
           ",\"admin_handler_changed\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.admin_handler_changed) +
           ",\"wal_snapshot_restore_touched\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.wal_snapshot_restore_touched) +
           ",\"behavior_changed\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.behavior_changed) +
           ",\"fixture_contract_preserved\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.fixture_contract_preserved) +
           ",\"order_authoritative\":" +
           format_json_bool(runtime_command_dispatch_quality_receipt.order_authoritative) +
           ",\"boundary\":" + json_string(runtime_command_dispatch_quality_receipt.boundary) +
           ",\"node_action\":" + json_string(runtime_command_dispatch_quality_receipt.node_action) + "}";
}

std::string adapter_shell_non_storage_guard_receipt_digest() {
    std::string source;
    append_digest_part(source, "mini-kv-adapter-shell-non-storage-guard-receipt");
    append_digest_part(source, version);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.consumed_by);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.consumed_release_version);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.consumed_artifact_path_hint);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.consumed_receipt_digest);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.current_artifact_path_hint);
    append_digest_part(source, runtime_adapter_shell_non_storage_guard_receipt.adapter_shell);
    append_digest_part(source, format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.read_only));
    append_digest_part(source, format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.execution_allowed));
    append_digest_part(
        source,
        format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.adapter_shell_storage_backend));
    append_digest_part(source,
                       format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.storage_backend_allowed));
    append_digest_part(source, format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.write_handler_changed));
    append_digest_part(source, format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.admin_handler_changed));
    append_digest_part(source,
                       format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.restore_execution_allowed));
    append_digest_part(source,
                       format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.load_restore_compact_executed));
    append_digest_part(source,
                       format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.managed_audit_write_executed));
    append_digest_part(source,
                       format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.local_dry_run_records_written));
    append_digest_part(source, format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.order_authoritative));
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_runtime_adapter_shell_non_storage_guard_receipt_json() {
    return "{\"consumer\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.consumer) +
           ",\"source_version\":" + json_string(version) +
           ",\"consumed_by\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.consumed_by) +
           ",\"consumed_release_version\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.consumed_artifact_path_hint) +
           ",\"consumed_receipt_digest\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.consumed_receipt_digest) +
           ",\"current_artifact_path_hint\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.current_artifact_path_hint) +
           ",\"adapter_shell\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.adapter_shell) +
           ",\"receipt_digest\":" +
           json_string(adapter_shell_non_storage_guard_receipt_digest()) +
           ",\"read_only\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.read_only) +
           ",\"execution_allowed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.execution_allowed) +
           ",\"adapter_shell_storage_backend\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.adapter_shell_storage_backend) +
           ",\"storage_backend_allowed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.storage_backend_allowed) +
           ",\"write_handler_changed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.write_handler_changed) +
           ",\"admin_handler_changed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.admin_handler_changed) +
           ",\"restore_execution_allowed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.restore_execution_allowed) +
           ",\"load_restore_compact_executed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.load_restore_compact_executed) +
           ",\"managed_audit_write_executed\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.managed_audit_write_executed) +
           ",\"local_dry_run_records_written\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.local_dry_run_records_written) +
           ",\"order_authoritative\":" +
           format_json_bool(runtime_adapter_shell_non_storage_guard_receipt.order_authoritative) +
           ",\"boundary\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.boundary) +
           ",\"node_action\":" +
           json_string(runtime_adapter_shell_non_storage_guard_receipt.node_action) + "}";
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

void append_digest_part(std::string& source, std::string_view value) {
    source += std::to_string(value.size());
    source.push_back(':');
    source += value;
    source.push_back(';');
}

struct CommandExplain {
    std::string command;
    std::string category = "unknown";
    bool mutates_store = false;
    bool touches_wal = false;
    std::optional<std::string> key;
    bool requires_value = false;
    bool ttl_sensitive = false;
    bool allowed_by_parser = false;
    std::vector<std::string> side_effects;
    std::vector<std::string> warnings;
};

std::string command_digest(const CommandExplain& explain) {
    std::string source;
    append_digest_part(source, std::to_string(explain_schema_version));
    append_digest_part(source, explain.command);
    append_digest_part(source, explain.category);
    append_digest_part(source, format_json_bool(explain.mutates_store));
    append_digest_part(source, format_json_bool(explain.touches_wal));
    append_digest_part(source, explain.key.value_or(""));
    append_digest_part(source, explain.key.has_value() ? "present" : "null");
    append_digest_part(source, format_json_bool(explain.requires_value));
    append_digest_part(source, format_json_bool(explain.ttl_sensitive));
    append_digest_part(source, format_json_bool(explain.allowed_by_parser));
    append_digest_part(source, std::to_string(explain.side_effects.size()));
    for (const auto& side_effect : explain.side_effects) {
        append_digest_part(source, side_effect);
    }
    append_digest_part(source, std::to_string(explain.warnings.size()));
    for (const auto& warning : explain.warnings) {
        append_digest_part(source, warning);
    }
    return "fnv1a64:" + format_hex64(fnv1a64(source));
}

std::string format_explain_json(const CommandExplain& explain) {
    std::string response = "{\"schema_version\":" + std::to_string(explain_schema_version) +
                           ",\"command_digest\":" + json_string(command_digest(explain)) +
                           ",\"command\":" + json_string(explain.command) +
                           ",\"category\":" + json_string(explain.category) +
                           ",\"mutates_store\":" + format_json_bool(explain.mutates_store) +
                           ",\"touches_wal\":" + format_json_bool(explain.touches_wal) +
                           ",\"key\":";
    if (explain.key.has_value()) {
        response += json_string(*explain.key);
    } else {
        response += "null";
    }
    response += ",\"requires_value\":" + format_json_bool(explain.requires_value) +
                ",\"ttl_sensitive\":" + format_json_bool(explain.ttl_sensitive) +
                ",\"allowed_by_parser\":" + format_json_bool(explain.allowed_by_parser) +
                ",\"side_effects\":" + format_json_string_array(explain.side_effects) +
                ",\"side_effect_count\":" + std::to_string(explain.side_effects.size()) +
                ",\"warnings\":" + format_json_string_array(explain.warnings) + "}";
    return response;
}

std::string execution_contract_durability(const CommandExplain& explain, bool wal_enabled) {
    if (!explain.mutates_store) {
        return "not_applicable";
    }
    if (!explain.touches_wal) {
        return "not_wal_backed";
    }
    return wal_enabled ? "wal_backed" : "memory_only";
}

std::vector<std::string> execution_contract_warnings(const CommandExplain& explain, bool wal_enabled) {
    std::vector<std::string> warnings = explain.warnings;
    if (!explain.mutates_store) {
        warnings.push_back("not a write command");
    } else if (!explain.touches_wal) {
        warnings.push_back("write command is not wal-backed");
    } else if (!wal_enabled) {
        warnings.push_back("wal disabled; write would be in-memory only");
    }
    return warnings;
}

std::string format_check_json(const CommandExplain& explain, bool wal_enabled) {
    const auto warnings = execution_contract_warnings(explain, wal_enabled);
    std::string response = "{\"schema_version\":" + std::to_string(explain_schema_version) +
                           ",\"read_only\":true,\"execution_allowed\":false" +
                           ",\"command_digest\":" + json_string(command_digest(explain)) +
                           ",\"command\":" + json_string(explain.command) +
                           ",\"write_command\":" + format_json_bool(explain.mutates_store) +
                           ",\"allowed_by_parser\":" + format_json_bool(explain.allowed_by_parser) +
                           ",\"side_effects\":" + format_json_string_array(explain.side_effects) +
                           ",\"side_effect_count\":" + std::to_string(explain.side_effects.size()) +
                           ",\"checks\":{\"parser_allowed\":" + format_json_bool(explain.allowed_by_parser) +
                           ",\"write_command\":" + format_json_bool(explain.mutates_store) +
                           ",\"wal_append_when_enabled\":" + format_json_bool(explain.touches_wal) +
                           ",\"wal_enabled\":" + format_json_bool(wal_enabled) + "}" +
                           ",\"wal\":{\"enabled\":" + format_json_bool(wal_enabled) +
                           ",\"touches_wal\":" + format_json_bool(explain.touches_wal) +
                           ",\"append_when_enabled\":" + format_json_bool(explain.touches_wal) +
                           ",\"durability\":" + json_string(execution_contract_durability(explain, wal_enabled)) + "}" +
                           ",\"warnings\":" + format_json_string_array(warnings) + "}";
    return response;
}

void mark_usage_warning(CommandExplain& explain, std::string_view command_usage) {
    explain.allowed_by_parser = false;
    explain.warnings.push_back(std::string{"usage: "} + std::string{command_usage});
}

CommandExplain explain_command(std::string_view line) {
    CommandExplain explain;
    const std::string trimmed = trim_copy(line);
    std::istringstream input{trimmed};
    input >> explain.command;
    explain.command = to_upper(explain.command);

    if (explain.command.empty()) {
        explain.warnings.push_back("missing command");
        return explain;
    }

    const auto* entry = find_command_catalog_entry(explain.command);
    if (entry == nullptr) {
        explain.warnings.push_back("unknown command");
        return explain;
    }

    explain.category = std::string{entry->category};
    explain.mutates_store = entry->mutates_store;
    explain.touches_wal = entry->touches_wal;
    explain.requires_value = explain.command == "SET" || explain.command == "SETNXEX";
    explain.ttl_sensitive = explain.command == "EXPIRE" || explain.command == "TTL" || explain.command == "SETNXEX";
    explain.allowed_by_parser = true;

    if (explain.command == "PING") {
        explain.side_effects.push_back("metadata_read");
        return explain;
    }

    if (explain.command == "SET") {
        explain.side_effects.push_back("store_write");
        explain.side_effects.push_back("wal_append_when_enabled");
        std::string key;
        input >> key;
        std::string value;
        std::getline(input >> std::ws, value);
        if (!key.empty()) {
            explain.key = key;
        }
        if (key.empty() || value.empty()) {
            mark_usage_warning(explain, "SET key value");
        }
        return explain;
    }

    if (explain.command == "SETNXEX") {
        explain.side_effects.push_back("store_write");
        explain.side_effects.push_back("store_ttl_update");
        explain.side_effects.push_back("wal_append_when_enabled");
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;
        std::string value;
        std::getline(input >> std::ws, value);
        if (!key.empty()) {
            explain.key = key;
        }
        if (key.empty() || !parse_positive_seconds(seconds_text).has_value() || value.empty()) {
            mark_usage_warning(explain, "SETNXEX key seconds value");
        }
        return explain;
    }

    if (explain.command == "GET" || explain.command == "DEL" || explain.command == "TTL") {
        if (explain.command == "GET" || explain.command == "TTL") {
            explain.side_effects.push_back("store_read");
        } else {
            explain.side_effects.push_back("store_write");
            explain.side_effects.push_back("wal_append_when_enabled");
        }
        std::string key;
        input >> key;
        if (!key.empty()) {
            explain.key = key;
        }
        if (key.empty() || has_extra_token(input)) {
            mark_usage_warning(explain, explain.command + " key");
        }
        return explain;
    }

    if (explain.command == "EXPIRE") {
        explain.side_effects.push_back("store_ttl_update");
        explain.side_effects.push_back("wal_append_when_enabled");
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;
        if (!key.empty()) {
            explain.key = key;
        }
        if (key.empty() || !parse_positive_seconds(seconds_text).has_value() || has_extra_token(input)) {
            mark_usage_warning(explain, "EXPIRE key seconds");
        }
        return explain;
    }

    if (explain.command == "KEYS" || explain.command == "KEYSJSON") {
        explain.side_effects.push_back("store_read");
        std::string prefix;
        if (input >> prefix && has_extra_token(input)) {
            mark_usage_warning(explain, explain.command + " [prefix]");
        }
        return explain;
    }

    if (explain.command == "SAVE" || explain.command == "LOAD") {
        if (explain.command == "SAVE") {
            explain.side_effects.push_back("snapshot_file_write");
        } else {
            explain.side_effects.push_back("store_replace_from_snapshot");
        }
        std::string path;
        std::getline(input >> std::ws, path);
        if (path.empty()) {
            mark_usage_warning(explain, explain.command + " path");
        }
        return explain;
    }

    if (explain.command == "EXPLAINJSON" || explain.command == "CHECKJSON") {
        explain.side_effects.push_back("metadata_read");
        std::string nested_command;
        input >> nested_command;
        if (nested_command.empty()) {
            mark_usage_warning(explain, explain.command + " command");
        }
        return explain;
    }

    if (explain.command == "STORAGEJSON" || explain.command == "SMOKEJSON") {
        explain.side_effects.push_back("metadata_read");
        explain.side_effects.push_back("store_read");
        explain.side_effects.push_back("wal_metadata_read_when_enabled");
        if (has_extra_token(input)) {
            mark_usage_warning(explain, explain.command);
        }
        return explain;
    }

    if (explain.command == "COMPACT") {
        explain.side_effects.push_back("wal_rewrite_when_enabled");
    } else if (explain.command == "RESETSTATS") {
        explain.side_effects.push_back("metrics_reset");
    } else if (explain.command == "EXIT" || explain.command == "QUIT") {
        explain.side_effects.push_back("connection_close");
    } else if (explain.category == "read") {
        explain.side_effects.push_back("metadata_read");
    } else {
        explain.side_effects.push_back("metadata_read");
    }

    if (has_extra_token(input)) {
        mark_usage_warning(explain, explain.command);
    }
    return explain;
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

CommandConnectionStats connection_stats(const CommandProcessorOptions& options) {
    if (options.connection_stats) {
        return options.connection_stats();
    }
    return {};
}

std::string format_connection_stats(const CommandConnectionStats& stats) {
    if (!stats.available) {
        return " connection_stats_available=no";
    }

    return " connection_stats_available=yes active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
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
           "},\"ci_evidence\":" + format_runtime_ci_evidence_hint_json() +
           ",\"artifact_retention\":" + format_runtime_artifact_retention_evidence_json() +
           ",\"binary_provenance\":" + format_runtime_binary_provenance_hint_json() +
           ",\"retention_provenance_check\":" + format_runtime_retention_provenance_check_json() +
           ",\"retention_provenance_replay_marker\":" +
           format_runtime_retention_provenance_replay_marker_json() +
           ",\"managed_audit_adapter_restore_boundary_receipt\":" +
           format_runtime_managed_audit_adapter_restore_boundary_receipt_json() +
           ",\"managed_audit_adapter_non_authoritative_storage_receipt\":" +
           format_runtime_managed_audit_adapter_non_authoritative_storage_receipt_json() +
           ",\"command_dispatch_quality_receipt\":" +
           format_runtime_command_dispatch_quality_receipt_json() +
           ",\"adapter_shell_non_storage_guard_receipt\":" +
           format_runtime_adapter_shell_non_storage_guard_receipt_json() +
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
                ",\"live_read_session\":" + format_live_read_session_hint_json(uptime, read_commands) +
                ",\"operator_window\":" + format_smoke_operator_window_proof_json() +
                ",\"ci_evidence\":" + format_runtime_ci_evidence_hint_json() +
                ",\"artifact_retention\":" + format_runtime_artifact_retention_evidence_json() +
                ",\"binary_provenance\":" + format_runtime_binary_provenance_hint_json() +
                ",\"retention_provenance_check\":" + format_runtime_retention_provenance_check_json() +
                ",\"retention_provenance_replay_marker\":" +
                format_runtime_retention_provenance_replay_marker_json() +
                ",\"managed_audit_adapter_restore_boundary_receipt\":" +
                format_runtime_managed_audit_adapter_restore_boundary_receipt_json() +
                ",\"managed_audit_adapter_non_authoritative_storage_receipt\":" +
                format_runtime_managed_audit_adapter_non_authoritative_storage_receipt_json() +
                ",\"command_dispatch_quality_receipt\":" +
                format_runtime_command_dispatch_quality_receipt_json() +
                ",\"adapter_shell_non_storage_guard_receipt\":" +
                format_runtime_adapter_shell_non_storage_guard_receipt_json() +
                ",\"failure_taxonomy\":" + format_smoke_failure_taxonomy_json() +
                ",\"diagnostics\":{\"node_consumption\":\"Node v221 may verify the mini-kv adapter shell non-storage guard receipt, the v88 command dispatch quality receipt, the v87 managed audit adapter non-authoritative storage receipt, the v86 managed audit adapter restore boundary receipt, runtime evidence retention, binary provenance digest alignment, live-read session echo, uptime bucket, read command digest, taxonomy digest, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence before managed audit local adapter candidate dry-run; mini-kv must already be running and the read-only window must be open\"," +
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

void CommandMetricsTracker::record(const CommandResult& result) {
    record("UNKNOWN", result, 0);
}

void CommandMetricsTracker::record(std::string_view command, const CommandResult& result, std::uint64_t elapsed_ns) {
    const std::string bucket_name = metrics_command_name(command);
    const bool is_error = result.response.rfind("ERR ", 0) == 0;
    if (elapsed_ns == 0) {
        elapsed_ns = 1;
    }

    std::lock_guard lock{mutex_};
    ++totals_.total_commands;
    totals_.total_latency_ns += elapsed_ns;
    if (elapsed_ns > totals_.max_latency_ns) {
        totals_.max_latency_ns = elapsed_ns;
    }

    auto& bucket = breakdown_[bucket_name];
    bucket.command = bucket_name;
    ++bucket.total_commands;
    bucket.total_latency_ns += elapsed_ns;
    if (elapsed_ns > bucket.max_latency_ns) {
        bucket.max_latency_ns = elapsed_ns;
    }

    if (is_error) {
        ++totals_.error_commands;
        ++bucket.error_commands;
        return;
    }

    ++totals_.successful_commands;
    ++bucket.successful_commands;
}

void CommandMetricsTracker::reset() {
    std::lock_guard lock{mutex_};
    totals_ = {};
    breakdown_.clear();
}

CommandProcessorMetrics CommandMetricsTracker::stats() const {
    std::lock_guard lock{mutex_};
    CommandProcessorMetrics snapshot = totals_;
    snapshot.command_breakdown.clear();
    snapshot.command_breakdown.reserve(breakdown_.size());
    for (const auto& [_, metrics] : breakdown_) {
        snapshot.command_breakdown.push_back(metrics);
    }
    return snapshot;
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
        result += command_metrics.command + ":" + std::to_string(command_metrics.total_commands) + "/" +
                  std::to_string(command_metrics.successful_commands) + "/" +
                  std::to_string(command_metrics.error_commands) + "/" +
                  std::to_string(command_metrics.total_latency_ns) + "/" +
                  std::to_string(command_average_latency_ns) + "/" +
                  std::to_string(command_metrics.max_latency_ns);
    }

    return result;
}

CommandProcessor::CommandProcessor(Store& store, WriteAheadLog* wal, CommandProcessorOptions options)
    : store_(store),
      wal_(wal),
      options_(std::move(options)),
      metrics_tracker_(options_.metrics_tracker ? options_.metrics_tracker : std::make_shared<CommandMetricsTracker>()) {
}

void CommandProcessor::auto_compact_wal_if_needed() {
    if (wal_ == nullptr || !options_.auto_compact_wal) {
        return;
    }

    (void)wal_->compact_if_recommended(store_);
}

CommandResult CommandProcessor::execute_runtime_evidence_command(std::string_view command, std::istringstream& input) {
    if (command == "STATS") {
        if (has_extra_token(input)) {
            return usage("STATS");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_stats(live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
    }

    if (command == "STATSJSON") {
        if (has_extra_token(input)) {
            return usage("STATSJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_stats_json(live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
    }

    if (command == "SMOKEJSON") {
        if (has_extra_token(input)) {
            return usage("SMOKEJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_smoke_json(
            live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_), options_.runtime_info)};
    }

    if (command == "STORAGEJSON") {
        if (has_extra_token(input)) {
            return usage("STORAGEJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_storage_evidence_json(live_keys, wal_, wal_report)};
    }

    if (command == "HEALTH") {
        if (has_extra_token(input)) {
            return usage("HEALTH");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_health(live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
    }

    if (command == "INFO") {
        if (has_extra_token(input)) {
            return usage("INFO");
        }

        return {format_info(store_.size(), wal_, options_.runtime_info)};
    }

    if (command == "INFOJSON") {
        if (has_extra_token(input)) {
            return usage("INFOJSON");
        }

        return {format_info_json(store_.size(), wal_, options_.runtime_info)};
    }

    if (command == "COMMANDS") {
        if (has_extra_token(input)) {
            return usage("COMMANDS");
        }

        return {format_commands()};
    }

    if (command == "COMMANDSJSON") {
        if (has_extra_token(input)) {
            return usage("COMMANDSJSON");
        }

        return {format_commands_json()};
    }

    return {"ERR unknown command"};
}

CommandResult CommandProcessor::execute(std::string_view line) {
    const std::string trimmed = trim_copy(line);
    if (trimmed.empty()) {
        return {};
    }

    const std::string command = command_token(trimmed);
    const auto started_at = std::chrono::steady_clock::now();
    CommandResult result = execute_trimmed(trimmed);
    const auto elapsed_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - started_at).count();
    const bool reset_succeeded = command == "RESETSTATS" && result.response == "OK stats reset";
    if (!reset_succeeded) {
        metrics_tracker_->record(command, result, static_cast<std::uint64_t>(elapsed_ns));
    }
    return result;
}

CommandProcessorMetrics CommandProcessor::metrics() const {
    return metrics_tracker_->stats();
}

CommandResult CommandProcessor::execute_trimmed(std::string_view trimmed) {
    std::istringstream input{std::string{trimmed}};

    std::string command;
    input >> command;
    command = to_upper(command);

    if (command == "PING") {
        std::string message;
        std::getline(input >> std::ws, message);

        return {message.empty() ? "PONG" : message};
    }

    if (command == "SET") {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return usage("SET key value");
        }

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!wal_->append(std::string{"SET "} + key + " " + value)) {
                return wal_error();
            }

            const bool inserted = store_.set(key, value);
            auto_compact_wal_if_needed();
            return {inserted ? "OK inserted" : "OK updated"};
        }

        const bool inserted = store_.set(key, value);
        return {inserted ? "OK inserted" : "OK updated"};
    }

    if (command == "SETNXEX") {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        std::string value;
        std::getline(input >> std::ws, value);

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || value.empty()) {
            return usage("SETNXEX key seconds value");
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (store_.contains(key)) {
                return {"0"};
            }

            if (!wal_->append(format_setex_at(key, expires_at, value))) {
                return wal_error();
            }

            const bool claimed = store_.set_if_absent(key, value, expires_at);
            auto_compact_wal_if_needed();
            return {claimed ? "1" : "0"};
        }

        return {store_.set_if_absent(key, value, expires_at) ? "1" : "0"};
    }

    if (command == "GET") {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("GET key");
        }

        const auto value = store_.get(key);
        if (!value.has_value()) {
            return {"(nil)"};
        }

        return {*value};
    }

    if (command == "DEL") {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("DEL key");
        }

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!store_.contains(key)) {
                return {"0"};
            }

            if (!wal_->append(std::string{"DEL "} + key)) {
                return wal_error();
            }

            const bool erased = store_.erase(key);
            auto_compact_wal_if_needed();
            return {erased ? "1" : "0"};
        }

        if (!store_.contains(key)) {
            return {"0"};
        }

        return {store_.erase(key) ? "1" : "0"};
    }

    if (command == "EXPIRE") {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
            return usage("EXPIRE key seconds");
        }

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!store_.contains(key)) {
                return {"0"};
            }

            const auto expires_at = Store::Clock::now() + *seconds;
            if (!wal_->append(format_expires_at(key, expires_at))) {
                return wal_error();
            }

            const bool updated = store_.expire_at(key, expires_at);
            auto_compact_wal_if_needed();
            return {updated ? "1" : "0"};
        }

        if (!store_.contains(key)) {
            return {"0"};
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return {store_.expire_at(key, expires_at) ? "1" : "0"};
    }

    if (command == "TTL") {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("TTL key");
        }

        const auto ttl = store_.ttl(key);
        if (!ttl.has_value()) {
            return {"-2"};
        }

        return {std::to_string(ttl->count())};
    }

    if (command == "SIZE") {
        if (has_extra_token(input)) {
            return usage("SIZE");
        }

        return {std::to_string(store_.size())};
    }

    if (command == "KEYS") {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYS [prefix]");
            }

            return {format_prefixed_keys(prefix, store_.keys_with_prefix(prefix))};
        }

        return {format_keys(store_.keys())};
    }

    if (command == "KEYSJSON") {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYSJSON [prefix]");
            }

            return {format_keys_json(prefix, store_.keys_with_prefix(prefix))};
        }

        return {format_keys_json(std::nullopt, store_.keys())};
    }

    if (command == "SAVE") {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("SAVE path");
        }

        std::size_t saved = 0;
        if (!SnapshotFile::save(store_, path, &saved)) {
            return snapshot_error("save");
        }

        return {std::string{"OK saved "} + std::to_string(saved)};
    }

    if (command == "LOAD") {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("LOAD path");
        }

        std::size_t loaded = 0;
        if (!SnapshotFile::load(store_, path, &loaded)) {
            return snapshot_error("load");
        }

        return {std::string{"OK loaded "} + std::to_string(loaded)};
    }

    if (command == "COMPACT") {
        if (has_extra_token(input)) {
            return usage("COMPACT");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::size_t compacted = 0;
        {
            std::lock_guard lock(wal_command_mutex());
            if (!wal_->compact(store_, &compacted)) {
                return wal_compact_error();
            }
        }

        return {std::string{"OK compacted "} + std::to_string(compacted)};
    }

    if (command == "WALINFO") {
        if (has_extra_token(input)) {
            return usage("WALINFO");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::lock_guard lock(wal_command_mutex());
        return {format_walinfo(wal_->maintenance_report(store_))};
    }

    if (command == "RESETSTATS") {
        if (has_extra_token(input)) {
            return usage("RESETSTATS");
        }

        metrics_tracker_->reset();
        return {"OK stats reset"};
    }

    if (dispatch_family_for(command) == CommandDispatchFamily::RuntimeEvidence) {
        return execute_runtime_evidence_command(command, input);
    }

    if (command == "EXPLAINJSON") {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("EXPLAINJSON command");
        }

        return {format_explain_json(explain_command(target_command))};
    }

    if (command == "CHECKJSON") {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("CHECKJSON command");
        }

        return {format_check_json(explain_command(target_command), wal_ != nullptr)};
    }

    if (command == "HELP") {
        if (has_extra_token(input)) {
            return usage("HELP");
        }

        return {help_text()};
    }

    if (command == "EXIT" || command == "QUIT") {
        if (has_extra_token(input)) {
            return usage("QUIT");
        }

        return {"BYE", true};
    }

    return {"ERR unknown command"};
}

std::string CommandProcessor::help_text() {
    return "Commands:\n"
           "  PING [message]\n"
           "  SET key value\n"
           "  SETNXEX key seconds value\n"
           "  GET key\n"
           "  DEL key\n"
           "  EXPIRE key seconds\n"
           "  TTL key\n"
           "  SIZE\n"
           "  KEYS [prefix]\n"
           "  KEYSJSON [prefix]\n"
           "  SAVE path\n"
           "  LOAD path\n"
           "  COMPACT\n"
           "  WALINFO\n"
           "  STATS\n"
           "  STATSJSON\n"
           "  SMOKEJSON\n"
           "  RESETSTATS\n"
           "  HEALTH\n"
           "  INFO\n"
           "  INFOJSON\n"
           "  COMMANDS\n"
           "  COMMANDSJSON\n"
           "  EXPLAINJSON command\n"
           "  CHECKJSON command\n"
           "  STORAGEJSON\n"
           "  HELP\n"
           "  EXIT\n"
           "  QUIT";
}

} // namespace minikv
