#include "minikv/runtime_evidence_receipts.hpp"
#include "receipt_fixture_parity.hpp"
#include "runtime_receipt_json_builder.hpp"
#include "runtime_receipt_parity_support.hpp"
#include "test_support.hpp"

#include <cassert>
#include <filesystem>
#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

using minikv::runtime_receipt_json::json_array;
using minikv::runtime_receipt_json::json_bool;
using minikv::runtime_receipt_json::json_integer;
using minikv::runtime_receipt_json::json_object;
using minikv::runtime_receipt_json::json_string;
using minikv::runtime_receipt_json::OrderedJsonField;
using minikv::test_support::count_occurrences;
using minikv::test_support::count_top_level_fields;
using minikv::test_support::extract_json_object_field;
using minikv::test_support::named_apostrophe_runtime_surface_canonical_json;
using minikv::test_support::read_fixture_text;
using minikv::test_support::release_fixture_path;
using minikv::test_support::stable_fixture_digest;

constexpr std::string_view no_network_fixture_legacy_phrase = "Node v323\\u0027s no-network fixture contract";
constexpr std::string_view no_network_runtime_canonical_phrase = "Node v323's no-network fixture contract";
constexpr std::string_view signed_human_fixture_legacy_phrase = "Node v314\\u0027s non-secret contract";
constexpr std::string_view signed_human_runtime_canonical_phrase = "Node v314's non-secret contract";
constexpr std::string_view approval_prerequisite_fixture_legacy_phrase =
    "Node v306\\u0027s non-secret artifact contract";
constexpr std::string_view approval_prerequisite_runtime_canonical_phrase = "Node v306's non-secret artifact contract";
constexpr std::string_view human_approval_review_fixture_legacy_phrase =
    "Node v308\\u0027s contract-only human approval artifact review packet";
constexpr std::string_view human_approval_review_runtime_canonical_phrase =
    "Node v308's contract-only human approval artifact review packet";
constexpr std::string_view human_approval_post_echo_fixture_legacy_phrase =
    "Node v310\\u0027s blocked post-echo decision gate";
constexpr std::string_view human_approval_post_echo_runtime_canonical_phrase =
    "Node v310's blocked post-echo decision gate";

std::string no_network_runtime_surface_canonical_json(std::string fixture_object_json) {
    return named_apostrophe_runtime_surface_canonical_json(
        std::move(fixture_object_json), no_network_fixture_legacy_phrase, no_network_runtime_canonical_phrase);
}

std::string signed_human_runtime_surface_canonical_json(std::string fixture_object_json) {
    return named_apostrophe_runtime_surface_canonical_json(
        std::move(fixture_object_json), signed_human_fixture_legacy_phrase, signed_human_runtime_canonical_phrase);
}

void assert_formatter_matches_fixture_object(std::string_view fixture_name, std::string_view object_field_name,
                                             const std::string& current_json,
                                             std::initializer_list<std::string_view> required_fragments) {
    const auto fixture = read_fixture_text(release_fixture_path(std::string{fixture_name}));
    const auto fixture_object = extract_json_object_field(fixture, std::string{object_field_name});
    assert(fixture_object.has_value());
    if (*fixture_object != current_json) {
        std::size_t mismatch_at = 0;
        const auto limit = std::min(fixture_object->size(), current_json.size());
        while (mismatch_at < limit && (*fixture_object)[mismatch_at] == current_json[mismatch_at]) {
            ++mismatch_at;
        }
        std::cerr << "fixture parity mismatch: " << fixture_name << " field=" << object_field_name
                  << " fixture_size=" << fixture_object->size() << " current_size=" << current_json.size()
                  << " mismatch_at=" << mismatch_at
                  << " fixture_digest=" << stable_fixture_digest("fixture", *fixture_object)
                  << " current_digest=" << stable_fixture_digest("current", current_json) << '\n';
    }
    assert(*fixture_object == current_json);
    for (const auto fragment : required_fragments) {
        assert(current_json.find(std::string{fragment}) != std::string::npos);
    }
}

