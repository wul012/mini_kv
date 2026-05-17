#include "minikv/managed_audit_receipts.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

} // namespace

int main() {
    const auto echo_marker = minikv::managed_audit_receipts::format_sandbox_connection_receipt_echo_marker_json();
    assert_contains(echo_marker, "\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_contains(echo_marker, "\"source_version\":\"0.101.0\"");
    assert_contains(echo_marker, "\"consumed_release_version\":\"v95\"");
    assert_contains(echo_marker, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(echo_marker, "\"current_artifact_path_hint\":\"c/101/\"");
    assert_contains(echo_marker, "\"marker_digest\":\"" +
                                     minikv::managed_audit_receipts::sandbox_connection_receipt_echo_marker_digest() +
                                     "\"");
    assert_contains(echo_marker, "\"connection_execution_allowed\":false");
    assert_contains(echo_marker, "\"credential_value_read_allowed\":false");
    assert_contains(echo_marker, "\"schema_migration_execution_allowed\":false");
    assert_contains(echo_marker, "\"managed_audit_write_allowed\":false");
    assert_contains(echo_marker, "\"node_auto_start_allowed\":false");
    assert_contains(echo_marker, "\"order_authoritative\":false");

    const auto no_start =
        minikv::managed_audit_receipts::format_sandbox_connection_no_start_guard_receipt_json();
    assert_contains(no_start, "\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_contains(no_start, "\"source_version\":\"0.101.0\"");
    assert_contains(no_start, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(no_start, "\"consumed_release_version\":\"v96\"");
    assert_contains(no_start, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(no_start, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(no_start, "\"current_artifact_path_hint\":\"c/101/\"");
    assert_contains(no_start, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(no_start,
                    "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(no_start, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(no_start, "\"timeout_budget_ms\":15000");
    assert_contains(no_start, "\"receipt_digest\":\"" +
                                  minikv::managed_audit_receipts::sandbox_connection_no_start_guard_receipt_digest() +
                                  "\"");
    assert_contains(no_start, "\"manual_window_open_by_default\":false");
    assert_contains(no_start, "\"connection_execution_allowed\":false");
    assert_contains(no_start, "\"node_auto_start_allowed\":false");
    assert_contains(no_start, "\"java_auto_start_allowed\":false");
    assert_contains(no_start, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(no_start, "\"credential_value_required\":false");
    assert_contains(no_start, "\"credential_value_read_allowed\":false");
    assert_contains(no_start, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(no_start, "\"schema_migration_execution_allowed\":false");
    assert_contains(no_start, "\"managed_audit_write_allowed\":false");
    assert_contains(no_start, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(no_start, "\"participates_in_sandbox_connection\":false");
    assert_contains(no_start, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(no_start, "\"restore_execution_allowed\":false");
    assert_contains(no_start, "\"load_restore_compact_executed\":false");
    assert_contains(no_start, "\"write_handler_changed\":false");
    assert_contains(no_start, "\"admin_handler_changed\":false");
    assert_contains(no_start, "\"wal_snapshot_restore_touched\":false");
    assert_contains(no_start, "\"order_authoritative\":false");

    return 0;
}
