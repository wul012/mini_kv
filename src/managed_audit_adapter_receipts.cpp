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
    current_version_path_hint,
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
    current_version_path_hint,
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

} // namespace

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

} // namespace minikv::managed_audit_receipts