void ordered_object_preserves_scalars_and_escaping() {
    const auto output = json_object({
        {"message", json_string("quote=\" slash=\\ line=\n tab=\t control=\x01")},
        {"enabled", json_bool(false)},
        {"count", json_integer(0)},
        {"empty_object", json_object({})},
        {"empty_array", json_array({})},
    });

    assert(output == "{\"message\":\"quote=\\\" slash=\\\\ line=\\n tab=\\t control=\\u0001\","
                     "\"enabled\":false,\"count\":0,\"empty_object\":{},\"empty_array\":[]}");
}

void nested_raw_values_are_not_double_escaped() {
    const auto nested = json_object({
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
    });
    const auto values = json_array({
        json_string("alpha"),
        nested,
        json_array({json_integer(0), json_bool(false)}),
    });
    const auto output = json_object({
        {"nested", nested},
        {"values", values},
    });

    assert(output == "{\"nested\":{\"read_only\":true,\"execution_allowed\":false},"
                     "\"values\":[\"alpha\",{\"read_only\":true,\"execution_allowed\":false},[0,false]]}");
}

void extractor_handles_nested_objects_and_escaped_strings() {
    const std::string document = "{\"before\":\"brace } and quote \\\" stay text\","
                                 "\"target\" : {\"nested\":{\"value\":\"{still text}\"},\"items\":[{\"id\":1}]},"
                                 "\"after\":true}";

    const auto extracted = extract_json_object_field(document, "target");
    assert(extracted.has_value());
    assert(*extracted == "{\"nested\":{\"value\":\"{still text}\"},\"items\":[{\"id\":1}]}");
}

void extractor_fails_closed_on_ambiguous_or_malformed_input() {
    assert(!extract_json_object_field("{\"other\":{}}", "target").has_value());
    assert(!extract_json_object_field("{\"target\":[]}", "target").has_value());
    assert(!extract_json_object_field("{\"target\":{\"open\":true}", "target").has_value());
    assert(!extract_json_object_field("{\"target\":{},\"target\":{}}", "target").has_value());
    assert(!extract_json_object_field("{\"target\":{\"text\":\"unterminated}}", "target").has_value());
    assert(!extract_json_object_field("{\"target\":{}}", "").has_value());
    assert(!extract_json_object_field("{\"target\":{}}", "bad\"name").has_value());
}

void frozen_fixture_subobjects_have_versioned_bytes() {
    const auto no_network_fixture = read_fixture_text(
        release_fixture_path("credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.json"));
    const auto no_network_object = extract_json_object_field(
        no_network_fixture, "credential_resolver_no_network_safety_fixture_contract_non_participation_receipt");
    assert(no_network_object.has_value());
    assert(no_network_object->size() == 17738);
    assert(stable_fixture_digest("v1626-no-network-fixture-object", *no_network_object) == "fnv1a64:794a730788ce0e87");
    assert(no_network_object->find("\"read_only\":true") != std::string::npos);
    assert(no_network_object->find("\"execution_allowed\":false") != std::string::npos);
    assert(no_network_object->find("\"network_safety_fixture_executed\":false") != std::string::npos);
    assert(no_network_object->find("\"http_request_sent\":false") != std::string::npos);
    assert(no_network_object->find("\"tcp_connection_attempted\":false") != std::string::npos);

    const auto abort_rollback_fixture = read_fixture_text(
        release_fixture_path("credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json"));
    const auto abort_rollback_object = extract_json_object_field(
        abort_rollback_fixture, "credential_resolver_abort_rollback_semantics_contract_non_participation_receipt");
    assert(abort_rollback_object.has_value());
    assert(abort_rollback_object->size() == 19183);
    assert(stable_fixture_digest("v1626-abort-rollback-fixture-object", *abort_rollback_object) ==
           "fnv1a64:0e41aded16909c6d");
    assert(abort_rollback_object->find("\"read_only\":true") != std::string::npos);
    assert(abort_rollback_object->find("\"execution_allowed\":false") != std::string::npos);
    assert(abort_rollback_object->find("\"rollback_executed\":false") != std::string::npos);
    assert(abort_rollback_object->find("\"mini_kv_write_command_allowed\":false") != std::string::npos);
    assert(abort_rollback_object->find("\"automatic_upstream_start\":false") != std::string::npos);
}

