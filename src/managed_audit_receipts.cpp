#include "minikv/managed_audit_receipts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::managed_audit_receipts {
namespace {

using runtime_evidence::JsonField;

std::string bool_part(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string field_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_object(const std::vector<JsonField>& fields) {
    return runtime_evidence::json_object(fields);
}

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

constexpr RuntimeManagedAuditAdapterRestoreBoundaryReceipt restore_boundary_receipt = {
    "Node v215 managed audit dry-run adapter candidate",
    "Node v213 managed audit packet restore drill plan",
    "v85",
    "c/85/",
    "fnv1a64:1ea4570c967cfdb1",
    "c/99/",
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

constexpr RuntimeManagedAuditAdapterNonAuthoritativeStorageReceipt non_authoritative_storage_receipt = {
    "Node v217 managed audit adapter production-hardening readiness gate",
    "Node v215 managed audit dry-run adapter candidate",
    "v86",
    "c/86/",
    "fnv1a64:f39d8e3ef98654ea",
    "c/99/",
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

constexpr RuntimeCommandDispatchQualityReceipt command_dispatch_quality_receipt = {
    "Node v219 managed audit adapter implementation precheck packet",
    "Node v217 managed audit adapter production-hardening readiness gate",
    "v87",
    "c/87/",
    "fnv1a64:111f0daf1283eab6",
    "c/99/",
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

constexpr RuntimeAdapterShellNonStorageGuardReceipt adapter_shell_guard_receipt = {
    "Node v221 managed audit local adapter candidate dry-run",
    "Node v220 managed audit adapter interface and disabled shell",
    "v88",
    "c/88/",
    "fnv1a64:4aa6d12fb067e2a6",
    "c/99/",
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

struct RuntimeManagedAuditExternalAdapterNonParticipationReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view adapter_target;
    std::string_view runtime_role;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool external_adapter_storage_backend;
    bool participates_in_external_adapter;
    bool credential_required;
    bool credential_read_allowed;
    bool migration_required;
    bool migration_execution_allowed;
    bool schema_authoritative;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool managed_audit_write_executed;
    bool local_dry_run_records_written;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditExternalAdapterNonParticipationReceipt external_adapter_receipt = {
    "Node v223 managed audit external adapter connection readiness review",
    "Node v222 managed audit local adapter candidate verification report",
    "v89",
    "c/89/",
    "fnv1a64:76411286a0913dc8",
    "c/99/",
    "real external managed audit adapter",
    "runtime evidence provider only",
    "external adapter non-participation receipt only; mini-kv is not an external managed audit adapter storage backend and keeps runtime evidence provider role",
    "verify mini-kv non-participation before external adapter connection readiness review",
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
    false,
    false,
    false,
    false,
    false,
};

struct RuntimeManagedAuditSandboxAdapterNonParticipationReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view adapter_target;
    std::string_view runtime_role;
    std::string_view boundary;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool sandbox_adapter_storage_backend;
    bool participates_in_sandbox_adapter;
    bool credential_value_required;
    bool credential_value_read_allowed;
    bool production_credential_read_allowed;
    bool schema_migration_execution_allowed;
    bool sandbox_schema_authoritative;
    bool sandbox_managed_audit_state_write_allowed;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool managed_audit_write_executed;
    bool sandbox_dry_run_records_written;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditSandboxAdapterNonParticipationReceipt sandbox_adapter_receipt = {
    "Node v225 managed audit sandbox adapter dry-run package",
    "Node v224 managed audit sandbox adapter dry-run plan",
    "v90",
    "c/90/",
    "fnv1a64:0dfb07cd2f8de289",
    "c/99/",
    "sandbox managed audit adapter dry-run",
    "runtime evidence provider only",
    "sandbox adapter runtime evidence non-participation receipt only; mini-kv is not a sandbox audit storage backend and does not read credentials or write managed audit state",
    "verify mini-kv sandbox non-participation before managed audit sandbox adapter dry-run package",
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
    false,
    false,
    false,
    false,
    false,
    false,
};

struct RuntimeManagedAuditSandboxConnectionReceiptEchoMarker {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_receipt_digest;
    std::string_view current_artifact_path_hint;
    std::string_view source_operator_packet_version;
    std::string_view source_operator_packet_profile;
    std::string_view packet_mode;
    std::string_view owner_approval_artifact_id_field;
    std::string_view credential_handle_name_field;
    std::string_view schema_rehearsal_id_field;
    std::string_view rollback_path_id_field;
    std::string_view manual_abort_marker;
    std::string_view runtime_role;
    std::string_view boundary;
    std::string_view node_action;
    int timeout_budget_ms;
    int required_operator_field_count;
    bool read_only;
    bool execution_allowed;
    bool connection_execution_allowed;
    bool credential_value_required;
    bool credential_value_read_allowed;
    bool schema_migration_execution_allowed;
    bool managed_audit_write_allowed;
    bool sandbox_adapter_storage_backend;
    bool participates_in_sandbox_connection;
    bool sandbox_managed_audit_state_write_allowed;
    bool node_auto_start_allowed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool wal_snapshot_restore_touched;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditSandboxConnectionReceiptEchoMarker sandbox_connection_echo_marker = {
    "Node v229 manual sandbox connection packet verification",
    "Node v228 managed audit manual sandbox connection operator packet",
    "v95",
    "c/95/",
    "fnv1a64:ceaed265f7f9560c",
    "c/99/",
    "Node v228",
    "managed-audit-manual-sandbox-connection-operator-packet.v1",
    "manual-sandbox-connection-operator-packet-only",
    "ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID",
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE",
    "ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID",
    "ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID",
    "ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT",
    "runtime evidence provider only",
    "sandbox connection receipt echo marker only; mini-kv echoes Node v228 operator packet field handles but remains outside managed audit connection, credential reads, schema migration, storage, and state writes",
    "verify mini-kv receipt echo before Node v229 manual sandbox connection packet verification",
    15000,
    6,
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
    false,
    false,
    false,
    false,
    false,
    false,
};

struct RuntimeManagedAuditSandboxConnectionNoStartGuardReceipt {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_marker_digest;
    std::string_view current_artifact_path_hint;
    std::string_view preflight_gate_version;
    std::string_view manual_window_flag_name;
    std::string_view manual_window_mode;
    std::string_view runtime_role;
    std::string_view boundary;
    std::string_view node_action;
    int timeout_budget_ms;
    bool read_only;
    bool execution_allowed;
    bool manual_window_open_by_default;
    bool connection_execution_allowed;
    bool node_auto_start_allowed;
    bool java_auto_start_allowed;
    bool mini_kv_auto_start_allowed;
    bool credential_value_required;
    bool credential_value_read_allowed;
    bool schema_rehearsal_execution_allowed;
    bool schema_migration_execution_allowed;
    bool managed_audit_write_allowed;
    bool sandbox_adapter_storage_backend;
    bool participates_in_sandbox_connection;
    bool sandbox_managed_audit_state_write_allowed;
    bool restore_execution_allowed;
    bool load_restore_compact_executed;
    bool write_handler_changed;
    bool admin_handler_changed;
    bool wal_snapshot_restore_touched;
    bool order_authoritative;
};

constexpr RuntimeManagedAuditSandboxConnectionNoStartGuardReceipt sandbox_connection_no_start_guard_receipt = {
    "Node v231 manual sandbox connection preflight verification",
    "Node v230 manual sandbox connection preflight gate",
    "v96",
    "c/96/",
    "fnv1a64:b9fc556875ea625b",
    "c/99/",
    "Node v230",
    "ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED",
    "manual-window-required-no-auto-start",
    "runtime evidence provider only",
    "manual sandbox connection no-start guard receipt only; mini-kv is not auto-started by Node, does not join the sandbox connection, and remains outside credential reads, schema rehearsal, storage, and state writes",
    "verify mini-kv no-start guard before Node v231 manual sandbox connection preflight verification",
    15000,
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

} // namespace

std::string restore_boundary_receipt_digest() {
    const auto& receipt = restore_boundary_receipt;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-adapter-restore-boundary-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_marker_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.adapter_write_allowed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.managed_audit_write_executed)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_restore_boundary_receipt_json() {
    const auto& receipt = restore_boundary_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_marker_digest", field_string(receipt.consumed_marker_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"receipt_digest", field_string(restore_boundary_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"adapter_write_allowed", field_bool(receipt.adapter_write_allowed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"managed_audit_write_executed", field_bool(receipt.managed_audit_write_executed)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string non_authoritative_storage_receipt_digest() {
    const auto& receipt = non_authoritative_storage_receipt;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-adapter-non-authoritative-storage-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_receipt_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.managed_audit_store)},
            {bool_part(receipt.storage_write_allowed)},
            {bool_part(receipt.admin_commands_allowed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.managed_audit_write_executed)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_non_authoritative_storage_receipt_json() {
    const auto& receipt = non_authoritative_storage_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(receipt.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"receipt_digest", field_string(non_authoritative_storage_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"managed_audit_store", field_bool(receipt.managed_audit_store)},
        {"storage_write_allowed", field_bool(receipt.storage_write_allowed)},
        {"admin_commands_allowed", field_bool(receipt.admin_commands_allowed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"managed_audit_write_executed", field_bool(receipt.managed_audit_write_executed)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string command_dispatch_quality_receipt_digest() {
    const auto& receipt = command_dispatch_quality_receipt;
    return runtime_evidence::digest(
        "mini-kv-command-dispatch-quality-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_receipt_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {std::string{receipt.dispatch_family}},
            {std::string{receipt.split_commands}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.dispatch_split_applied)},
            {bool_part(receipt.handler_table_required)},
            {bool_part(receipt.write_handler_changed)},
            {bool_part(receipt.admin_handler_changed)},
            {bool_part(receipt.wal_snapshot_restore_touched)},
            {bool_part(receipt.behavior_changed)},
            {bool_part(receipt.fixture_contract_preserved)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_command_dispatch_quality_receipt_json() {
    const auto& receipt = command_dispatch_quality_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(receipt.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"dispatch_family", field_string(receipt.dispatch_family)},
        {"split_commands", field_string(receipt.split_commands)},
        {"receipt_digest", field_string(command_dispatch_quality_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"dispatch_split_applied", field_bool(receipt.dispatch_split_applied)},
        {"handler_table_required", field_bool(receipt.handler_table_required)},
        {"write_handler_changed", field_bool(receipt.write_handler_changed)},
        {"admin_handler_changed", field_bool(receipt.admin_handler_changed)},
        {"wal_snapshot_restore_touched", field_bool(receipt.wal_snapshot_restore_touched)},
        {"behavior_changed", field_bool(receipt.behavior_changed)},
        {"fixture_contract_preserved", field_bool(receipt.fixture_contract_preserved)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string adapter_shell_non_storage_guard_receipt_digest() {
    const auto& receipt = adapter_shell_guard_receipt;
    return runtime_evidence::digest(
        "mini-kv-adapter-shell-non-storage-guard-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_receipt_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {std::string{receipt.adapter_shell}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.adapter_shell_storage_backend)},
            {bool_part(receipt.storage_backend_allowed)},
            {bool_part(receipt.write_handler_changed)},
            {bool_part(receipt.admin_handler_changed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.managed_audit_write_executed)},
            {bool_part(receipt.local_dry_run_records_written)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_adapter_shell_non_storage_guard_receipt_json() {
    const auto& receipt = adapter_shell_guard_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(receipt.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"adapter_shell", field_string(receipt.adapter_shell)},
        {"receipt_digest", field_string(adapter_shell_non_storage_guard_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"adapter_shell_storage_backend", field_bool(receipt.adapter_shell_storage_backend)},
        {"storage_backend_allowed", field_bool(receipt.storage_backend_allowed)},
        {"write_handler_changed", field_bool(receipt.write_handler_changed)},
        {"admin_handler_changed", field_bool(receipt.admin_handler_changed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"managed_audit_write_executed", field_bool(receipt.managed_audit_write_executed)},
        {"local_dry_run_records_written", field_bool(receipt.local_dry_run_records_written)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string external_adapter_non_participation_receipt_digest() {
    const auto& receipt = external_adapter_receipt;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-external-adapter-non-participation-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_receipt_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {std::string{receipt.adapter_target}},
            {std::string{receipt.runtime_role}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.external_adapter_storage_backend)},
            {bool_part(receipt.participates_in_external_adapter)},
            {bool_part(receipt.credential_required)},
            {bool_part(receipt.credential_read_allowed)},
            {bool_part(receipt.migration_required)},
            {bool_part(receipt.migration_execution_allowed)},
            {bool_part(receipt.schema_authoritative)},
            {bool_part(receipt.write_handler_changed)},
            {bool_part(receipt.admin_handler_changed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.managed_audit_write_executed)},
            {bool_part(receipt.local_dry_run_records_written)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_external_adapter_non_participation_receipt_json() {
    const auto& receipt = external_adapter_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(receipt.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"adapter_target", field_string(receipt.adapter_target)},
        {"runtime_role", field_string(receipt.runtime_role)},
        {"receipt_digest", field_string(external_adapter_non_participation_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"external_adapter_storage_backend", field_bool(receipt.external_adapter_storage_backend)},
        {"participates_in_external_adapter", field_bool(receipt.participates_in_external_adapter)},
        {"credential_required", field_bool(receipt.credential_required)},
        {"credential_read_allowed", field_bool(receipt.credential_read_allowed)},
        {"migration_required", field_bool(receipt.migration_required)},
        {"migration_execution_allowed", field_bool(receipt.migration_execution_allowed)},
        {"schema_authoritative", field_bool(receipt.schema_authoritative)},
        {"write_handler_changed", field_bool(receipt.write_handler_changed)},
        {"admin_handler_changed", field_bool(receipt.admin_handler_changed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"managed_audit_write_executed", field_bool(receipt.managed_audit_write_executed)},
        {"local_dry_run_records_written", field_bool(receipt.local_dry_run_records_written)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string sandbox_adapter_non_participation_receipt_digest() {
    const auto& receipt = sandbox_adapter_receipt;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-sandbox-adapter-non-participation-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_receipt_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {std::string{receipt.adapter_target}},
            {std::string{receipt.runtime_role}},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.sandbox_adapter_storage_backend)},
            {bool_part(receipt.participates_in_sandbox_adapter)},
            {bool_part(receipt.credential_value_required)},
            {bool_part(receipt.credential_value_read_allowed)},
            {bool_part(receipt.production_credential_read_allowed)},
            {bool_part(receipt.schema_migration_execution_allowed)},
            {bool_part(receipt.sandbox_schema_authoritative)},
            {bool_part(receipt.sandbox_managed_audit_state_write_allowed)},
            {bool_part(receipt.write_handler_changed)},
            {bool_part(receipt.admin_handler_changed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.managed_audit_write_executed)},
            {bool_part(receipt.sandbox_dry_run_records_written)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_sandbox_adapter_non_participation_receipt_json() {
    const auto& receipt = sandbox_adapter_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(receipt.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"adapter_target", field_string(receipt.adapter_target)},
        {"runtime_role", field_string(receipt.runtime_role)},
        {"receipt_digest", field_string(sandbox_adapter_non_participation_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"sandbox_adapter_storage_backend", field_bool(receipt.sandbox_adapter_storage_backend)},
        {"participates_in_sandbox_adapter", field_bool(receipt.participates_in_sandbox_adapter)},
        {"credential_value_required", field_bool(receipt.credential_value_required)},
        {"credential_value_read_allowed", field_bool(receipt.credential_value_read_allowed)},
        {"production_credential_read_allowed", field_bool(receipt.production_credential_read_allowed)},
        {"schema_migration_execution_allowed", field_bool(receipt.schema_migration_execution_allowed)},
        {"sandbox_schema_authoritative", field_bool(receipt.sandbox_schema_authoritative)},
        {"sandbox_managed_audit_state_write_allowed", field_bool(receipt.sandbox_managed_audit_state_write_allowed)},
        {"write_handler_changed", field_bool(receipt.write_handler_changed)},
        {"admin_handler_changed", field_bool(receipt.admin_handler_changed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"managed_audit_write_executed", field_bool(receipt.managed_audit_write_executed)},
        {"sandbox_dry_run_records_written", field_bool(receipt.sandbox_dry_run_records_written)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

std::string sandbox_connection_receipt_echo_marker_digest() {
    const auto& marker = sandbox_connection_echo_marker;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-sandbox-connection-receipt-echo-marker",
        {
            {std::string{version}},
            {std::string{marker.consumed_by}},
            {std::string{marker.consumed_release_version}},
            {std::string{marker.consumed_artifact_path_hint}},
            {std::string{marker.consumed_receipt_digest}},
            {std::string{marker.current_artifact_path_hint}},
            {std::string{marker.source_operator_packet_version}},
            {std::string{marker.source_operator_packet_profile}},
            {std::string{marker.packet_mode}},
            {std::string{marker.owner_approval_artifact_id_field}},
            {std::string{marker.credential_handle_name_field}},
            {std::string{marker.schema_rehearsal_id_field}},
            {std::string{marker.rollback_path_id_field}},
            {std::to_string(marker.timeout_budget_ms)},
            {std::string{marker.manual_abort_marker}},
            {std::to_string(marker.required_operator_field_count)},
            {bool_part(marker.read_only)},
            {bool_part(marker.execution_allowed)},
            {bool_part(marker.connection_execution_allowed)},
            {bool_part(marker.credential_value_required)},
            {bool_part(marker.credential_value_read_allowed)},
            {bool_part(marker.schema_migration_execution_allowed)},
            {bool_part(marker.managed_audit_write_allowed)},
            {bool_part(marker.sandbox_adapter_storage_backend)},
            {bool_part(marker.participates_in_sandbox_connection)},
            {bool_part(marker.sandbox_managed_audit_state_write_allowed)},
            {bool_part(marker.node_auto_start_allowed)},
            {bool_part(marker.restore_execution_allowed)},
            {bool_part(marker.load_restore_compact_executed)},
            {bool_part(marker.write_handler_changed)},
            {bool_part(marker.admin_handler_changed)},
            {bool_part(marker.wal_snapshot_restore_touched)},
            {bool_part(marker.order_authoritative)},
        });
}

std::string format_sandbox_connection_receipt_echo_marker_json() {
    const auto& marker = sandbox_connection_echo_marker;
    return json_object({
        {"consumer", field_string(marker.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(marker.consumed_by)},
        {"consumed_release_version", field_string(marker.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(marker.consumed_artifact_path_hint)},
        {"consumed_receipt_digest", field_string(marker.consumed_receipt_digest)},
        {"current_artifact_path_hint", field_string(marker.current_artifact_path_hint)},
        {"source_operator_packet_version", field_string(marker.source_operator_packet_version)},
        {"source_operator_packet_profile", field_string(marker.source_operator_packet_profile)},
        {"packet_mode", field_string(marker.packet_mode)},
        {"owner_approval_artifact_id_field", field_string(marker.owner_approval_artifact_id_field)},
        {"credential_handle_name_field", field_string(marker.credential_handle_name_field)},
        {"schema_rehearsal_id_field", field_string(marker.schema_rehearsal_id_field)},
        {"rollback_path_id_field", field_string(marker.rollback_path_id_field)},
        {"timeout_budget_ms", std::to_string(marker.timeout_budget_ms)},
        {"manual_abort_marker", field_string(marker.manual_abort_marker)},
        {"required_operator_field_count", std::to_string(marker.required_operator_field_count)},
        {"runtime_role", field_string(marker.runtime_role)},
        {"marker_digest", field_string(sandbox_connection_receipt_echo_marker_digest())},
        {"read_only", field_bool(marker.read_only)},
        {"execution_allowed", field_bool(marker.execution_allowed)},
        {"connection_execution_allowed", field_bool(marker.connection_execution_allowed)},
        {"credential_value_required", field_bool(marker.credential_value_required)},
        {"credential_value_read_allowed", field_bool(marker.credential_value_read_allowed)},
        {"schema_migration_execution_allowed", field_bool(marker.schema_migration_execution_allowed)},
        {"managed_audit_write_allowed", field_bool(marker.managed_audit_write_allowed)},
        {"sandbox_adapter_storage_backend", field_bool(marker.sandbox_adapter_storage_backend)},
        {"participates_in_sandbox_connection", field_bool(marker.participates_in_sandbox_connection)},
        {"sandbox_managed_audit_state_write_allowed", field_bool(marker.sandbox_managed_audit_state_write_allowed)},
        {"node_auto_start_allowed", field_bool(marker.node_auto_start_allowed)},
        {"restore_execution_allowed", field_bool(marker.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(marker.load_restore_compact_executed)},
        {"write_handler_changed", field_bool(marker.write_handler_changed)},
        {"admin_handler_changed", field_bool(marker.admin_handler_changed)},
        {"wal_snapshot_restore_touched", field_bool(marker.wal_snapshot_restore_touched)},
        {"order_authoritative", field_bool(marker.order_authoritative)},
        {"boundary", field_string(marker.boundary)},
        {"node_action", field_string(marker.node_action)},
    });
}

std::string sandbox_connection_no_start_guard_receipt_digest() {
    const auto& receipt = sandbox_connection_no_start_guard_receipt;
    return runtime_evidence::digest(
        "mini-kv-managed-audit-sandbox-connection-no-start-guard-receipt",
        {
            {std::string{version}},
            {std::string{receipt.consumed_by}},
            {std::string{receipt.consumed_release_version}},
            {std::string{receipt.consumed_artifact_path_hint}},
            {std::string{receipt.consumed_marker_digest}},
            {std::string{receipt.current_artifact_path_hint}},
            {std::string{receipt.preflight_gate_version}},
            {std::string{receipt.manual_window_flag_name}},
            {std::string{receipt.manual_window_mode}},
            {std::to_string(receipt.timeout_budget_ms)},
            {bool_part(receipt.read_only)},
            {bool_part(receipt.execution_allowed)},
            {bool_part(receipt.manual_window_open_by_default)},
            {bool_part(receipt.connection_execution_allowed)},
            {bool_part(receipt.node_auto_start_allowed)},
            {bool_part(receipt.java_auto_start_allowed)},
            {bool_part(receipt.mini_kv_auto_start_allowed)},
            {bool_part(receipt.credential_value_required)},
            {bool_part(receipt.credential_value_read_allowed)},
            {bool_part(receipt.schema_rehearsal_execution_allowed)},
            {bool_part(receipt.schema_migration_execution_allowed)},
            {bool_part(receipt.managed_audit_write_allowed)},
            {bool_part(receipt.sandbox_adapter_storage_backend)},
            {bool_part(receipt.participates_in_sandbox_connection)},
            {bool_part(receipt.sandbox_managed_audit_state_write_allowed)},
            {bool_part(receipt.restore_execution_allowed)},
            {bool_part(receipt.load_restore_compact_executed)},
            {bool_part(receipt.write_handler_changed)},
            {bool_part(receipt.admin_handler_changed)},
            {bool_part(receipt.wal_snapshot_restore_touched)},
            {bool_part(receipt.order_authoritative)},
        });
}

std::string format_sandbox_connection_no_start_guard_receipt_json() {
    const auto& receipt = sandbox_connection_no_start_guard_receipt;
    return json_object({
        {"consumer", field_string(receipt.consumer)},
        {"source_version", field_string(version)},
        {"consumed_by", field_string(receipt.consumed_by)},
        {"consumed_release_version", field_string(receipt.consumed_release_version)},
        {"consumed_artifact_path_hint", field_string(receipt.consumed_artifact_path_hint)},
        {"consumed_marker_digest", field_string(receipt.consumed_marker_digest)},
        {"current_artifact_path_hint", field_string(receipt.current_artifact_path_hint)},
        {"preflight_gate_version", field_string(receipt.preflight_gate_version)},
        {"manual_window_flag_name", field_string(receipt.manual_window_flag_name)},
        {"manual_window_mode", field_string(receipt.manual_window_mode)},
        {"timeout_budget_ms", std::to_string(receipt.timeout_budget_ms)},
        {"runtime_role", field_string(receipt.runtime_role)},
        {"receipt_digest", field_string(sandbox_connection_no_start_guard_receipt_digest())},
        {"read_only", field_bool(receipt.read_only)},
        {"execution_allowed", field_bool(receipt.execution_allowed)},
        {"manual_window_open_by_default", field_bool(receipt.manual_window_open_by_default)},
        {"connection_execution_allowed", field_bool(receipt.connection_execution_allowed)},
        {"node_auto_start_allowed", field_bool(receipt.node_auto_start_allowed)},
        {"java_auto_start_allowed", field_bool(receipt.java_auto_start_allowed)},
        {"mini_kv_auto_start_allowed", field_bool(receipt.mini_kv_auto_start_allowed)},
        {"credential_value_required", field_bool(receipt.credential_value_required)},
        {"credential_value_read_allowed", field_bool(receipt.credential_value_read_allowed)},
        {"schema_rehearsal_execution_allowed", field_bool(receipt.schema_rehearsal_execution_allowed)},
        {"schema_migration_execution_allowed", field_bool(receipt.schema_migration_execution_allowed)},
        {"managed_audit_write_allowed", field_bool(receipt.managed_audit_write_allowed)},
        {"sandbox_adapter_storage_backend", field_bool(receipt.sandbox_adapter_storage_backend)},
        {"participates_in_sandbox_connection", field_bool(receipt.participates_in_sandbox_connection)},
        {"sandbox_managed_audit_state_write_allowed", field_bool(receipt.sandbox_managed_audit_state_write_allowed)},
        {"restore_execution_allowed", field_bool(receipt.restore_execution_allowed)},
        {"load_restore_compact_executed", field_bool(receipt.load_restore_compact_executed)},
        {"write_handler_changed", field_bool(receipt.write_handler_changed)},
        {"admin_handler_changed", field_bool(receipt.admin_handler_changed)},
        {"wal_snapshot_restore_touched", field_bool(receipt.wal_snapshot_restore_touched)},
        {"order_authoritative", field_bool(receipt.order_authoritative)},
        {"boundary", field_string(receipt.boundary)},
        {"node_action", field_string(receipt.node_action)},
    });
}

} // namespace minikv::managed_audit_receipts
