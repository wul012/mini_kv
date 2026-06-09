#include "minikv/shard_preview_candidate_material_request_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_material_request_validation;

    const auto passed = validation::format_candidate_material_request_validation_json(25, true, 25, 25, 25, 25, 10, 10);
    assert_contains(passed, "\"candidateMaterialRequestValidationPassed\":true");
    assert_contains(passed, "\"sourceIntakePacketFrozen\":true");
    assert_contains(passed, "\"itemCountsAligned\":true");
    assert_contains(passed, "\"checkCountsAligned\":true");
    assert_contains(passed, "\"stageCountsAligned\":true");
    assert_contains(passed, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(passed, "\"reviewedMaterialPresent\":false");
    assert_contains(passed, "\"documentIntakeOpened\":false");
    assert_contains(passed, "\"documentRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed = validation::format_candidate_material_request_validation_json(24, true, 25, 25, 25, 25, 10, 10);
    assert_contains(failed, "\"candidateMaterialRequestValidationPassed\":false");
    assert_contains(failed, "\"sourceIntakePacketFrozen\":false");
}
