#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_contains(const std::string& text, std::string_view expected) {
    assert(text.find(std::string{expected}) != std::string::npos);
}

std::size_t count_occurrences(const std::string& text, std::string_view needle) {
    std::size_t count = 0;
    std::size_t offset = 0;
    const std::string value{needle};
    while (true) {
        const auto found = text.find(value, offset);
        if (found == std::string::npos) {
            return count;
        }
        ++count;
        offset = found + value.size();
    }
}

} // namespace

int main() {
    const auto index = read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "index.json");
    const auto infojson_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json");
    const auto statsjson_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "statsjson-empty-inline.json");
    const auto field_guide =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json");
    const auto checkjson_set_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json");
    const auto checkjson_get_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json");

    assert_contains(index, "\"index_version\":\"mini-kv-readonly-fixtures.v1\"");
    assert_contains(index, "\"read_only\":true");
    assert_contains(index, "\"execution_allowed\":false");
    assert_contains(index, "\"runtime_read_commands_allowed\":true");
    assert_contains(index, "\"write_execution_allowed\":false");
    assert_contains(index, "\"order_authoritative\":false");
    assert_contains(index,
                    "\"consumer_hint\":\"Node v159 read-only capture release evidence review preparation\"");
    assert_contains(index, "\"fixture_count\":5");
    assert(count_occurrences(index, "\"path\":") == 5);
    assert_contains(index, "\"command\":\"CHECKJSON SET orderops:1 value\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/set-orderops-write-contract.json\"");
    assert_contains(index, "\"command\":\"CHECKJSON GET orderops:1\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/get-orderops-read-contract.json\"");
    assert_contains(index, "\"command\":\"INFOJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(index, "\"sample_type\":\"runtime_identity\",\"schema_version\":1,"
                           "\"read_only\":true,\"execution_allowed\":false,"
                           "\"order_authoritative\":false");
    assert_contains(index, "\"dynamic_fields\":[\"server.uptime_seconds\"]");
    assert_contains(index, "\"command\":\"STATSJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/statsjson-empty-inline.json\"");
    assert_contains(index, "\"sample_type\":\"runtime_metrics\",\"schema_version\":1,"
                           "\"read_only\":true,\"execution_allowed\":false,"
                           "\"order_authoritative\":false");
    assert_contains(index, "\"id\":\"runtime-read-field-guide\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/runtime-read-field-guide.json\"");
    assert_contains(index, "\"sample_type\":\"field_guide\"");
    assert_contains(index, "\"explains\":[\"INFOJSON fields\",\"STATSJSON fields\","
                           "\"read-only smoke semantics\"]");
    assert_contains(index, "\"no write command executed\"");
    assert_contains(index, "INFOJSON and STATSJSON carry their own read-only diagnostics");
    assert_contains(index, "field guide is explanatory evidence, not a runtime endpoint");
    assert_contains(index, "external control planes must treat samples as shape evidence, not production pass evidence");
    assert_contains(index,
                    "runtime-read-field-guide.json explains dynamic fields and read-only smoke interpretation for Node v159");

    assert_contains(field_guide, "\"guide_version\": \"mini-kv-runtime-read-field-guide.v1\"");
    assert_contains(field_guide, "\"consumer_hint\": \"Node v159 read-only capture release evidence review\"");
    assert_contains(field_guide, "\"command\": \"INFOJSON\"");
    assert_contains(field_guide, "\"evidence_type\": \"runtime_identity\"");
    assert_contains(field_guide, "\"path\": \"server.uptime_seconds\"");
    assert_contains(field_guide, "\"comparison_rule\": \"Check type and presence; do not compare exact fixture value in live capture.\"");
    assert_contains(field_guide, "\"command\": \"STATSJSON\"");
    assert_contains(field_guide, "\"evidence_type\": \"runtime_metrics\"");
    assert_contains(field_guide, "\"path\": \"commands.total_latency_ns\"");
    assert_contains(field_guide, "\"allowed_commands\": [");
    assert_contains(field_guide, "\"CHECKJSON GET orderops:1\"");
    assert_contains(field_guide, "\"write_commands_not_used\": [");
    assert_contains(field_guide, "\"SET\"");
    assert_contains(field_guide, "\"DEL\"");
    assert_contains(field_guide, "\"EXPIRE\"");
    assert_contains(field_guide, "Skipped or mixed capture evidence is not a production pass.");
    assert_contains(field_guide, "not a new server endpoint");

    minikv::Store check_store;
    minikv::CommandProcessor check_processor{check_store};
    auto result = check_processor.execute("CHECKJSON SET orderops:1 value");
    assert(result.response == checkjson_set_fixture);
    result = check_processor.execute("CHECKJSON GET orderops:1");
    assert(result.response == checkjson_get_fixture);

    minikv::CommandProcessorOptions info_options;
    info_options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::Store info_store;
    minikv::CommandProcessor info_processor{info_store, nullptr, info_options};
    result = info_processor.execute("INFOJSON");
    assert(result.response == infojson_fixture);
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_identity\"");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal\":{\"enabled\":false}");
    assert_contains(result.response, "\"metrics\":{\"enabled\":false}");
    assert_contains(result.response, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"source_version\":\"0.97.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/97/\"");
    assert_contains(result.response, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(result.response, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:bc97d5dba7bf5130\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(result.response, "\"expected_binary_provenance_digest\":\"fnv1a64:bc97d5dba7bf5130\"");
    assert_contains(result.response, "\"check_digest\":\"fnv1a64:dfb8f391498358f3\"");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(result.response, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v84\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(result.response, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/97/\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:70a34f981a518759\"");
    assert_contains(result.response, "\"replay_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v85\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:efcbd2ffb50e96aa\"");
    assert_contains(result.response, "\"adapter_write_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:7074656bc886ed76\"");
    assert_contains(result.response, "\"managed_audit_store\":false");
    assert_contains(result.response, "\"storage_write_allowed\":false");
    assert_contains(result.response, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:637e56ca0a9c6536\"");
    assert_contains(result.response, "\"dispatch_split_applied\":true");
    assert_contains(result.response, "\"wal_snapshot_restore_touched\":false");
    assert_contains(result.response, "\"fixture_contract_preserved\":true");
    assert_contains(result.response, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v88\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/88/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_contains(result.response, "\"adapter_shell\":\"ManagedAuditAdapter disabled shell\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:4686040cffebdf32\"");
    assert_contains(result.response, "\"adapter_shell_storage_backend\":false");
    assert_contains(result.response, "\"storage_backend_allowed\":false");
    assert_contains(result.response, "\"local_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v89\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(result.response, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:953dd8fb3e1640f5\"");
    assert_contains(result.response, "\"external_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_external_adapter\":false");
    assert_contains(result.response, "\"credential_required\":false");
    assert_contains(result.response, "\"credential_read_allowed\":false");
    assert_contains(result.response, "\"migration_required\":false");
    assert_contains(result.response, "\"migration_execution_allowed\":false");
    assert_contains(result.response, "\"schema_authoritative\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v90\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_contains(result.response, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:d71b79804d9cfd94\"");
    assert_contains(result.response, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(result.response, "\"credential_value_read_allowed\":false");
    assert_contains(result.response, "\"schema_migration_execution_allowed\":false");
    assert_contains(result.response, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(result.response, "\"sandbox_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v95\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(result.response, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(result.response, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_contains(result.response, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(result.response, "\"timeout_budget_ms\":15000");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:fbad91e89da49997\"");
    assert_contains(result.response, "\"connection_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_write_allowed\":false");
    assert_contains(result.response, "\"participates_in_sandbox_connection\":false");
    assert_contains(result.response, "\"node_auto_start_allowed\":false");
    assert_contains(result.response, "\"diagnostics\":{\"write_commands_executed\":false,"
                                     "\"dynamic_fields\":[\"server.uptime_seconds\"]}");

    minikv::Store stats_store;
    minikv::CommandProcessor stats_processor{stats_store};
    result = stats_processor.execute("STATSJSON");
    assert(result.response == statsjson_fixture);
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_metrics\"");
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal_enabled\":false");
    assert_contains(result.response, "\"wal\":null");
    assert_contains(result.response, "\"total_commands\":0");
    assert_contains(result.response, "\"breakdown\":[]");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");
    assert_contains(result.response, "\"diagnostics\":{\"write_commands_executed\":false,"
                                     "\"dynamic_fields\":[\"commands.total_latency_ns\","
                                     "\"commands.avg_latency_ns\",\"commands.max_latency_ns\","
                                     "\"commands.breakdown[*].*_latency_ns\"]}");

    return 0;
}
