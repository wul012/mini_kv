#include "minikv/shard_preview_candidate_material_request_integrity_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_material_request_integrity_validation;

    const auto passed = validation::format_candidate_material_request_integrity_validation_json(
        10, true, 25, 25, 40, 20, 15, 15, 10, 10);
    assert_contains(passed, "\"candidateMaterialRequestIntegrityValidationPassed\":true");
    assert_contains(passed, "\"sourceMaterialRequestFrozen\":true");
    assert_contains(passed, "\"integrityCountsAligned\":true");
    assert_contains(passed, "\"nodeStopConditionRespected\":true");
    assert_contains(passed, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(passed, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(passed, "\"reviewedMaterialPresent\":false");
    assert_contains(passed, "\"documentIntakeOpened\":false");
    assert_contains(passed, "\"documentRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed = validation::format_candidate_material_request_integrity_validation_json(
        9, true, 25, 25, 40, 20, 15, 15, 10, 10);
    assert_contains(failed, "\"candidateMaterialRequestIntegrityValidationPassed\":false");
    assert_contains(failed, "\"sourceMaterialRequestFrozen\":false");
}
