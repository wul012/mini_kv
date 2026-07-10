#include "minikv/runtime_evidence_receipts.hpp"

#include "receipt_fixture_parity.hpp"
#include "runtime_receipt_parity_support.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

using Formatter = std::string (*)(const std::vector<std::string>&);

enum class CompatibilityRule {
    exact,
    resolver_decision_field_order,
    frozen_runtime_metadata_fields,
};

struct Candidate {
    std::string_view family;
    std::string_view fixture_name;
    std::string_view object_field;
    std::string_view fixture_digest;
    std::string_view runtime_digest;
    std::size_t fixture_bytes;
    std::size_t runtime_bytes;
    std::size_t fixture_fields;
    std::size_t runtime_fields;
    CompatibilityRule compatibility_rule;
    Formatter formatter;
};

std::string read_fixture(std::string_view fixture_name) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / "fixtures" / "release" / fixture_name;
    std::ifstream input{path, std::ios::binary};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot open fixture: " + path.string()};
    }
    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}

std::string canonical_runtime_surface(std::string fixture_object, CompatibilityRule rule) {
    using minikv::test_support::replace_once;

    if (rule == CompatibilityRule::resolver_decision_field_order) {
        constexpr std::string_view fixture_order =
            "\"receipt_digest\":\"fnv1a64:240de318ac5de064\",\"read_only\":true,\"execution_allowed\":false,"
            "\"dry_run_only\":true,\"credential_resolver_decision_only\":true,";
        constexpr std::string_view runtime_order =
            "\"receipt_digest\":\"fnv1a64:240de318ac5de064\",\"credential_resolver_decision_only\":true,"
            "\"read_only\":true,\"execution_allowed\":false,\"dry_run_only\":true,";
        assert(fixture_object.find(runtime_order) == std::string::npos);
        return replace_once(std::move(fixture_object), fixture_order, runtime_order);
    }
    if (rule == CompatibilityRule::frozen_runtime_metadata_fields) {
        fixture_object = replace_once(std::move(fixture_object), "\"runtime_project_version\":\"0.102.0\",", "");
        fixture_object =
            replace_once(std::move(fixture_object), "\"current_runtime_fixture_release_version\":\"v102\",", "");
        fixture_object =
            replace_once(std::move(fixture_object), "\"current_runtime_fixture_artifact_path_hint\":\"c/102/\",", "");
    }
    return fixture_object;
}

} // namespace

