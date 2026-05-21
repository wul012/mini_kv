#include "minikv/command_response_formatters.hpp"

#include "minikv/managed_audit_receipts.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/runtime_evidence_receipts.hpp"
#include "minikv/version.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace minikv::command_response_formatters {
namespace {

constexpr int runtime_smoke_schema_version = 1;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string protocol_json_array(std::string_view protocol) {
    std::vector<std::string> values;
    while (!protocol.empty()) {
        const auto comma = protocol.find(',');
        std::string_view token = protocol.substr(0, comma);
        while (!token.empty() && (token.front() == ' ' || token.front() == '\t' || token.front() == '\r' || token.front() == '\n')) {
            token.remove_prefix(1);
        }
        while (!token.empty() && (token.back() == ' ' || token.back() == '\t' || token.back() == '\r' || token.back() == '\n')) {
            token.remove_suffix(1);
        }
        if (!token.empty()) {
            values.emplace_back(token);
        }

        if (comma == std::string_view::npos) {
            break;
        }
        protocol.remove_prefix(comma + 1);
    }
    return json_string_array(values);
}

std::string runtime_no_start_no_write_follow_up_json(const std::vector<std::string>& read_commands) {
    return "{\"follow_up_version\":\"mini-kv-runtime-no-start-no-write-follow-up.v1\","
           "\"follow_up_fixture_path\":\"fixtures/release/runtime-no-start-no-write-follow-up.json\","
           "\"source_envelope\":\"Node v236 manual sandbox connection dry-run request envelope\","
           "\"consumer_hint\":\"Node v239 manual sandbox connection operator window evidence verification\","
           "\"current_project_version\":" + json_string(version) +
           ",\"current_release_version\":\"v102\","
           "\"current_artifact_path_hint\":" + json_string(current_version_path_hint) + ","
           "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\","
           "\"binary_provenance_digest\":" +
           json_string(runtime_evidence_receipts::binary_provenance_digest()) +
           ",\"retention_check_digest\":" +
           json_string(runtime_evidence_receipts::retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           json_string(runtime_evidence_receipts::retention_provenance_replay_marker_digest()) +
           ",\"restore_boundary_receipt_digest\":" +
           json_string(managed_audit_receipts::restore_boundary_receipt_digest()) +
           ",\"non_authoritative_storage_receipt_digest\":" +
           json_string(managed_audit_receipts::non_authoritative_storage_receipt_digest()) +
           ",\"command_dispatch_receipt_digest\":" +
           json_string(managed_audit_receipts::command_dispatch_quality_receipt_digest()) +
           ",\"adapter_shell_receipt_digest\":" +
           json_string(managed_audit_receipts::adapter_shell_non_storage_guard_receipt_digest()) +
           ",\"external_adapter_receipt_digest\":" +
           json_string(managed_audit_receipts::external_adapter_non_participation_receipt_digest()) +
           ",\"sandbox_adapter_receipt_digest\":" +
           json_string(managed_audit_receipts::sandbox_adapter_non_participation_receipt_digest()) +
           ",\"sandbox_connection_echo_marker_digest\":" +
           json_string(managed_audit_receipts::sandbox_connection_receipt_echo_marker_digest()) +
           ",\"sandbox_no_start_receipt_digest\":" +
           json_string(managed_audit_receipts::sandbox_connection_no_start_guard_receipt_digest()) +
           ",\"failure_taxonomy_digest\":" +
           json_string(runtime_evidence_receipts::smoke_failure_taxonomy_digest()) +
           ",\"read_command_list_digest\":" +
           json_string(runtime_evidence_receipts::read_command_list_digest(read_commands)) +
           ",\"read_only\":true,\"execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"connection_execution_allowed\":false,"
           "\"write_commands_executed\":false,\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,\"managed_audit_store\":false,"
           "\"storage_write_allowed\":false,\"managed_audit_write_executed\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_read_allowed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"order_authoritative\":false,"
           "\"node_action\":\"verify no-start/no-write follow-up before Node v239 manual sandbox connection operator window evidence verification\"}";
}

std::string operator_window_no_start_no_write_receipt_json(const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-operator-window-no-start-no-write-receipt.v1\","
           "\"receipt_fixture_path\":\"fixtures/release/operator-window-no-start-no-write-receipt.json\","
           "\"source_checklist\":\"Node v238 manual sandbox connection operator window checklist\","
           "\"consumer_hint\":\"Node v239 manual sandbox connection operator window evidence verification\","
           "\"current_project_version\":" + json_string(version) +
           ",\"current_release_version\":\"v102\","
           "\"current_artifact_path_hint\":" + json_string(current_version_path_hint) + ","
           "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\","
           "\"source_checklist_state\":\"manual-sandbox-connection-operator-window-checklist-ready\","
           "\"approval_item_count\":3,\"checklist_step_count\":8,"
           "\"pause_condition_count\":8,\"forbidden_operation_count\":6,"
           "\"operator_window_materials_read_only\":true,"
           "\"ready_for_java_v93_echo_receipt\":true,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"binary_provenance_digest\":" +
           json_string(runtime_evidence_receipts::binary_provenance_digest()) +
           ",\"retention_check_digest\":" +
           json_string(runtime_evidence_receipts::retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           json_string(runtime_evidence_receipts::retention_provenance_replay_marker_digest()) +
           ",\"restore_boundary_receipt_digest\":" +
           json_string(managed_audit_receipts::restore_boundary_receipt_digest()) +
           ",\"non_authoritative_storage_receipt_digest\":" +
           json_string(managed_audit_receipts::non_authoritative_storage_receipt_digest()) +
           ",\"command_dispatch_receipt_digest\":" +
           json_string(managed_audit_receipts::command_dispatch_quality_receipt_digest()) +
           ",\"adapter_shell_receipt_digest\":" +
           json_string(managed_audit_receipts::adapter_shell_non_storage_guard_receipt_digest()) +
           ",\"external_adapter_receipt_digest\":" +
           json_string(managed_audit_receipts::external_adapter_non_participation_receipt_digest()) +
           ",\"sandbox_adapter_receipt_digest\":" +
           json_string(managed_audit_receipts::sandbox_adapter_non_participation_receipt_digest()) +
           ",\"sandbox_connection_echo_marker_digest\":" +
           json_string(managed_audit_receipts::sandbox_connection_receipt_echo_marker_digest()) +
           ",\"sandbox_no_start_receipt_digest\":" +
           json_string(managed_audit_receipts::sandbox_connection_no_start_guard_receipt_digest()) +
           ",\"runtime_follow_up_fixture_path\":\"fixtures/release/runtime-no-start-no-write-follow-up.json\","
           "\"failure_taxonomy_digest\":" +
           json_string(runtime_evidence_receipts::smoke_failure_taxonomy_digest()) +
           ",\"read_command_list_digest\":" +
           json_string(runtime_evidence_receipts::read_command_list_digest(read_commands)) +
           ",\"read_only\":true,\"execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"connection_execution_allowed\":false,"
           "\"write_commands_executed\":false,\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,\"managed_audit_store\":false,"
           "\"storage_write_allowed\":false,\"managed_audit_write_executed\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"operator_window_write_allowed\":false,"
           "\"order_authoritative\":false,"
           "\"node_action\":\"verify mini-kv operator window no-start/no-write receipt with Java v93 echo before Node v239 evidence verification\"}";
}

} // namespace

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
        "managed_audit_sandbox_connection_no_start_guard_receipt",
        "runtime_no_start_no_write_follow_up",
        "operator_window_no_start_no_write_receipt",
        "manual_sandbox_dry_run_command_non_participation_receipt",
        "manual_sandbox_connection_precheck_non_participation_receipt",
        "disabled_adapter_client_non_participation_receipt",
        "fake_transport_dry_run_packet_non_participation_receipt",
        "sandbox_endpoint_handle_non_participation_receipt",
        "credential_resolver_non_participation_receipt",
        "disabled_credential_resolver_precheck_non_participation_receipt",
        "test_only_resolver_shell_non_participation_receipt",
        "credential_resolver_fake_shell_archive_non_participation_receipt",
        "credential_resolver_production_readiness_blocked_decision_non_participation_receipt",
        "credential_resolver_pre_implementation_plan_intake_non_participation_receipt",
        "credential_resolver_disabled_implementation_candidate_non_participation_receipt",
        "credential_resolver_approval_required_boundary_non_participation_receipt",
        "credential_resolver_approval_required_implementation_readiness_non_participation_receipt",
        "credential_resolver_implementation_plan_non_participation_receipt",
        "credential_resolver_disabled_fake_harness_non_participation_receipt",
        "credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt",
        "credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check",
        "credential_resolver_disabled_runtime_shell_non_participation_receipt",
        "credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt",
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
                           ",\"server\":{\"protocol\":" + protocol_json_array(runtime_info.protocol) +
                           ",\"uptime_seconds\":" + std::to_string(uptime) +
                           ",\"max_request_bytes\":" + std::to_string(runtime_info.max_request_bytes) +
                           ",\"metrics_enabled\":" + json_bool(runtime_info.metrics_enabled) + "}" +
                           ",\"store\":{\"live_keys\":" + std::to_string(live_keys) +
                           ",\"order_authoritative\":false}" +
                           ",\"wal\":{\"enabled\":" + json_bool(wal != nullptr);

    if (wal_report.has_value()) {
        response += ",\"status\":\"enabled\",\"compact_recommended\":" +
                    json_bool(wal_report->compact_recommended) +
                    ",\"bytes\":" + std::to_string(wal_report->bytes) +
                    ",\"records\":" + std::to_string(wal_report->records) + "}";
    } else {
        response += ",\"status\":\"disabled\",\"compact_recommended\":false}";
    }

    response += "," + format_command_metrics_json(command_metrics);
    response += ",\"connection_stats\":{\"available\":" + json_bool(stats.available);
    if (stats.available) {
        response += ",\"active_connections\":" + std::to_string(stats.active_connections) +
                    ",\"total_connections\":" + std::to_string(stats.total_connections) +
                    ",\"peak_connections\":" + std::to_string(stats.peak_connections);
    }
    response += "},\"real_read\":{\"allowed\":true,\"commands\":" +
                json_string_array(read_commands) +
                ",\"forbidden_commands\":" + json_string_array(forbidden_commands) +
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
                ",\"managed_audit_sandbox_connection_no_start_guard_receipt\":" +
                managed_audit_receipts::format_sandbox_connection_no_start_guard_receipt_json() +
                ",\"runtime_no_start_no_write_follow_up\":" +
                runtime_no_start_no_write_follow_up_json(read_commands) +
                ",\"operator_window_no_start_no_write_receipt\":" +
                operator_window_no_start_no_write_receipt_json(read_commands) +
                ",\"manual_sandbox_dry_run_command_non_participation_receipt\":" +
                runtime_evidence_receipts::format_manual_sandbox_dry_run_command_non_participation_receipt_json(
                    read_commands) +
                ",\"manual_sandbox_connection_precheck_non_participation_receipt\":" +
                runtime_evidence_receipts::format_manual_sandbox_connection_precheck_non_participation_receipt_json(
                    read_commands) +
                ",\"disabled_adapter_client_non_participation_receipt\":" +
                runtime_evidence_receipts::format_disabled_adapter_client_non_participation_receipt_json(
                    read_commands) +
                ",\"fake_transport_dry_run_packet_non_participation_receipt\":" +
                runtime_evidence_receipts::format_fake_transport_dry_run_packet_non_participation_receipt_json(
                    read_commands) +
                ",\"sandbox_endpoint_handle_non_participation_receipt\":" +
                runtime_evidence_receipts::format_sandbox_endpoint_handle_non_participation_receipt_json(
                    read_commands) +
                ",\"credential_resolver_non_participation_receipt\":" +
                runtime_evidence_receipts::format_credential_resolver_non_participation_receipt_json(
                    read_commands) +
                ",\"disabled_credential_resolver_precheck_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_disabled_credential_resolver_precheck_non_participation_receipt_json(
                        read_commands) +
                ",\"test_only_resolver_shell_non_participation_receipt\":" +
                runtime_evidence_receipts::format_test_only_resolver_shell_non_participation_receipt_json(
                    read_commands) +
                ",\"credential_resolver_fake_shell_archive_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_fake_shell_archive_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_implementation_candidate_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_approval_required_boundary_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_approval_required_boundary_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_implementation_plan_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_implementation_plan_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_disabled_fake_harness_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_fake_harness_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_json(
                        read_commands) +
                ",\"credential_resolver_disabled_runtime_shell_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json(
                        read_commands) +
                ",\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\":" +
                runtime_evidence_receipts::
                    format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json(
                        read_commands) +
                ",\"failure_taxonomy\":" + runtime_evidence_receipts::format_smoke_failure_taxonomy_json() +
                ",\"diagnostics\":{\"node_consumption\":\"Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt after Node v297 blocked candidate gate and Java v134 runtime shell candidate gate echo; Node v296 may verify the mini-kv v130 credential resolver disabled runtime shell non-participation receipt after Node v295 design review and Java v133 disabled runtime shell handoff echo; Node v293 may verify the mini-kv v129 credential resolver disabled fake harness execution-denied receipt verification/retention check after Node v292 blocked readiness decision record and Java v131 direct execution-denied echo; Node v291 may verify the mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt after Node v290 disabled fake harness execution-denied route preflight and Java v127 evidence; Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt after Node v288 disabled fake harness contract and Java v122-v126 evidence; Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt after Node v283 implementation plan draft and Java v121 echo; Node v282 may verify the mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt after Node v281 readiness review; Node v275 may verify the mini-kv v121 credential resolver approval-required boundary non-participation receipt after Node v274 disabled candidate upstream echo verification; Node v274 may verify the mini-kv v120 credential resolver disabled implementation candidate non-participation receipt after Node v273 disabled implementation candidate review; Node v272 may verify the mini-kv v119 credential resolver pre-implementation plan intake non-participation receipt after Node v270 plan intake; Node v269 may verify the mini-kv v118 credential resolver production-readiness blocked-decision non-participation receipt after Node v268 blocked decision gate; Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt after Node v266 archive verification; Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt after Node v264 credential resolver test-only shell contract; Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt after Node v245 precheck packet evidence; Node v257 may verify the mini-kv v112 fake transport dry-run packet non-participation receipt after Node v255/v256 packet and archive evidence; Node v263 may verify the mini-kv v115 disabled credential resolver precheck non-participation receipt after Node v262 disabled credential resolver precheck; Node v261 may verify the mini-kv v114 credential resolver non-participation receipt after Node v260 credential resolver decision record; Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt after Node v258 endpoint handle preflight review; Node v254 may verify the mini-kv v111 disabled adapter client non-participation receipt after Node v252/v253 disabled client and fake transport shell evidence; Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt after Node v241/v243 command package evidence; Node v239 may still verify the mini-kv operator window no-start/no-write receipt, the v102 runtime no-start/no-write follow-up, the v96 sandbox connection receipt echo marker, the v95 sandbox adapter non-participation receipt, the v90 external adapter non-participation receipt, the v89 adapter shell non-storage guard receipt, the v88 command dispatch quality receipt, the v87 managed audit adapter non-authoritative storage receipt, the v86 managed audit adapter restore boundary receipt, runtime evidence retention, binary provenance digest alignment, live-read session echo, uptime bucket, read command digest, taxonomy digest, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence before operator window evidence verification; mini-kv must already be running and the read-only window must be open manually\","
                "\"dynamic_fields\":" + json_string_array(dynamic_fields) +
                ",\"notes\":" + json_string_array(notes) + "}}";
    return response;
}

} // namespace minikv::command_response_formatters
