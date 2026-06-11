#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation;

    const auto passed =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            30, true, false, false, false, false, false, false, 8, 8, 3, 3);
    assert_contains(passed, "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true");
    assert_contains(passed, "\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeImplementationPlanConstantsImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeImplementationPlanReferencesImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeImplementationPlanChecksExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false");
    assert_contains(passed, "\"endpointHandleRead\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"credentialValueRead\":false");
    assert_contains(passed, "\"managedAuditConnectionOpened\":false");
    assert_contains(passed, "\"activeRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            29, true, false, false, false, false, false, false, 8, 8, 3, 3);
    assert_contains(failed_source,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_node =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            30, true, true, false, false, false, false, false, 8, 8, 3, 3);
    assert_contains(failed_node,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":false");
    assert_contains(failed_node, "\"nodePlanObservedOnly\":false");

    const auto failed_import =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            30, true, false, true, false, false, false, false, 8, 8, 3, 3);
    assert_contains(failed_import,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":false");
    assert_contains(failed_import, "\"miniKvBoundaryClosed\":false");

    const auto failed_endpoint =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            30, true, false, false, false, false, true, false, 8, 8, 3, 3);
    assert_contains(failed_endpoint,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":false");
    assert_contains(failed_endpoint, "\"miniKvBoundaryClosed\":false");

    const auto failed_count =
        validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
            30, true, false, false, false, false, false, false, 8, 7, 3, 3);
    assert_contains(failed_count,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
