#include "minikv/managed_audit_receipts.hpp"

#include "managed_audit_receipt_utils.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>

namespace minikv::managed_audit_receipts {
namespace {

using detail::bool_part;
using detail::field_bool;
using detail::field_string;
using detail::json_object;

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
    current_version_path_hint,
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
    current_version_path_hint,
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
    current_version_path_hint,
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
    current_version_path_hint,
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

} // namespace minikv::managed_audit_receipts
