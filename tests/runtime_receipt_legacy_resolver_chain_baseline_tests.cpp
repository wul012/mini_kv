#include "minikv/runtime_evidence_receipts.hpp"

#include "receipt_fixture_parity.hpp"
#include "runtime_receipt_parity_support.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

using Formatter = std::string (*)(const std::vector<std::string>&);

struct Candidate {
    std::string_view fixture_name;
    std::string_view object_field;
    std::string_view digest_prefix;
    std::string_view expected_digest;
    std::size_t expected_bytes;
    std::size_t expected_fields;
    std::string_view boundary_marker;
    Formatter formatter;
};

std::string read_fixture(std::string_view fixture_name) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / "fixtures" / "release" / fixture_name;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}

void print_first_mismatch(std::string_view fixture, std::string_view runtime) {
    std::size_t index = 0;
    while (index < fixture.size() && index < runtime.size() && fixture[index] == runtime[index]) {
        ++index;
    }
    const auto begin = index > 50 ? index - 50 : 0;
    const auto fixture_count = std::min<std::size_t>(120, fixture.size() - begin);
    const auto runtime_count = std::min<std::size_t>(120, runtime.size() - begin);
    std::cerr << "first_mismatch=" << index << '\n'
              << "fixture_context=" << fixture.substr(begin, fixture_count) << '\n'
              << "runtime_context=" << runtime.substr(begin, runtime_count) << '\n';
}

} // namespace

int main() {
    using minikv::runtime_evidence_receipts::
        format_credential_resolver_disabled_implementation_candidate_non_participation_receipt_json;
    using minikv::runtime_evidence_receipts::
        format_credential_resolver_fake_shell_archive_non_participation_receipt_json;
    using minikv::runtime_evidence_receipts::
        format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json;
    using minikv::runtime_evidence_receipts::
        format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json;
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
        {"credential-resolver-fake-shell-archive-non-participation-receipt.json",
         "credential_resolver_fake_shell_archive_non_participation_receipt",
         "v1651-legacy-resolver-archive-fixture-object", "fnv1a64:13e45250210526fe", 12335, 95,
         "\"credential_resolver_fake_shell_archive_non_participation_receipt_only\":true",
         format_credential_resolver_fake_shell_archive_non_participation_receipt_json},
        {"credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json",
         "credential_resolver_production_readiness_blocked_decision_non_participation_receipt",
         "v1651-legacy-resolver-blocked-decision-fixture-object", "fnv1a64:10793a18910b09d2", 10623, 99,
         "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt_only\":true",
         format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json},
        {"credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json",
         "credential_resolver_pre_implementation_plan_intake_non_participation_receipt",
         "v1651-legacy-resolver-plan-intake-fixture-object", "fnv1a64:b5bf23a427aa5aae", 11114, 101,
         "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt_only\":true",
         format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json},
        {"credential-resolver-disabled-implementation-candidate-non-participation-receipt.json",
         "credential_resolver_disabled_implementation_candidate_non_participation_receipt",
         "v1651-legacy-resolver-disabled-candidate-fixture-object", "fnv1a64:a602d27c08315d04", 15160, 106,
         "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt_only\":true",
         format_credential_resolver_disabled_implementation_candidate_non_participation_receipt_json},
    };

    for (const auto& candidate : candidates) {
        const auto fixture_text = read_fixture(candidate.fixture_name);
        const auto fixture_object = extract_json_object_field(fixture_text, candidate.object_field);
        assert(fixture_object.has_value());
        const auto runtime_object = candidate.formatter(read_commands);

        assert(fixture_object->size() == candidate.expected_bytes);
        assert(runtime_object.size() == candidate.expected_bytes);
        assert(count_top_level_fields(*fixture_object) == candidate.expected_fields);
        assert(count_top_level_fields(runtime_object) == candidate.expected_fields);
        assert(stable_fixture_digest(candidate.digest_prefix, *fixture_object) == candidate.expected_digest);
        assert(count_occurrences(*fixture_object, "\\u0027") == 0);
        assert(count_occurrences(runtime_object, "\\u0027") == 0);

        assert(fixture_object->find(candidate.boundary_marker) != std::string::npos);
        assert(runtime_object.find(candidate.boundary_marker) != std::string::npos);
        const std::vector<std::string_view> required_boundaries = {
            "\"read_only\":true",
            "\"execution_allowed\":false",
            "\"credential_value_read_allowed\":false",
            "\"raw_endpoint_url_parsed\":false",
            "\"external_request_sent\":false",
            "\"write_commands_executed\":false",
            "\"load_restore_compact_executed\":false",
            "\"order_authoritative\":false",
        };
        for (const auto boundary : required_boundaries) {
            assert(runtime_object.find(boundary) != std::string::npos);
        }
        if (*fixture_object != runtime_object) {
            print_first_mismatch(*fixture_object, runtime_object);
        }
        assert(*fixture_object == runtime_object);
    }
    return 0;
}
