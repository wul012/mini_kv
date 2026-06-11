#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_validation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

std::string format_validation(int source_stage_count = 20,
                              bool source_complete = true,
                              bool fresh_minikv = false,
                              bool fresh_java = false,
                              bool reads_walkthrough = false,
                              bool scans_node = false,
                              bool executes_node_route = false,
                              bool starts_node = false,
                              bool starts_java = false,
                              bool starts_minikv = false,
                              bool mutates_node_docs = false,
                              bool mutates_java_docs = false,
                              bool placeholder_allowed = false,
                              bool bulk_relocation_allowed = false,
                              int planned_checks = 12,
                              int completed_checks = 12,
                              int published_stages = 6,
                              int planned_stages = 6) {
    namespace validation = minikv::shard_preview_code_walkthrough_quality_gate_non_participation_validation;
    return validation::format_code_walkthrough_quality_gate_non_participation_validation_json(
        source_stage_count,
        source_complete,
        fresh_minikv,
        fresh_java,
        reads_walkthrough,
        scans_node,
        executes_node_route,
        starts_node,
        starts_java,
        starts_minikv,
        mutates_node_docs,
        mutates_java_docs,
        placeholder_allowed,
        bulk_relocation_allowed,
        planned_checks,
        completed_checks,
        published_stages,
        planned_stages);
}

void assert_validation_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json,
        "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = format_validation();
    assert_contains(valid, "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":true");
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"nodePlanObservedOnly\":true");
    assert_contains(valid, "\"miniKvBoundaryClosed\":true");
    assert_contains(valid, "\"qualityFloorClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");
    assert_contains(valid, "\"walkthroughQualityGateOnly\":true");
    assert_contains(valid, "\"nodeWalkthroughFilesReadByMiniKv\":false");
    assert_contains(valid, "\"miniKvScansNodeRepository\":false");
    assert_contains(valid, "\"miniKvExecutesNodeQualityRoute\":false");
    assert_contains(valid, "\"placeholderWalkthroughAllowed\":false");
    assert_contains(valid, "\"bulkHistoricalRelocationAllowed\":false");
    assert_contains(valid, "\"writeRoutingAllowed\":false");
    assert_contains(valid, "\"executionAllowed\":false");

    assert_validation_fails(format_validation(19));
    assert_validation_fails(format_validation(20, false));
    assert_validation_fails(format_validation(20, true, true));
    assert_validation_fails(format_validation(20, true, false, true));
    assert_validation_fails(format_validation(20, true, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, false, 12, 11));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, false, 12, 12, 5));

    return 0;
}
