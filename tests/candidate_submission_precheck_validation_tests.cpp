#include "minikv/shard_preview_candidate_submission_precheck_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_submission_precheck_validation;

    const auto passed =
        validation::format_candidate_submission_precheck_validation_json(25, true, 10, 10, 10, 10, 10, 10);
    assert_contains(passed, "\"candidateSubmissionPrecheckValidationPassed\":true");
    assert_contains(passed, "\"sourceIntegrityFrozen\":true");
    assert_contains(passed, "\"checkpointCountsAligned\":true");
    assert_contains(passed, "\"validatorCountsAligned\":true");
    assert_contains(passed, "\"additionalRequestEchoCreated\":false");
    assert_contains(passed, "\"additionalSubmissionPrecheckEchoCreated\":false");
    assert_contains(passed, "\"documentIntakeOpened\":false");
    assert_contains(passed, "\"documentRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed =
        validation::format_candidate_submission_precheck_validation_json(24, true, 10, 10, 10, 10, 10, 10);
    assert_contains(failed, "\"candidateSubmissionPrecheckValidationPassed\":false");
    assert_contains(failed, "\"sourceIntegrityFrozen\":false");
    return 0;
}
