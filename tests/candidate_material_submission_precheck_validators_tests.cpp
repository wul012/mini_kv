#include "minikv/shard_preview_candidate_material_submission_precheck_validators.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validators = minikv::shard_preview_candidate_material_submission_precheck_validators;

    const auto catalog = validators::candidate_material_submission_precheck_validators();
    assert(catalog.size() == 10);
    assert(validators::planned_candidate_material_submission_precheck_validator_count() == 10);
    assert(catalog.front().sequence == 1);
    assert(catalog.front().validator_code == std::string{"source-material-request-integrity-frozen"});
    assert(catalog.back().sequence == 10);
    assert(catalog.back().validator_code == std::string{"no-execution-closeout-guard"});

    const auto json = validators::format_candidate_material_submission_precheck_validators_json(10);
    assert_contains(json, "\"validatorCode\":\"source-material-request-integrity-frozen\"");
    assert_contains(json, "\"validatorCode\":\"node-v1456-plan-pinned\"");
    assert_contains(json, "\"validatorCode\":\"no-execution-closeout-guard\"");
    assert_contains(json, "\"materialSubmissionPrecheckOnly\":true");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)validators::format_candidate_material_submission_precheck_validators_json(11);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
