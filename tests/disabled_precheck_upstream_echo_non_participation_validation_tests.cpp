#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_validation;

    const auto passed = validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
        12, true, false, false, false, false, false, false, 25, 25, 25, 25);
    assert_contains(passed, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvImportsNodeModules\":false");
    assert_contains(passed, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(passed, "\"miniKvReadsEndpoints\":false");
    assert_contains(passed, "\"miniKvReadsCredentials\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source = validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
        11, true, false, false, false, false, false, false, 25, 25, 25, 25);
    assert_contains(failed_source, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_node_plan =
        validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
            12, true, true, false, false, false, false, false, 25, 25, 25, 25);
    assert_contains(failed_node_plan, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_node_plan, "\"nodePlanObservedOnly\":false");

    const auto failed_boundary =
        validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
            12, true, false, true, false, false, false, false, 25, 25, 25, 25);
    assert_contains(failed_boundary, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_boundary, "\"miniKvBoundaryClosed\":false");

    const auto failed_count = validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
        12, true, false, false, false, false, false, false, 25, 24, 25, 25);
    assert_contains(failed_count, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
