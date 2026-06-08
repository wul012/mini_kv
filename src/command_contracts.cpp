#include "minikv/command_contracts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/string_utils.hpp"

#include <cstdint>
#include <iomanip>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::command_contracts {
namespace {

constexpr int explain_schema_version = 1;
constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ull;
constexpr std::uint64_t fnv_prime = 1099511628211ull;

bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}

bool parse_positive_seconds(std::string_view text) {
    std::istringstream input{std::string{text}};
    long long seconds = 0;
    char extra = '\0';

    return (input >> seconds) && seconds > 0 && !(input >> extra);
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

std::string format_yes_no(bool value) {
    return value ? "yes" : "no";
}

std::string format_json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string format_json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

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
    {"SHARDJSON", "read", false, false, true, "Read shard readiness evidence as JSON without enabling shard writes"},
    {"SHARDROUTE", "read", false, false, true, "Preview a key slot and shard without enabling active routing"},
    {"SHARDROUTEJSON", "read", false, false, true, "Preview a key slot and shard as JSON without enabling active routing"},
    {"SHARDROUTEVERIFYJSON",
     "read",
     false,
     false,
     true,
     "Verify a route preview JSON contract without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTJSON",
     "read",
     false,
     false,
     true,
     "Package route preview verification evidence without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTCLOSEOUTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report closeout summary without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVEJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive manifest without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive closeout evidence without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive closeout verification evidence without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive closeout verification audit evidence without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive closeout verification audit closeout evidence without enabling active routing"},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview verification report archive closeout verification audit closeout archive verification evidence without enabling active routing"},
    {"SHARDROUTEARCHIVEMAINTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview archive maintenance ledger evidence without enabling active routing"},
    {"SHARDROUTEARCHIVEMAINTVERIFYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview archive maintenance verification evidence without enabling active routing"},
    {"SHARDROUTEWORKSHEETVERIFYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview worksheet verification evidence without accepting manual values or enabling active routing"},
    {"SHARDROUTEIMPORTPREFLIGHTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator evidence import preflight without importing values or enabling active routing"},
    {"SHARDROUTEVALUEDRAFTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator evidence value draft without accepting values or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply envelope without accepting values or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYPRECHECKJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply approval precheck without accepting values or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply approval packet template without accepting values or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval template without capturing signatures or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval capture preflight without capturing signatures or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval capture artifact preflight without materializing artifacts or enabling active routing"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft authoring readiness without generating draft text or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft instruction preflight without materializing draft instructions or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft text package review preflight without parsing packages or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft text package review closeout audit without parsing packages or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft text package comparison closeout audit without parsing packages or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON",
     "read",
     false,
     false,
     true,
     "Read route preview operator value-supply signed approval artifact draft text package compared package evidence intake audit without parsing packages or enabling execution"},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON",
     "read",
     false,
     false,
     true,
     "Read route preview candidate document request package closeout without creating documents or enabling execution"},
    {"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON",
     "read",
     false,
     false,
     true,
     "Read route preview candidate request package fixture integrity evidence without opening document intake"},
    {"COMMANDS", "meta", false, false, true, "Read command catalog as text"},
    {"COMMANDSJSON", "meta", false, false, true, "Read command catalog as JSON"},
    {"EXPLAINJSON", "meta", false, false, true, "Explain a command risk profile as JSON without executing it"},
    {"CHECKJSON", "meta", false, false, true, "Read a write command execution contract as JSON without executing it"},
    {"STORAGEJSON", "read", false, true, true, "Read storage evidence as JSON without mutating data"},
    {"HELP", "meta", false, false, true, "Show command help"},
    {"EXIT", "meta", false, false, true, "Close the current client session"},
    {"QUIT", "meta", false, false, true, "Close the current client session"},
};

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
        if (key.empty() || !parse_positive_seconds(seconds_text) || value.empty()) {
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
        if (key.empty() || !parse_positive_seconds(seconds_text) || has_extra_token(input)) {
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

    if (explain.command == "SHARDJSON") {
        explain.side_effects.push_back("metadata_read");
        if (has_extra_token(input)) {
            mark_usage_warning(explain, "SHARDJSON");
        }
        return explain;
    }

    if (explain.command == "SHARDROUTE" || explain.command == "SHARDROUTEJSON" ||
        explain.command == "SHARDROUTEVERIFYJSON" || explain.command == "SHARDROUTEVERIFYREPORTJSON") {
        explain.side_effects.push_back("metadata_read");
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

} // namespace

const CommandCatalogEntry* find_command_catalog_entry(std::string_view command) {
    for (const auto& entry : command_catalog) {
        if (entry.name == command) {
            return &entry;
        }
    }
    return nullptr;
}

bool is_known_command(std::string_view command) {
    return find_command_catalog_entry(command) != nullptr;
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

std::string format_explain_json(std::string_view command_line) {
    return format_explain_json(explain_command(command_line));
}

std::string format_check_json(std::string_view command_line, bool wal_enabled) {
    return format_check_json(explain_command(command_line), wal_enabled);
}

} // namespace minikv::command_contracts
