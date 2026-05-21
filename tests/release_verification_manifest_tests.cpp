#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string read_file_text(const std::filesystem::path& path) {
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

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    return read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path);
}

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

} // namespace

int main() {
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const auto manifest = read_fixture_text(manifest_path);

    assert_contains(manifest, "\"manifest_version\":\"mini-kv-release-verification-manifest.v1\"");
    assert_contains(manifest, "\"project\":\"mini-kv\"");
    assert_contains(manifest, "\"project_version\":\"0.102.0\"");
    assert_contains(manifest, "\"release_version\":\"v102\"");
    assert_contains(manifest, "\"read_only\":true");
    assert_contains(manifest, "\"execution_allowed\":false");
    assert_contains(manifest, "\"order_authoritative\":false");
    assert_contains(manifest, "\"no_runtime_write_command_added\":true");
    assert_contains(manifest,
                    "\"consumer_hint\":\"Node v246 manual sandbox connection precheck upstream receipt verification\"");

    assert_contains(manifest, "\"command\":\"cmake -S . -B cmake-build-v102");
    assert_contains(manifest, "\"command\":\"cmake --build cmake-build-v102 --parallel 2\"");
    assert_contains(manifest, "\"command\":\"ctest --test-dir cmake-build-v102 --output-on-failure\"");
    assert_contains(manifest, "\"minikv_command_tests\"");
    assert_contains(manifest, "\"minikv_readonly_fixture_tests\"");
    assert_contains(manifest, "\"minikv_recovery_fixture_index_tests\"");
    assert_contains(manifest, "\"minikv_ttl_token_recovery_tests\"");
    assert_contains(manifest, "\"minikv_release_verification_manifest_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_rollback_evidence_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_bundle_manifest_tests\"");
    assert_contains(manifest, "\"minikv_restore_compatibility_handoff_tests\"");
    assert_contains(manifest, "\"minikv_restore_dry_run_operator_package_tests\"");
    assert_contains(manifest, "\"minikv_artifact_digest_compatibility_matrix_tests\"");
    assert_contains(manifest, "\"minikv_release_artifact_digest_package_tests\"");
    assert_contains(manifest, "\"minikv_restore_drill_evidence_tests\"");
    assert_contains(manifest, "\"minikv_restore_handoff_checklist_tests\"");
    assert_contains(manifest, "\"minikv_restore_evidence_retention_tests\"");
    assert_contains(manifest, "\"minikv_retained_restore_artifact_digest_tests\"");
    assert_contains(manifest, "\"minikv_restore_approval_boundary_tests\"");
    assert_contains(manifest, "\"minikv_restore_boundary_smoke_manifest_tests\"");
    assert_contains(manifest, "\"minikv_runtime_smoke_evidence_tests\"");
    assert_contains(manifest, "\"minikv_credential_resolver_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"minikv_test_only_resolver_shell_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"minikv_credential_resolver_impl_readiness_receipt_tests\"");
    assert_contains(manifest, "\"minikv_cred_res_exec_denied_retention_check_tests\"");
    assert_contains(manifest, "\"minikv_disabled_runtime_shell_receipt_tests\"");
    assert_contains(manifest, "\"minikv_disabled_runtime_shell_candidate_gate_receipt_tests\"");

    assert_contains(manifest, "\"SMOKEJSON\"");
    assert_contains(manifest, "\"INFOJSON\"");
    assert_contains(manifest, "\"STORAGEJSON\"");
    assert_contains(manifest, "\"HEALTH\"");
    assert_contains(manifest, "\"GET restore:real-read-token\"");
    assert_contains(manifest, "\"SMOKEJSON version matches 0.102.0\"");
    assert_contains(manifest, "\"SMOKEJSON read_only=true and execution_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes live_read_session.session_id_echo=mini-kv-live-read-v102 and read_command_list_digest=fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(manifest, "\"SMOKEJSON exposes binary_provenance.provenance_digest=fnv1a64:d9776044d393ecbc and production_binary_claimed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes retention_provenance_check.check_digest=fnv1a64:8cf7cc0df218035f and managed_audit_write_executed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes retention_provenance_replay_marker.marker_digest=fnv1a64:6b9ef93f40cbd343 and replay_executed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_adapter_restore_boundary_receipt.receipt_digest=fnv1a64:1177efbaceb52c1c and adapter_write_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes adapter_shell_non_storage_guard_receipt.consumed_release_version=v88\"");
    assert_contains(manifest, "\"SMOKEJSON exposes adapter_shell_non_storage_guard_receipt.receipt_digest=fnv1a64:0174fa831d17cea4\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.consumed_release_version=v89\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.receipt_digest=fnv1a64:9bacde73d6d07097\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.external_adapter_storage_backend=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.credential_read_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.consumed_release_version=v90\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.receipt_digest=fnv1a64:f0cae7a4ce0674c2\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_adapter_storage_backend=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.credential_value_read_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_managed_audit_state_write_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_release_version=v95\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_artifact_path_hint=c/95/\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_receipt_digest=fnv1a64:ceaed265f7f9560c\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.source_operator_packet_profile=managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.packet_mode=manual-sandbox-connection-operator-packet-only\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.marker_digest=fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.connection_execution_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.credential_value_read_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.managed_audit_write_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.node_auto_start_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.consumed_release_version=v96\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.receipt_digest=fnv1a64:69e377d01f15fd57\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.node_auto_start_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.mini_kv_auto_start_allowed=false\"");
    assert_contains(manifest, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(manifest, "managed_audit_adapter_non_authoritative_storage_receipt");
    assert_contains(manifest, "command_dispatch_quality_receipt");
    assert_contains(manifest, "\"SMOKEJSON and INFOJSON expose ci_evidence.artifact_path_hint=c/80/ and no_restore_proof=true\"");
    assert_contains(manifest, "\"SMOKEJSON and INFOJSON expose artifact_retention.retention_days=30 and github_artifact_upload_attempted=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes failure taxonomy categories and taxonomy_digest for imported window verification\"");
    assert_contains(manifest, "\"SMOKEJSON exposes operator_window.identity_neutral_proof=true for archive handoff checks\"");
    assert_contains(manifest, "\"write_commands_executed\":false");
    assert_contains(manifest, "\"admin_commands_executed\":false");
    assert_contains(manifest, "\"runtime_write_observed\":false");
    assert_contains(manifest, "\"ci_artifact_upload_executed\":false");
    assert_contains(manifest, "\"production_window_opened\":false");

    const std::vector<std::filesystem::path> required_paths = {
        manifest_path,
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-rollback-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-compatibility-handoff.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-dry-run-operator-package.json",
        std::filesystem::path{"fixtures"} / "release" / "artifact-digest-compatibility-matrix.json",
        std::filesystem::path{"fixtures"} / "release" / "release-artifact-digest-package.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-drill-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-handoff-checklist.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-evidence-retention.json",
        std::filesystem::path{"fixtures"} / "release" / "retained-restore-artifact-digest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-approval-boundary.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-boundary-smoke-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json",
        std::filesystem::path{"fixtures"} / "readonly" / "index.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "statsjson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json",
        std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json",
        std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json",
        std::filesystem::path{"fixtures"} / "recovery" / "index.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "index.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "operator-window-no-start-no-write-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "manual-sandbox-dry-run-command-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "manual-sandbox-connection-precheck-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "sandbox-endpoint-handle-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "credential-resolver-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "test-only-resolver-shell-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "credential-resolver-fake-shell-archive-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-disabled-implementation-candidate-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-approval-required-boundary-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-disabled-runtime-shell-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(manifest, "\"path\":\"" + path.generic_string() + "\"");
    }

    assert_contains(manifest, "\"cmake_project_version\":\"0.102.0\"");
    assert_contains(manifest, "\"generated_header\":\"include/minikv/version.hpp.in\"");
    assert_contains(manifest, "\"managed_audit_receipts_header\":\"include/minikv/managed_audit_receipts.hpp\"");
    assert_contains(manifest, "\"managed_audit_receipts_source\":\"src/managed_audit_receipts.cpp\"");
    assert_contains(manifest, "\"runtime_evidence_receipts_header\":\"include/minikv/runtime_evidence_receipts.hpp\"");
    assert_contains(manifest, "\"runtime_evidence_receipts_source\":\"src/runtime_evidence_receipts.cpp\"");
    assert_contains(manifest, "\"runtime_sandbox_receipts_source\":\"src/runtime_sandbox_receipts.cpp\"");
    assert_contains(manifest, "\"credential_resolver_non_participation_receipt\"");
    assert_contains(manifest, "SMOKEJSON exposes credential_resolver_non_participation_receipt for Node v261");
    assert_contains(manifest, "\"test_only_resolver_shell_non_participation_receipt\"");
    assert_contains(manifest, "SMOKEJSON exposes test_only_resolver_shell_non_participation_receipt for Node v265");
    assert_contains(manifest, "\"credential_resolver_fake_shell_archive_non_participation_receipt\"");
    assert_contains(manifest, "SMOKEJSON exposes credential_resolver_fake_shell_archive_non_participation_receipt for Node v267");
    assert_contains(manifest, "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_production_readiness_blocked_decision_non_participation_receipt for Node v269");
    assert_contains(manifest, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_pre_implementation_plan_intake_non_participation_receipt for Node v272");
    assert_contains(manifest, "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_implementation_candidate_non_participation_receipt for Node v274");
    assert_contains(manifest, "\"credential_resolver_approval_required_boundary_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_approval_required_boundary_non_participation_receipt for Node v275");
    assert_contains(manifest,
                    "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_approval_required_implementation_readiness_non_participation_receipt for Node v282");
    assert_contains(
        manifest,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1\"");
    assert_contains(manifest,
                    "\"source_review_state\":\"credential-resolver-approval-required-implementation-readiness-review-ready\"");
    assert_contains(manifest,
                    "\"readiness_review\":{\"review_mode\":\"node-v281-approval-required-implementation-readiness-review-only\"");
    assert_contains(manifest, "\"required_artifact_count\":18");
    assert_contains(manifest, "\"approval_required_implementation_readiness_non_participation_receipt_only\":true");
    assert_contains(manifest, "\"ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_contains(manifest, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:b6e1efa8878b64d6\"");
    assert_contains(
        manifest,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification.v1\"");
    assert_contains(
        manifest,
        "\"source_verification_state\":\"credential-resolver-disabled-candidate-upstream-echo-verification-ready\"");
    assert_contains(manifest, "\"source_node_v274_verification\":");
    assert_contains(manifest, "\"source_node_v274_ready\":true");
    assert_contains(manifest, "\"source_node_v274_read_only\":true");
    assert_contains(manifest, "\"approval_required_boundary_count_preserved\":true");
    assert_contains(manifest, "\"boundary_count\":6");
    assert_contains(
        manifest,
        "\"boundary_codes\":[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]");
    assert_contains(manifest, "\"approval_required_boundary_non_participation_receipt_only\":true");
    assert_contains(manifest, "\"approval_required_boundary_refinement_only\":true");
    assert_contains(manifest, "\"ready_for_java_v115_parallel_echo\":true");
    assert_contains(manifest, "\"ready_for_node_v275_upstream_echo\":true");
    assert_contains(manifest, "\"plan_mode\":\"plan-intake-only\"");
    assert_contains(manifest, "\"boundary_count\":10");
    assert_contains(manifest, "\"defined_boundary_count\":10");
    assert_contains(manifest, "\"missing_boundary_count\":0");
    assert_contains(manifest, "\"prohibited_action_count\":21");
    assert_contains(manifest, "\"plan_digest\":\"3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8\"");
    assert_contains(manifest, "\"intake_digest\":\"43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603\"");
    assert_contains(manifest, "\"source_readiness_decision\":\"blocked\"");
    assert_contains(manifest, "\"missing_pre_implementation_requirement_count\":10");
    assert_contains(manifest, "\"production_blocker_count\":10");
    assert_contains(manifest, "\"real_resolver_implementation_allowed\":false");
    assert_contains(manifest, "\"source_archive_verification_state\":\"credential-resolver-fake-shell-archive-verification-ready\"");
    assert_contains(manifest, "\"archive_file_count\":9");
    assert_contains(manifest, "\"required_snippet_count\":24");
    assert_contains(manifest, "\"matched_snippet_count\":24");
    assert_contains(manifest, "\"archive_files_read_by_mini_kv\":false");
    assert_contains(manifest, "\"source_request_shape_field_count\":9");
    assert_contains(manifest, "\"source_response_shape_field_count\":13");
    assert_contains(manifest, "\"source_failure_mapping_count\":7");
    assert_contains(manifest, "\"source_guard_condition_count\":10");
    assert_contains(manifest, "\"fake_resolver_probe_executed\":false");
    assert_contains(manifest, "\"source_required_decision_field_count\":8");
    assert_contains(manifest, "\"source_explicit_no_go_condition_count\":9");
    assert_contains(manifest, "\"credential_resolver_invoked\":false");
    assert_contains(manifest, "\"secret_provider_instantiated\":false");
    assert_contains(manifest, "\"credential_value_stored\":false");
    assert_contains(
        manifest,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check for Node v293");
    assert_contains(
        manifest,
        "SMOKEJSON v129 retention check keeps source Node v292 readinessDecision=blocked");
    assert_contains(manifest, "\"receipt_verification_retention_check_only\":true");
    assert_contains(manifest, "\"retention_check_only\":true");
    assert_contains(manifest, "\"source_node_v292_reference\":");
    assert_contains(manifest, "\"readiness_decision\":\"blocked\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:9d433e000a555c11\"");
    assert_contains(manifest, "\"credential_resolver_disabled_runtime_shell_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_runtime_shell_non_participation_receipt for Node v296");
    assert_contains(
        manifest,
        "SMOKEJSON v130 disabled runtime shell receipt keeps source Node v295 reviewDigest=3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9");
    assert_contains(manifest, "\"disabled_runtime_shell_non_participation_receipt_only\":true");
    assert_contains(manifest, "\"source_node_v295_reference\":");
    assert_contains(manifest, "\"review_digest\":\"3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:80181b2752099581\"");
    assert_contains(manifest, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt for Node v298");
    assert_contains(
        manifest,
        "SMOKEJSON v131 candidate gate receipt keeps Node v297 gateDigest=651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22 and blocked decision");
    assert_contains(manifest, "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true");
    assert_contains(manifest, "\"source_node_v297_reference\":");
    assert_contains(manifest, "\"gate_digest\":\"651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:0557867ad4f6ed7e\"");
    assert_contains(manifest, "\"command_contracts_header\":\"include/minikv/command_contracts.hpp\"");
    assert_contains(manifest, "\"command_contracts_source\":\"src/command_contracts.cpp\"");
    assert_contains(manifest, "\"command_response_formatters_header\":\"include/minikv/command_response_formatters.hpp\"");
    assert_contains(manifest, "\"command_response_formatters_source\":\"src/command_response_formatters.cpp\"");
    assert_contains(manifest, "\"command_aggregation_source\":\"src/command.cpp\"");
    assert_contains(manifest, "\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(manifest, "\"fixtures/release/verification-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-rollback-evidence.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-bundle-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-compatibility-handoff.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-dry-run-operator-package.json\"");
    assert_contains(manifest, "\"fixtures/release/artifact-digest-compatibility-matrix.json\"");
    assert_contains(manifest, "\"fixtures/release/release-artifact-digest-package.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-drill-evidence.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-handoff-checklist.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-evidence-retention.json\"");
    assert_contains(manifest, "\"fixtures/release/retained-restore-artifact-digest.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-approval-boundary.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-boundary-smoke-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(
        manifest,
        "\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json\"");
    assert_contains(
        manifest,
        "\"fixtures/release/credential-resolver-disabled-runtime-shell-non-participation-receipt.json\"");
    assert_contains(manifest, "\"manifest only\"");
    assert_contains(manifest, "\"bundle manifest only\"");
    assert_contains(manifest, "\"restore compatibility handoff sample only\"");
    assert_contains(manifest, "\"restore dry-run operator package only\"");
    assert_contains(manifest, "\"artifact digest compatibility matrix only\"");
    assert_contains(manifest, "\"release artifact digest package only\"");
    assert_contains(manifest, "\"restore drill evidence only\"");
    assert_contains(manifest, "\"restore handoff checklist only\"");
    assert_contains(manifest, "\"restore evidence retention only\"");
    assert_contains(manifest, "\"retained restore artifact digest only\"");
    assert_contains(manifest, "\"restore approval boundary only\"");
    assert_contains(manifest, "\"restore boundary smoke manifest only\"");
    assert_contains(manifest, "\"runtime smoke evidence only\"");
    assert_contains(manifest, "\"live-read session hint only\"");
    assert_contains(manifest, "\"binary provenance hint only\"");
    assert_contains(manifest, "\"retention provenance check only\"");
    assert_contains(manifest, "\"retention provenance replay marker only\"");
    assert_contains(manifest, "\"managed audit adapter restore boundary receipt only\"");
    assert_contains(manifest, "\"CI evidence hint only\"");
    assert_contains(manifest, "\"artifact path hint only\"");
    assert_contains(manifest, "\"artifact retention evidence only\"");
    assert_contains(manifest, "\"retention dry-run only\"");
    assert_contains(manifest, "\"no production window opening\"");
    assert_contains(manifest, "\"taxonomy digest sample only\"");
    assert_contains(manifest, "\"operator-window proof only\"");
    assert_contains(manifest, "\"identity-neutral proof only\"");
    assert_contains(manifest, "\"no-restore proof only\"");
    assert_contains(manifest, "\"no CI artifact upload\"");
    assert_contains(manifest, "\"not connected to Java transaction chain\"");
    assert_contains(manifest, "\"does not perform restore\"");
    assert_contains(manifest, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(manifest, "\"no_restore_proof\":true");
    assert_contains(manifest, "\"upload_allowed\":false");
    assert_contains(manifest, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(manifest, "\"artifact_root\":\"c/\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(manifest, "\"retention_days\":30");
    assert_contains(manifest, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_contains(manifest, "\"release_evidence_ready\":true");
    assert_contains(manifest, "\"github_artifact_upload_attempted\":false");
    assert_contains(manifest, "\"production_window_allowed\":false");
    assert_contains(manifest, "\"node_action\":\"verify retention days and artifact path before cross-project retention gate\"");
    assert_contains(manifest, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(manifest, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(manifest, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_contains(manifest, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(manifest, "\"read_command_count\":4");
    assert_contains(manifest, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(manifest, "\"write_commands_allowed\":false");
    assert_contains(manifest, "\"auto_start_allowed\":false");
    assert_contains(manifest, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(manifest, "\"source_version\":\"0.102.0\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/102/\"");
    assert_contains(manifest, "\"runtime_binary_hint\":\"cmake-build-v102/minikv_server and cmake-build-v102/minikv_client from the current CMake build\"");
    assert_contains(manifest, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(manifest, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(manifest, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(manifest, "\"load_restore_compact_executed\":false");
    assert_contains(manifest, "\"production_binary_claimed\":false");
    assert_contains(manifest, "\"verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work\"");
    assert_contains(manifest, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(manifest, "\"expected_binary_provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(manifest, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_contains(manifest, "\"managed_audit_write_executed\":false");
    assert_contains(manifest, "\"verify runtime evidence retention and binary provenance alignment before managed audit dry-run packet work\"");
    assert_contains(manifest, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v84\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(manifest, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(manifest, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(manifest, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_contains(manifest, "\"replay_executed\":false");
    assert_contains(manifest, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v85\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(manifest, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_contains(manifest, "\"adapter_write_allowed\":false");
    assert_contains(manifest, "\"restore_execution_allowed\":false");
    assert_contains(manifest, "\"load_restore_compact_executed\":false");
    assert_contains(manifest, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v86\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/86/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:f39d8e3ef98654ea\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:667590b83f510a58\"");
    assert_contains(manifest, "\"managed_audit_store\":false");
    assert_contains(manifest, "\"storage_write_allowed\":false");
    assert_contains(manifest, "\"admin_commands_allowed\":false");
    assert_contains(manifest, "\"Node v208 may verify SMOKEJSON and INFOJSON binary provenance hints before managed audit persistence boundary work\"");
    assert_contains(manifest, "\"Node v211 may verify runtime evidence retention and provenance check hints before managed audit identity approval provenance dry-run packet work\"");
    assert_contains(manifest, "\"Node v213 may verify the v84 retention provenance replay marker before managed audit packet restore drill planning\"");
    assert_contains(manifest,
                    "\"node_action\":\"verify mini-kv storage is non-authoritative before managed audit adapter production-hardening readiness gate\"");
    assert_contains(manifest, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v87\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/87/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:111f0daf1283eab6\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_contains(manifest, "\"dispatch_split_applied\":true");
    assert_contains(manifest, "\"wal_snapshot_restore_touched\":false");
    assert_contains(manifest, "\"fixture_contract_preserved\":true");
    assert_contains(manifest, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v88\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/88/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_contains(manifest, "\"adapter_shell_storage_backend\":false");
    assert_contains(manifest, "\"local_dry_run_records_written\":false");
    assert_contains(manifest, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v89\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(manifest, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(manifest, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_contains(manifest, "\"external_adapter_storage_backend\":false");
    assert_contains(manifest, "\"participates_in_external_adapter\":false");
    assert_contains(manifest, "\"credential_required\":false");
    assert_contains(manifest, "\"credential_read_allowed\":false");
    assert_contains(manifest, "\"migration_required\":false");
    assert_contains(manifest, "\"migration_execution_allowed\":false");
    assert_contains(manifest, "\"schema_authoritative\":false");
    assert_contains(manifest, "\"managed_audit_write_executed\":false");
    assert_contains(manifest, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v90\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_contains(manifest, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(manifest, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(manifest, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_contains(manifest, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(manifest, "\"participates_in_sandbox_adapter\":false");
    assert_contains(manifest, "\"credential_value_required\":false");
    assert_contains(manifest, "\"credential_value_read_allowed\":false");
    assert_contains(manifest, "\"production_credential_read_allowed\":false");
    assert_contains(manifest, "\"schema_migration_execution_allowed\":false");
    assert_contains(manifest, "\"sandbox_schema_authoritative\":false");
    assert_contains(manifest, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(manifest, "\"sandbox_dry_run_records_written\":false");
    assert_contains(manifest, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v95\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_contains(manifest, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(manifest, "\"source_operator_packet_version\":\"Node v228\"");
    assert_contains(manifest, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(manifest, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_contains(manifest, "\"owner_approval_artifact_id_field\":\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_contains(manifest, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(manifest, "\"schema_rehearsal_id_field\":\"ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID\"");
    assert_contains(manifest, "\"rollback_path_id_field\":\"ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID\"");
    assert_contains(manifest, "\"timeout_budget_ms\":15000");
    assert_contains(manifest, "\"manual_abort_marker\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT\"");
    assert_contains(manifest, "\"required_operator_field_count\":6");
    assert_contains(manifest, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(manifest, "\"connection_execution_allowed\":false");
    assert_contains(manifest, "\"managed_audit_write_allowed\":false");
    assert_contains(manifest, "\"participates_in_sandbox_connection\":false");
    assert_contains(manifest, "\"node_auto_start_allowed\":false");
    assert_contains(manifest, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_contains(manifest, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(manifest, "\"consumed_release_version\":\"v96\"");
    assert_contains(manifest, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(manifest, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(manifest, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(manifest, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(manifest, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(manifest, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_contains(manifest, "\"manual_window_open_by_default\":false");
    assert_contains(manifest, "\"java_auto_start_allowed\":false");
    assert_contains(manifest, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(manifest, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(manifest, "\"Binary provenance hint is read-only path alignment evidence, not production binary attestation\"");
    assert_contains(manifest, "\"Node v205 may verify SMOKEJSON live-read session hints before the real-read execution packet\"");
    assert_contains(manifest, "\"Live-read session hint is read-only evidence, not permission to auto-start mini-kv\"");
    assert_contains(manifest, "\"Node v203 may verify SMOKEJSON and INFOJSON artifact retention evidence before the cross-project retention gate\"");
    assert_contains(manifest, "\"Artifact retention evidence is read-only rehearsal metadata, not a real GitHub artifact retention configuration\"");
    assert_contains(manifest, "\"Node v201 may still verify SMOKEJSON and INFOJSON CI evidence hints before checking the CI artifact manifest\"");
    assert_contains(manifest, "\"CI evidence hint is read-only path evidence, not a real GitHub artifact upload\"");
    assert_contains(manifest, "\"SMOKEJSON operator-window proof is identity-neutral evidence, not authentication or production authorization\"");
    assert_contains(manifest, "\"Node v223 may verify the mini-kv external adapter non-participation receipt before external adapter connection readiness review\"");
    assert_contains(manifest, "\"Managed audit external adapter non-participation receipt is read-only evidence that mini-kv is not external adapter storage and reads no credentials\"");
    assert_contains(manifest, "\"Node v225 may verify the mini-kv sandbox adapter non-participation receipt before sandbox adapter dry-run package\"");
    assert_contains(manifest, "\"Managed audit sandbox adapter non-participation receipt is read-only evidence that mini-kv is not sandbox audit storage, reads no credentials, and writes no managed audit state\"");
    assert_contains(manifest, "\"Node v231 may verify the mini-kv sandbox connection no-start guard receipt before manual sandbox connection preflight verification\"");
    assert_contains(manifest, "\"Managed audit sandbox connection receipt echo marker echoes field names only; mini-kv does not connect, read credential values, run schema rehearsal, or write managed audit state\"");
    assert_contains(manifest, "\"Managed audit receipt formatter module keeps digest/json formatting outside command.cpp while preserving read-only runtime contracts\"");
    assert_contains(manifest, "\"Runtime evidence receipt formatter module keeps live-read, taxonomy, binary provenance, retention provenance, and operator-window formatting outside command.cpp while preserving INFOJSON/SMOKEJSON field contracts\"");
    assert_contains(manifest, "\"Command contract formatter module keeps command catalog, EXPLAINJSON, and CHECKJSON formatting outside command.cpp while preserving read-only command contract fields\"");
    assert_contains(manifest, "\"Command response formatter module keeps KEYS, INFO, STATS, HEALTH, SMOKEJSON, STORAGEJSON, and WALINFO formatting outside command.cpp while preserving runtime response field contracts\"");
    assert_contains(manifest, "\"managed audit external adapter non-participation receipt only\"");
    assert_contains(manifest, "\"not external adapter storage backend\"");
    assert_contains(manifest, "\"no external adapter participation\"");
    assert_contains(manifest, "\"no credential read\"");
    assert_contains(manifest, "\"no migration execution\"");
    assert_contains(manifest, "\"managed audit sandbox adapter non-participation receipt only\"");
    assert_contains(manifest, "\"managed audit receipt formatter split only\"");
    assert_contains(manifest, "\"runtime evidence receipt formatter split only\"");
    assert_contains(manifest, "\"command contract formatter split only\"");
    assert_contains(manifest, "\"command response formatter split only\"");
    assert_contains(manifest, "\"no INFOJSON/SMOKEJSON field contract expansion from formatter split\"");
    assert_contains(manifest, "\"no INFOJSON/SMOKEJSON field contract expansion from runtime evidence receipt split\"");
    assert_contains(manifest, "\"no CHECKJSON/EXPLAINJSON field contract expansion from command contract split\"");
    assert_contains(manifest, "\"no runtime response field contract expansion from command response split\"");
    assert_contains(manifest, "\"not sandbox audit storage backend\"");
    assert_contains(manifest, "\"no sandbox adapter participation\"");
    assert_contains(manifest, "\"no sandbox credential value read\"");
    assert_contains(manifest, "\"no production credential read\"");
    assert_contains(manifest, "\"no sandbox managed audit state write\"");
    assert_contains(manifest, "\"no sandbox schema migration execution\"");
    assert_contains(manifest, "\"managed audit sandbox connection receipt echo marker only\"");
    assert_contains(manifest, "\"operator packet field echo only\"");
    assert_contains(manifest, "\"not sandbox connection participant\"");
    assert_contains(manifest, "\"no sandbox connection execution\"");
    assert_contains(manifest, "\"no credential handle value read\"");
    assert_contains(manifest, "\"no schema rehearsal execution\"");
    assert_contains(manifest, "\"no managed audit state write from connection marker\"");
    assert_contains(manifest, "\"no node auto-start from connection marker\"");

    const auto cmake_lists = read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / "CMakeLists.txt");
    assert_contains(cmake_lists, "project(mini_kv VERSION 0.102.0");
    assert_contains(cmake_lists, "src/command_contracts.cpp");
    assert_contains(cmake_lists, "src/command_response_formatters.cpp");
    assert_contains(cmake_lists, "src/managed_audit_receipts.cpp");
    assert_contains(cmake_lists, "src/runtime_evidence_receipts.cpp");
    assert_contains(cmake_lists, "src/runtime_sandbox_receipts.cpp");
    assert_contains(cmake_lists, "src/runtime_credential_resolver_approval_boundary_receipts.cpp");
    assert_contains(cmake_lists, "minikv_release_verification_manifest_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_rollback_evidence_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_bundle_manifest_tests");
    assert_contains(cmake_lists, "minikv_restore_compatibility_handoff_tests");
    assert_contains(cmake_lists, "minikv_restore_dry_run_operator_package_tests");
    assert_contains(cmake_lists, "minikv_artifact_digest_compatibility_matrix_tests");
    assert_contains(cmake_lists, "minikv_release_artifact_digest_package_tests");
    assert_contains(cmake_lists, "minikv_restore_drill_evidence_tests");
    assert_contains(cmake_lists, "minikv_restore_handoff_checklist_tests");
    assert_contains(cmake_lists, "minikv_restore_evidence_retention_tests");
    assert_contains(cmake_lists, "minikv_retained_restore_artifact_digest_tests");
    assert_contains(cmake_lists, "minikv_restore_approval_boundary_tests");
    assert_contains(cmake_lists, "minikv_restore_boundary_smoke_manifest_tests");
    assert_contains(cmake_lists, "minikv_runtime_smoke_evidence_tests");
    assert_contains(cmake_lists, "minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests");
    assert_contains(cmake_lists, "minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests");
    assert_contains(cmake_lists, "minikv_sandbox_endpoint_handle_non_participation_receipt_tests");

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.102.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
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
    assert_contains(result.response, "\"source_version\":\"0.102.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");

    result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_smoke\"");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(result.response, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(result.response, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(result.response, "\"write_commands_allowed\":false");
    assert_contains(result.response, "\"auto_start_allowed\":false");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_contains(result.response, "\"identity_neutral_proof\":true");
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
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(result.response, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(result.response, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v84\"");
    assert_contains(result.response, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_contains(result.response, "\"replay_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v85\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_contains(result.response, "\"adapter_write_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(result.response, "\"managed_audit_store\":false");
    assert_contains(result.response, "\"storage_write_allowed\":false");
    assert_contains(result.response, "\"admin_commands_allowed\":false");
    assert_contains(result.response, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_contains(result.response, "\"dispatch_split_applied\":true");
    assert_contains(result.response, "\"wal_snapshot_restore_touched\":false");
    assert_contains(result.response, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_contains(result.response, "\"adapter_shell_storage_backend\":false");
    assert_contains(result.response, "\"local_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v89\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(result.response, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_contains(result.response, "\"external_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_external_adapter\":false");
    assert_contains(result.response, "\"credential_read_allowed\":false");
    assert_contains(result.response, "\"migration_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v90\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_contains(result.response, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_sandbox_adapter\":false");
    assert_contains(result.response, "\"credential_value_required\":false");
    assert_contains(result.response, "\"credential_value_read_allowed\":false");
    assert_contains(result.response, "\"production_credential_read_allowed\":false");
    assert_contains(result.response, "\"schema_migration_execution_allowed\":false");
    assert_contains(result.response, "\"sandbox_schema_authoritative\":false");
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
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(result.response, "\"connection_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_write_allowed\":false");
    assert_contains(result.response, "\"participates_in_sandbox_connection\":false");
    assert_contains(result.response, "\"node_auto_start_allowed\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v96\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(result.response, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(result.response, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_contains(result.response, "\"manual_window_open_by_default\":false");
    assert_contains(result.response, "\"java_auto_start_allowed\":false");
    assert_contains(result.response, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(result.response, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(result.response,
                    "\"node_consumption\":\"Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v296 may verify the mini-kv v130 credential resolver disabled runtime shell non-participation receipt");
    assert_contains(result.response,
                    "Node v293 may verify the mini-kv v129 credential resolver disabled fake harness execution-denied receipt verification/retention check");
    assert_contains(result.response,
                    "Node v291 may verify the mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt");
    assert_contains(result.response,
                    "Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt");
    assert_contains(result.response,
                    "Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");
    assert_contains(result.response,
                    "Node v282 may verify the mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt");
    assert_contains(result.response,
                    "Node v275 may verify the mini-kv v121 credential resolver approval-required boundary non-participation receipt");
    assert_contains(result.response,
                    "Node v274 may verify the mini-kv v120 credential resolver disabled implementation candidate non-participation receipt");
    assert_contains(result.response,
                    "Node v272 may verify the mini-kv v119 credential resolver pre-implementation plan intake non-participation receipt");
    assert_contains(result.response, "Node v269 may verify the mini-kv v118 credential resolver production-readiness blocked-decision non-participation receipt");
    assert_contains(result.response, "Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt");
    assert_contains(result.response, "Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt");
    assert_contains(result.response, "Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt");
    assert_contains(result.response, "Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt");
    assert_contains(result.response, "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");
    assert_contains(result.response, "Node v239 may still verify the mini-kv operator window no-start/no-write receipt");
    assert_contains(result.response, "\"manual_sandbox_connection_precheck_non_participation_receipt\"");
    assert_contains(result.response, "\"sandbox_endpoint_handle_non_participation_receipt\"");
    assert_contains(result.response, "\"test_only_resolver_shell_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_fake_shell_archive_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_approval_required_boundary_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_implementation_plan_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\"");
    assert_contains(
        result.response,
        "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\"");
    assert_contains(
        result.response,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\"");
    assert_contains(result.response, "\"credential_resolver_disabled_runtime_shell_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\"");
    assert_contains(result.response, "\"review_digest\":\"3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9\"");
    assert_contains(result.response, "\"current_release_version\":\"v130\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/130/\"");
    assert_contains(result.response, "\"current_release_version\":\"v131\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/131/\"");
    assert_contains(result.response, "\"gate_digest\":\"651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22\"");
    assert_contains(result.response, "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0557867ad4f6ed7e\"");
    assert_contains(result.response, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\"");
    assert_contains(result.response, "\"runtime_shell_implemented\":false");
    assert_contains(result.response, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(result.response, "\"write_commands_executed\":false");

    result = processor.execute("STORAGEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    return 0;
}
