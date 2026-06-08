#include "minikv/shard_preview_candidate_request_package_gates.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace gates = minikv::shard_preview_candidate_request_package_gates;
    const auto records = gates::candidate_document_request_gates();

    assert(records.size() == 38);
    assert(gates::planned_candidate_document_request_gate_count() == 38);
    assert(records.front().gate_code == std::string{"source-intake-fixture-gate"});
    assert(records.back().gate_code == std::string{"closeout-read-only-gate"});

    const auto none = gates::format_candidate_document_request_gates_json(0);
    assert(none == "[]");

    const auto first = gates::format_candidate_document_request_gates_json(1);
    assert_contains(first, "\"gateCode\":\"source-intake-fixture-gate\"");
    assert(first.find("closeout-read-only-gate") == std::string::npos);

    const auto all = gates::format_candidate_document_request_gates_json(99);
    assert_contains(all, "\"gateCode\":\"closeout-read-only-gate\"");
    assert_contains(all, "\"gateBypassed\":false");
    assert_contains(all, "\"executionAllowed\":false");
}