int main() {
    using namespace minikv::runtime_evidence_receipts;
    using minikv::test_support::count_occurrences;
    using minikv::test_support::count_top_level_fields;
    using minikv::test_support::extract_json_object_field;
    using minikv::test_support::stable_fixture_digest;

    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const std::vector<Candidate> candidates = {
        {"base-resolver", "credential-resolver-non-participation-receipt.json",
         "credential_resolver_non_participation_receipt", "fnv1a64:20da9877c6cf8724", "fnv1a64:272dd84f8f36f3ea", 7944,
         7944, 104, 104, CompatibilityRule::resolver_decision_field_order,
         format_credential_resolver_non_participation_receipt_json},
        {"base-resolver", "disabled-credential-resolver-precheck-non-participation-receipt.json",
         "disabled_credential_resolver_precheck_non_participation_receipt", "fnv1a64:0f5091489a656f50",
         "fnv1a64:0f5091489a656f50", 10291, 10291, 118, 118, CompatibilityRule::exact,
         format_disabled_credential_resolver_precheck_non_participation_receipt_json},
        {"base-resolver", "test-only-resolver-shell-non-participation-receipt.json",
         "test_only_resolver_shell_non_participation_receipt", "fnv1a64:32ac44efd69ca3d2", "fnv1a64:32ac44efd69ca3d2",
         11201, 11201, 133, 133, CompatibilityRule::exact,
         format_test_only_resolver_shell_non_participation_receipt_json},
        {"implementation-chain",
         "credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json",
         "credential_resolver_approval_required_implementation_readiness_non_participation_receipt",
         "fnv1a64:cc9b259e630b82cd", "fnv1a64:cc9b259e630b82cd", 16889, 16889, 112, 112, CompatibilityRule::exact,
         format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json},
        {"implementation-chain", "credential-resolver-implementation-plan-non-participation-receipt.json",
         "credential_resolver_implementation_plan_non_participation_receipt", "fnv1a64:bdea7f2634c384f0",
         "fnv1a64:bdea7f2634c384f0", 19163, 19163, 121, 121, CompatibilityRule::exact,
         format_credential_resolver_implementation_plan_non_participation_receipt_json},
        {"implementation-chain", "credential-resolver-disabled-fake-harness-non-participation-receipt.json",
         "credential_resolver_disabled_fake_harness_non_participation_receipt", "fnv1a64:fbc65c76469b4aab",
         "fnv1a64:fbc65c76469b4aab", 13870, 13870, 135, 135, CompatibilityRule::exact,
         format_credential_resolver_disabled_fake_harness_non_participation_receipt_json},
        {"implementation-chain",
         "credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json",
         "credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt",
         "fnv1a64:ffe9a9c891eaf2df", "fnv1a64:ffe9a9c891eaf2df", 15264, 15264, 138, 138, CompatibilityRule::exact,
         format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json},
        {"implementation-chain",
         "credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json",
         "credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt",
         "fnv1a64:86f2b243c74f9386", "fnv1a64:86f2b243c74f9386", 21562, 21562, 113, 113, CompatibilityRule::exact,
         format_credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_json},
        {"sandbox-chain", "manual-sandbox-dry-run-command-non-participation-receipt.json",
         "manual_sandbox_dry_run_command_non_participation_receipt", "fnv1a64:a0f08858e298b52b",
         "fnv1a64:260d1f03289ad0c1", 3199, 3060, 56, 53, CompatibilityRule::frozen_runtime_metadata_fields,
         format_manual_sandbox_dry_run_command_non_participation_receipt_json},
        {"sandbox-chain", "manual-sandbox-connection-precheck-non-participation-receipt.json",
         "manual_sandbox_connection_precheck_non_participation_receipt", "fnv1a64:7c137ad07478a5f0",
         "fnv1a64:7c137ad07478a5f0", 4176, 4176, 74, 74, CompatibilityRule::exact,
         format_manual_sandbox_connection_precheck_non_participation_receipt_json},
        {"sandbox-chain", "disabled-adapter-client-non-participation-receipt.json",
         "disabled_adapter_client_non_participation_receipt", "fnv1a64:4a3b015e8f721809", "fnv1a64:4a3b015e8f721809",
         5361, 5361, 92, 92, CompatibilityRule::exact, format_disabled_adapter_client_non_participation_receipt_json},
        {"sandbox-chain", "fake-transport-dry-run-packet-non-participation-receipt.json",
         "fake_transport_dry_run_packet_non_participation_receipt", "fnv1a64:46b00933b662eaa0",
         "fnv1a64:46b00933b662eaa0", 6040, 6040, 92, 92, CompatibilityRule::exact,
         format_fake_transport_dry_run_packet_non_participation_receipt_json},
        {"sandbox-chain", "sandbox-endpoint-handle-non-participation-receipt.json",
         "sandbox_endpoint_handle_non_participation_receipt", "fnv1a64:2b8d4e4c353af393", "fnv1a64:2b8d4e4c353af393",
         6340, 6340, 92, 92, CompatibilityRule::exact, format_sandbox_endpoint_handle_non_participation_receipt_json},
    };

    std::size_t exact_candidates = 0;
    std::size_t resolver_order_rules = 0;
    std::size_t metadata_field_rules = 0;
    for (const auto& candidate : candidates) {
        const auto fixture_text = read_fixture(candidate.fixture_name);
        const auto fixture_object = extract_json_object_field(fixture_text, candidate.object_field);
        assert(fixture_object.has_value());
        const auto runtime_object = candidate.formatter(read_commands);
        const auto digest_prefix = std::string{"v1653-remaining-owner-"} + std::string{candidate.object_field};

        assert(fixture_object->size() == candidate.fixture_bytes);
        assert(runtime_object.size() == candidate.runtime_bytes);
        assert(count_top_level_fields(*fixture_object) == candidate.fixture_fields);
        assert(count_top_level_fields(runtime_object) == candidate.runtime_fields);
        assert(stable_fixture_digest(digest_prefix, *fixture_object) == candidate.fixture_digest);
        assert(stable_fixture_digest(digest_prefix, runtime_object) == candidate.runtime_digest);
        assert(count_occurrences(*fixture_object, "\\u0027") == 0);
        assert(count_occurrences(runtime_object, "\\u0027") == 0);

        const auto canonical_fixture = canonical_runtime_surface(*fixture_object, candidate.compatibility_rule);
        assert(canonical_fixture == runtime_object);
        if (candidate.compatibility_rule == CompatibilityRule::exact) {
            assert(*fixture_object == runtime_object);
            ++exact_candidates;
        } else if (candidate.compatibility_rule == CompatibilityRule::resolver_decision_field_order) {
            assert(*fixture_object != runtime_object);
            ++resolver_order_rules;
        } else {
            assert(*fixture_object != runtime_object);
            ++metadata_field_rules;
        }

        const std::vector<std::string_view> closed_boundaries = {
            "\"read_only\":true",
            "\"execution_allowed\":false",
            "\"credential_value_read_allowed\":false",
            "\"managed_audit_store\":false",
            "\"storage_write_allowed\":false",
            "\"write_commands_executed\":false",
            "\"runtime_write_observed\":false",
            "\"load_restore_compact_executed\":false",
            "\"node_auto_start_allowed\":false",
            "\"order_authoritative\":false",
        };
        for (const auto boundary : closed_boundaries) {
            assert(runtime_object.find(boundary) != std::string::npos);
        }

        std::cout << candidate.family << ' ' << candidate.object_field << " fixture_bytes=" << candidate.fixture_bytes
                  << " runtime_bytes=" << candidate.runtime_bytes << " fixture_fields=" << candidate.fixture_fields
                  << " runtime_fields=" << candidate.runtime_fields
                  << " compatibility=" << (candidate.compatibility_rule == CompatibilityRule::exact ? "exact" : "named")
                  << '\n';
    }
    assert(candidates.size() == 13);
    assert(exact_candidates == 11);
    assert(resolver_order_rules == 1);
    assert(metadata_field_rules == 1);
}