void candidate_formatters_have_explicit_fixture_parity_state() {
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};

    const auto no_network_fixture = read_fixture_text(
        release_fixture_path("credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.json"));
    const auto no_network_object = extract_json_object_field(
        no_network_fixture, "credential_resolver_no_network_safety_fixture_contract_non_participation_receipt");
    assert(no_network_object.has_value());
    const auto no_network_current = minikv::runtime_evidence_receipts::
        format_credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_json(read_commands);
    assert(*no_network_object != no_network_current);
    assert(count_top_level_fields(*no_network_object) == 118);
    assert(count_top_level_fields(no_network_current) == 118);
    assert(no_network_object->size() == no_network_current.size() + 5);
    assert(count_occurrences(*no_network_object, "\\u0027") == 1);
    assert(count_occurrences(no_network_current, "\\u0027") == 0);
    assert(count_occurrences(*no_network_object, no_network_fixture_legacy_phrase) == 1);
    assert(count_occurrences(no_network_current, no_network_runtime_canonical_phrase) == 1);
    assert(no_network_object->find("\"boundary\":\"credential resolver no-network safety fixture contract "
                                   "non-participation receipt only; mini-kv echoes Node v323\\u0027s "
                                   "no-network fixture contract") != std::string::npos);
    assert(no_network_current.find("\"boundary\":\"credential resolver no-network safety fixture contract "
                                   "non-participation receipt only; mini-kv echoes Node v323's "
                                   "no-network fixture contract") != std::string::npos);
    assert(no_network_object->find(std::string{no_network_fixture_legacy_phrase}) != std::string::npos);
    assert(no_network_current.find(std::string{no_network_runtime_canonical_phrase}) != std::string::npos);
    assert(no_network_current.find(std::string{no_network_fixture_legacy_phrase}) == std::string::npos);
    assert(no_network_runtime_surface_canonical_json(*no_network_object) == no_network_current);
    assert(no_network_current.find("\"receipt_digest\":\"fnv1a64:3d8c483c93f8acf9\"") != std::string::npos);
    assert(no_network_current.find("\"read_only\":true") != std::string::npos);
    assert(no_network_current.find("\"execution_allowed\":false") != std::string::npos);
    assert(no_network_current.find("\"network_safety_fixture_executed\":false") != std::string::npos);
    assert(no_network_current.find("\"http_request_sent\":false") != std::string::npos);
    assert(no_network_current.find("\"tcp_connection_attempted\":false") != std::string::npos);

    const auto abort_rollback_fixture = read_fixture_text(
        release_fixture_path("credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json"));
    const auto abort_rollback_object = extract_json_object_field(
        abort_rollback_fixture, "credential_resolver_abort_rollback_semantics_contract_non_participation_receipt");
    assert(abort_rollback_object.has_value());
    const auto abort_rollback_current = minikv::runtime_evidence_receipts::
        format_credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_json(read_commands);
    assert(*abort_rollback_object == abort_rollback_current);
    assert(abort_rollback_current.find("\"read_only\":true") != std::string::npos);
    assert(abort_rollback_current.find("\"execution_allowed\":false") != std::string::npos);
    assert(abort_rollback_current.find("\"rollback_executed\":false") != std::string::npos);
    assert(abort_rollback_current.find("\"mini_kv_write_command_allowed\":false") != std::string::npos);
    assert(abort_rollback_current.find("\"automatic_upstream_start\":false") != std::string::npos);
}

