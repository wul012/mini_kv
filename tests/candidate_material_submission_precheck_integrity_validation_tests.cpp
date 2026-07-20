#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_material_submission_precheck_integrity_validation;

    const auto json = validation::format_candidate_material_submission_precheck_integrity_validation_json(
        10, true, 10, 10, 25, 25, 20, 20, 41, 25, 25, 25, 25);
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityValidationPassed\":true");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckComplete\":true");
    assert_contains(json, "\"sourceMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"sourceMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"sourceMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceAcceptanceCheckCount\":25");
    assert_contains(json, "\"sourceRequiredMaterialFieldCount\":20");
    assert_contains(json, "\"sourceSubmissionMaterialFieldCount\":20");
    assert_contains(json, "\"sourceMaterialSubmissionGateCount\":41");
    assert_contains(json, "\"sourceMaterialSubmissionPrecheckFrozen\":true");
    assert_contains(json, "\"integrityCountsAligned\":true");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto failing = validation::format_candidate_material_submission_precheck_integrity_validation_json(
        10, true, 10, 9, 25, 25, 20, 20, 41, 25, 25, 25, 25);
    assert_contains(failing, "\"candidateMaterialSubmissionPrecheckIntegrityValidationPassed\":false");
    assert_contains(failing, "\"sourceMaterialSubmissionPrecheckFrozen\":false");
    return 0;
}
