#include "minikv/shard_preview_candidate_submission_precheck_validators.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validators = minikv::shard_preview_candidate_submission_precheck_validators;

    const auto catalog = validators::candidate_submission_precheck_validators();
    assert(catalog.size() == 10);
    assert(validators::planned_candidate_submission_precheck_validator_count() == 10);
    assert(catalog.front().validator_code == std::string{"source-integrity-fixture-frozen"});
    assert(catalog.back().validator_code == std::string{"no-extra-precheck-echo-guard"});

    const auto partial_json = validators::format_candidate_submission_precheck_validators_json(3);
    assert_contains(partial_json, "\"validatorCode\":\"source-integrity-fixture-frozen\"");
    assert_contains(partial_json, "\"validatorCode\":\"node-v1411-plan-pinned\"");
    assert_contains(partial_json, "\"validatorCode\":\"request-package-lineage-carried\"");
    assert(partial_json.find("\"validatorCode\":\"requested-field-count-carried\"") == std::string::npos);

    const auto full_json = validators::format_candidate_submission_precheck_validators_json(10);
    assert_contains(full_json, "\"validatorCode\":\"payload-intake-guard\"");
    assert_contains(full_json, "\"validatorCode\":\"router-write-wal-guard\"");
    assert_contains(full_json, "\"validatorCode\":\"no-extra-precheck-echo-guard\"");
    assert_contains(full_json, "\"documentIntakeOpened\":false");
    assert_contains(full_json, "\"candidatePayloadImported\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)validators::format_candidate_submission_precheck_validators_json(11);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