void approval_contract_slice_formatters_have_fixture_parity_before_builder_migration() {
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};

    const auto signed_human_fixture = read_fixture_text(release_fixture_path(
        "credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json"));
    const auto signed_human_object = extract_json_object_field(
        signed_human_fixture, "credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt");
    assert(signed_human_object.has_value());
    const auto signed_human_current = minikv::runtime_evidence_receipts::
        format_credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_json(
            read_commands);
    assert(*signed_human_object != signed_human_current);
    const auto signed_human_field_count = count_top_level_fields(*signed_human_object);
    assert(signed_human_field_count == count_top_level_fields(signed_human_current));
    assert(signed_human_field_count >= 100);
    assert(signed_human_object->size() == signed_human_current.size() + 5);
    assert(count_occurrences(*signed_human_object, "\\u0027") == 1);
    assert(count_occurrences(signed_human_current, "\\u0027") == 0);
    assert(count_occurrences(*signed_human_object, signed_human_fixture_legacy_phrase) == 1);
    assert(count_occurrences(signed_human_current, signed_human_runtime_canonical_phrase) == 1);
    assert(signed_human_current.find(std::string{signed_human_fixture_legacy_phrase}) == std::string::npos);
    assert(signed_human_runtime_surface_canonical_json(*signed_human_object) == signed_human_current);
    assert(signed_human_current.find("\"read_only\":true") != std::string::npos);
    assert(signed_human_current.find("\"execution_allowed\":false") != std::string::npos);
    assert(signed_human_current.find("\"credential_value_read\":false") != std::string::npos);
    assert(signed_human_current.find("\"raw_endpoint_url_parsed\":false") != std::string::npos);
    assert(signed_human_current.find("\"external_request_sent\":false") != std::string::npos);
    assert(signed_human_current.find("\"approval_ledger_written\":false") != std::string::npos);
    assert(signed_human_current.find("\"automatic_upstream_start\":false") != std::string::npos);
    assert(signed_human_current.find(
               "\"signed_human_approval_artifact_contract_non_participation_receipt_only\":true") != std::string::npos);

    assert_formatter_matches_fixture_object(
        "credential-resolver-credential-handle-approval-contract-non-participation-receipt.json",
        "credential_resolver_credential_handle_approval_contract_non_participation_receipt",
        minikv::runtime_evidence_receipts::
            format_credential_resolver_credential_handle_approval_contract_non_participation_receipt_json(
                read_commands),
        {"\"read_only\":true", "\"execution_allowed\":false", "\"credential_value_read\":false",
         "\"raw_endpoint_url_parsed\":false", "\"external_request_sent\":false", "\"approval_ledger_written\":false",
         "\"automatic_upstream_start\":false",
         "\"credential_handle_approval_contract_non_participation_receipt_only\":true"});

    assert_formatter_matches_fixture_object(
        "credential-resolver-endpoint-handle-allowlist-approval-contract-non-participation-receipt.json",
        "credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt",
        minikv::runtime_evidence_receipts::
            format_credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_json(
                read_commands),
        {"\"read_only\":true", "\"execution_allowed\":false", "\"credential_value_read\":false",
         "\"raw_endpoint_url_parsed\":false", "\"external_request_sent\":false", "\"approval_ledger_written\":false",
         "\"automatic_upstream_start\":false",
         "\"endpoint_handle_allowlist_approval_contract_non_participation_receipt_only\":true"});
}

