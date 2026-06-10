#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_type_barrel_split_follow_up_non_participation_validation;

    const auto passed = validation::format_type_barrel_split_follow_up_non_participation_validation_json(
        25, true, false, false, false, false, false, 20, 20, 20, 20);
    assert_contains(passed, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"followUpClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvTypeBarrelFollowUpStarted\":false");
    assert_contains(passed, "\"miniKvImportsNodeTypes\":false");
    assert_contains(passed, "\"profileBoundaryEndpointRead\":false");
    assert_contains(passed, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source = validation::format_type_barrel_split_follow_up_non_participation_validation_json(
        24, true, false, false, false, false, false, 20, 20, 20, 20);
    assert_contains(failed_source, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_closure = validation::format_type_barrel_split_follow_up_non_participation_validation_json(
        25, true, false, false, true, false, false, 20, 20, 20, 20);
    assert_contains(failed_closure, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":false");
    assert_contains(failed_closure, "\"followUpClosed\":false");

    const auto failed_count = validation::format_type_barrel_split_follow_up_non_participation_validation_json(
        25, true, false, false, false, false, false, 20, 19, 20, 20);
    assert_contains(failed_count, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
