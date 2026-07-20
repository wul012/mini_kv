#include "minikv/shard_preview_candidate_material_request_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_material_request_checks;

    const auto catalog = checks::candidate_material_acceptance_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_candidate_material_acceptance_check_count() == 25);
    assert(catalog.front().check_code == std::string{"reviewer-identity-present"});
    assert(catalog.back().check_code == std::string{"cleanup-proof-present"});

    const auto partial_json = checks::format_candidate_material_acceptance_checks_json(3);
    assert_contains(partial_json, "\"checkCode\":\"reviewer-identity-present\"");
    assert_contains(partial_json, "\"checkCode\":\"source-uri-handle-present\"");
    assert_contains(partial_json, "\"checkCode\":\"source-ownership-present\"");
    assert(partial_json.find("\"checkCode\":\"source-observed-at-present\"") == std::string::npos);

    const auto full_json = checks::format_candidate_material_acceptance_checks_json(25);
    assert_contains(full_json, "\"checkCode\":\"canonical-body-present\"");
    assert_contains(full_json, "\"checkCode\":\"source-intake-guards-aligned\"");
    assert_contains(full_json, "\"checkCode\":\"cleanup-proof-present\"");
    assert_contains(full_json, "\"materialPresent\":false");
    assert_contains(full_json, "\"checkSatisfiedByRuntime\":false");
    assert_contains(full_json, "\"materialImported\":false");
    assert_contains(full_json, "\"candidateDocumentEvaluated\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_candidate_material_acceptance_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