void approval_lifecycle_candidates_have_frozen_parity_baseline() {
    using Formatter = std::string (*)(const std::vector<std::string>&);
    struct Candidate {
        std::string_view fixture_name;
        std::string_view field_name;
        std::string_view digest_prefix;
        std::string_view expected_fixture_digest;
        std::size_t expected_fixture_size;
        std::size_t expected_top_level_fields;
        std::string_view fixture_legacy_phrase;
        std::string_view runtime_canonical_phrase;
        std::string_view mode_fragment;
        bool migration_ready;
        Formatter formatter;
    };

    const Candidate candidates[] = {
        {"credential-resolver-approval-required-boundary-non-participation-receipt.json",
         "credential_resolver_approval_required_boundary_non_participation_receipt",
         "v1645-approval-required-boundary-fixture-object",
         "fnv1a64:d401383f6006f7b0",
         12949,
         108,
         {},
         {},
         "\"approval_required_boundary_non_participation_receipt_only\":true",
         true,
         minikv::runtime_evidence_receipts::
             format_credential_resolver_approval_required_boundary_non_participation_receipt_json},
        {"credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json",
         "credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt",
         "v1645-approval-prerequisite-artifact-fixture-object", "fnv1a64:bae24c5eb4acc9f3", 16483, 107,
         approval_prerequisite_fixture_legacy_phrase, approval_prerequisite_runtime_canonical_phrase,
         "\"approval_prerequisite_artifact_intake_non_participation_receipt_only\":true", true,
         minikv::runtime_evidence_receipts::
             format_credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_json},
        {"credential-resolver-human-approval-artifact-review-non-participation-receipt.json",
         "credential_resolver_human_approval_artifact_review_non_participation_receipt",
         "v1645-human-approval-review-fixture-object", "fnv1a64:14567498219a39ad", 17116, 107,
         human_approval_review_fixture_legacy_phrase, human_approval_review_runtime_canonical_phrase,
         "\"human_approval_artifact_review_non_participation_receipt_only\":true", true,
         minikv::runtime_evidence_receipts::
             format_credential_resolver_human_approval_artifact_review_non_participation_receipt_json},
        {"credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json",
         "credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt",
         "v1645-human-approval-post-echo-fixture-object", "fnv1a64:2a534ddf7be23111", 14169, 101,
         human_approval_post_echo_fixture_legacy_phrase, human_approval_post_echo_runtime_canonical_phrase,
         "\"human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only\":true", true,
         minikv::runtime_evidence_receipts::
             format_credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_json},
    };
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};

    for (const auto& candidate : candidates) {
        const auto fixture = read_fixture_text(release_fixture_path(std::string{candidate.fixture_name}));
        const auto fixture_object = extract_json_object_field(fixture, std::string{candidate.field_name});
        assert(fixture_object.has_value());
        const auto current = candidate.formatter(read_commands);
        assert(fixture_object->size() == candidate.expected_fixture_size);
        assert(stable_fixture_digest(candidate.digest_prefix, *fixture_object) == candidate.expected_fixture_digest);
        assert(count_top_level_fields(*fixture_object) == candidate.expected_top_level_fields);
        assert(count_top_level_fields(current) == candidate.expected_top_level_fields);
        assert(current.find("\"read_only\":true") != std::string::npos);
        assert(current.find("\"execution_allowed\":false") != std::string::npos);
        assert(current.find("\"external_request_sent\":false") != std::string::npos);
        assert(current.find("\"approval_ledger_written\":false") != std::string::npos);
        assert(current.find("\"automatic_upstream_start\":false") != std::string::npos);
        assert(current.find(std::string{candidate.mode_fragment}) != std::string::npos);

        if (candidate.fixture_legacy_phrase.empty()) {
            assert(candidate.runtime_canonical_phrase.empty());
            assert(candidate.migration_ready);
            assert(count_occurrences(*fixture_object, "\\u0027") == 0);
            assert(count_occurrences(current, "\\u0027") == 0);
            assert(*fixture_object == current);
            continue;
        }

        assert(!candidate.runtime_canonical_phrase.empty());
        assert(count_occurrences(current, "\\u0027") == 0);
        assert(count_occurrences(current, candidate.runtime_canonical_phrase) == 1);
        assert(current.find(std::string{candidate.fixture_legacy_phrase}) == std::string::npos);
        const auto canonical_fixture = named_apostrophe_runtime_surface_canonical_json(
            *fixture_object, candidate.fixture_legacy_phrase, candidate.runtime_canonical_phrase);
        assert(candidate.migration_ready);
        assert(fixture_object->size() == current.size() + 5);
        assert(canonical_fixture == current);
    }
}

} // namespace

int main() {
    ordered_object_preserves_scalars_and_escaping();
    nested_raw_values_are_not_double_escaped();
    extractor_handles_nested_objects_and_escaped_strings();
    extractor_fails_closed_on_ambiguous_or_malformed_input();
    frozen_fixture_subobjects_have_versioned_bytes();
    candidate_formatters_have_explicit_fixture_parity_state();
    approval_contract_slice_formatters_have_fixture_parity_before_builder_migration();
    approval_lifecycle_candidates_have_frozen_parity_baseline();
    return 0;
}
