#include "minikv/runtime_evidence_receipts.hpp"
#include "minikv/runtime_evidence.hpp"
#include "receipt_fixture_parity.hpp"
#include "runtime_receipt_json_builder.hpp"
#include "test_support.hpp"

#include <cassert>
#include <filesystem>
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
using minikv::test_support::extract_json_object_field;
using minikv::test_support::read_fixture_text;
using minikv::test_support::release_fixture_path;

std::string stable_fixture_digest(std::string_view prefix, const std::string& object_json) {
    return minikv::runtime_evidence::digest(prefix, {{object_json}});
}

std::string replace_once(std::string text, std::string_view from, std::string_view to) {
    const auto position = text.find(std::string{from});
    assert(position != std::string::npos);
    assert(text.find(std::string{from}, position + from.size()) == std::string::npos);
    text.replace(position, from.size(), std::string{to});
    return text;
}

std::size_t count_occurrences(std::string_view text, std::string_view needle) {
    assert(!needle.empty());
    std::size_t count = 0;
    for (std::size_t position = text.find(needle); position != std::string_view::npos;
         position = text.find(needle, position + needle.size())) {
        ++count;
    }
    return count;
}

std::size_t count_top_level_fields(std::string_view object_json) {
    assert(!object_json.empty() && object_json.front() == '{' && object_json.back() == '}');

    std::size_t depth = 0;
    std::size_t count = 0;
    bool in_string = false;
    bool escaped = false;
    bool top_level_string_candidate = false;

    for (std::size_t index = 0; index < object_json.size(); ++index) {
        const char ch = object_json[index];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
                if (top_level_string_candidate) {
                    std::size_t cursor = index + 1;
                    while (cursor < object_json.size() &&
                           (object_json[cursor] == ' ' || object_json[cursor] == '\t' || object_json[cursor] == '\n' ||
                            object_json[cursor] == '\r')) {
                        ++cursor;
                    }
                    if (cursor < object_json.size() && object_json[cursor] == ':') {
                        ++count;
                    }
                }
                top_level_string_candidate = false;
            }
            continue;
        }

        if (ch == '"') {
            in_string = true;
            top_level_string_candidate = depth == 1;
        } else if (ch == '{' || ch == '[') {
            ++depth;
        } else if (ch == '}' || ch == ']') {
            assert(depth > 0);
            --depth;
        }
    }

    assert(depth == 0);
    assert(!in_string);
    return count;
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
    assert(count_occurrences(*no_network_object, "Node v323\\u0027s no-network fixture contract") == 1);
    assert(count_occurrences(no_network_current, "Node v323's no-network fixture contract") == 1);
    assert(no_network_object->find("\"boundary\":\"credential resolver no-network safety fixture contract "
                                   "non-participation receipt only; mini-kv echoes Node v323\\u0027s "
                                   "no-network fixture contract") != std::string::npos);
    assert(no_network_current.find("\"boundary\":\"credential resolver no-network safety fixture contract "
                                   "non-participation receipt only; mini-kv echoes Node v323's "
                                   "no-network fixture contract") != std::string::npos);
    assert(no_network_object->find("Node v323\\u0027s no-network fixture contract") != std::string::npos);
    assert(no_network_current.find("Node v323's no-network fixture contract") != std::string::npos);
    assert(no_network_current.find("Node v323\\u0027s no-network fixture contract") == std::string::npos);
    assert(replace_once(*no_network_object, "Node v323\\u0027s no-network fixture contract",
                        "Node v323's no-network fixture contract") == no_network_current);
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

} // namespace

int main() {
    ordered_object_preserves_scalars_and_escaping();
    nested_raw_values_are_not_double_escaped();
    extractor_handles_nested_objects_and_escaped_strings();
    extractor_fails_closed_on_ambiguous_or_malformed_input();
    frozen_fixture_subobjects_have_versioned_bytes();
    candidate_formatters_have_explicit_fixture_parity_state();
    return 0;
}
