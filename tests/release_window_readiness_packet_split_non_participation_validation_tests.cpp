#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_release_window_readiness_packet_split_non_participation_validation;

    const auto passed = validation::format_release_window_readiness_packet_split_non_participation_validation_json(
        3, true, false, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(passed, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
    assert_contains(passed, "\"endpointHandleRead\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"credentialValueRead\":false");
    assert_contains(passed, "\"managedAuditConnectionOpened\":false");
    assert_contains(passed, "\"activeRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            2, true, false, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(failed_source, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_fresh =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            3, true, true, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(failed_fresh, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_fresh, "\"nodePlanObservedOnly\":false");

    const auto failed_import =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            3, true, false, true, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(failed_import, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_import, "\"miniKvBoundaryClosed\":false");

    const auto failed_endpoint =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            3, true, false, false, false, false, true, false, false, 15, 15, 15, 15);
    assert_contains(failed_endpoint, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_endpoint, "\"miniKvBoundaryClosed\":false");

    const auto failed_packet_execution =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            3, true, false, false, false, false, false, false, true, 15, 15, 15, 15);
    assert_contains(failed_packet_execution,
                    "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_packet_execution, "\"miniKvBoundaryClosed\":false");

    const auto failed_count =
        validation::format_release_window_readiness_packet_split_non_participation_validation_json(
            3, true, false, false, false, false, false, false, false, 15, 14, 15, 15);
    assert_contains(failed_count, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
