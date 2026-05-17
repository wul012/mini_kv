#pragma once

#include <string>

namespace minikv::managed_audit_receipts {

std::string restore_boundary_receipt_digest();
std::string format_restore_boundary_receipt_json();

std::string non_authoritative_storage_receipt_digest();
std::string format_non_authoritative_storage_receipt_json();

std::string command_dispatch_quality_receipt_digest();
std::string format_command_dispatch_quality_receipt_json();

std::string adapter_shell_non_storage_guard_receipt_digest();
std::string format_adapter_shell_non_storage_guard_receipt_json();

std::string external_adapter_non_participation_receipt_digest();
std::string format_external_adapter_non_participation_receipt_json();

std::string sandbox_adapter_non_participation_receipt_digest();
std::string format_sandbox_adapter_non_participation_receipt_json();

std::string sandbox_connection_receipt_echo_marker_digest();
std::string format_sandbox_connection_receipt_echo_marker_json();

std::string sandbox_connection_no_start_guard_receipt_digest();
std::string format_sandbox_connection_no_start_guard_receipt_json();

} // namespace minikv::managed_audit_receipts
