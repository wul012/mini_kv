#include "minikv/shard_preview_candidate_material_submission_precheck_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_material_submission_precheck_validation;

    const auto json = validation::format_candidate_material_submission_precheck_validation_json(
        10, true, 25, 25, 20, 20, 41, 10, 10, 10, 10, 10, 10);
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckValidationPassed\":true");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityComplete\":true");
    assert_contains(json, "\"sourceCountsAligned\":true");
    assert_contains(json, "\"checkpointCountsAligned\":true");
    assert_contains(json, "\"validatorCountsAligned\":true");
    assert_contains(json, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto failing = validation::format_candidate_material_submission_precheck_validation_json(
        9, true, 25, 25, 20, 20, 41, 10, 10, 10, 10, 10, 10);
    assert_contains(failing, "\"candidateMaterialSubmissionPrecheckValidationPassed\":false");
    assert_contains(failing, "\"sourceIntegrityFrozen\":false");
    return 0;
}
