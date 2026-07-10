#include "minikv/runtime_evidence_receipts.hpp"
#include "receipt_fixture_parity.hpp"
#include "runtime_receipt_parity_support.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>
#include <vector>

namespace {

using Formatter = std::string (*)(const std::vector<std::string>&);

struct Candidate {
    std::string_view fixture_name;
    std::string_view field_name;
    std::string_view digest_prefix;
    std::string_view expected_fixture_digest;
    std::size_t expected_fixture_size;
    std::size_t expected_top_level_fields;
    std::string_view mode_fragment;
    Formatter formatter;
};

void runtime_shell_family_has_frozen_fixture_parity_baseline() {
    using minikv::test_support::count_occurrences;
    using minikv::test_support::count_top_level_fields;
    using minikv::test_support::extract_json_object_field;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::release_fixture_path;
    using minikv::test_support::stable_fixture_digest;

    const Candidate candidates[] = {
        {"credential-resolver-disabled-runtime-shell-non-participation-receipt.json",
         "credential_resolver_disabled_runtime_shell_non_participation_receipt",
         "v1648-disabled-runtime-shell-fixture-object", "fnv1a64:08807cb6305602e8", 12523, 110,
         "\"disabled_runtime_shell_non_participation_receipt_only\":true",
         minikv::runtime_evidence_receipts::
             format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json},
        {"credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json",
         "credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt",
         "v1648-disabled-runtime-shell-candidate-gate-fixture-object", "fnv1a64:d1a2681a73de4b8e", 14936, 114,
         "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true",
         minikv::runtime_evidence_receipts::
             format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json},
        {"credential-resolver-runtime-shell-decision-record-non-participation-receipt.json",
         "credential_resolver_runtime_shell_decision_record_non_participation_receipt",
         "v1648-runtime-shell-decision-record-fixture-object", "fnv1a64:9926939aac05efd6", 14208, 113,
         "\"runtime_shell_decision_record_non_participation_receipt_only\":true",
         minikv::runtime_evidence_receipts::
             format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json},
        {"credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json",
         "credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt",
         "v1648-runtime-shell-post-decision-plan-fixture-object", "fnv1a64:e6c73dbf0f4bc4d1", 14084, 113,
         "\"runtime_shell_post_decision_plan_intake_non_participation_receipt_only\":true",
         minikv::runtime_evidence_receipts::
             format_credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_json},
        {"credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json",
         "credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt",
         "v1648-runtime-shell-stop-prerequisite-fixture-object", "fnv1a64:b6984418a3b69de7", 16306, 115,
         "\"runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_only\":true",
         minikv::runtime_evidence_receipts::
             format_credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_json},
    };
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};

    for (const auto& candidate : candidates) {
        const auto fixture = read_fixture_text(release_fixture_path(std::string{candidate.fixture_name}));
        const auto fixture_object = extract_json_object_field(fixture, candidate.field_name);
        assert(fixture_object.has_value());
        const auto current = candidate.formatter(read_commands);

        assert(fixture_object->size() == candidate.expected_fixture_size);
        assert(current.size() == candidate.expected_fixture_size);
        assert(stable_fixture_digest(candidate.digest_prefix, *fixture_object) == candidate.expected_fixture_digest);
        assert(count_top_level_fields(*fixture_object) == candidate.expected_top_level_fields);
        assert(count_top_level_fields(current) == candidate.expected_top_level_fields);
        assert(count_occurrences(*fixture_object, "\\u0027") == 0);
        assert(count_occurrences(current, "\\u0027") == 0);
        assert(*fixture_object == current);

        assert(current.find("\"read_only\":true") != std::string::npos);
        assert(current.find("\"execution_allowed\":false") != std::string::npos);
        assert(current.find("\"runtime_shell_implemented\":false") != std::string::npos);
        assert(current.find("\"credential_value_read\":false") != std::string::npos);
        assert(current.find("\"raw_endpoint_url_parsed\":false") != std::string::npos);
        assert(current.find("\"external_request_sent\":false") != std::string::npos);
        assert(current.find("\"approval_ledger_written\":false") != std::string::npos);
        assert(current.find("\"automatic_upstream_start\":false") != std::string::npos);
        assert(current.find(std::string{candidate.mode_fragment}) != std::string::npos);
    }
}

} // namespace

int main() {
    runtime_shell_family_has_frozen_fixture_parity_baseline();
    return 0;
}
