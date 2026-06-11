#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks =
        minikv::shard_preview_release_window_readiness_packet_split_non_participation_checks;

    const auto records = checks::release_window_readiness_packet_split_non_participation_checks();
    assert(checks::planned_release_window_readiness_packet_split_non_participation_check_count() == 15);
    assert(records.size() == 15);
    assert(records.front().check_code == std::string{"source-implementation-plan-closeout-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty = checks::format_release_window_readiness_packet_split_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_release_window_readiness_packet_split_non_participation_checks_json(15);
    assert_contains(json, "\"checkCode\":\"source-implementation-plan-closeout-freeze\"");
    assert_contains(json, "\"checkCode\":\"node-release-window-packet-plan-intake\"");
    assert_contains(json, "\"checkCode\":\"packet-types-no-import\"");
    assert_contains(json, "\"checkCode\":\"packet-evidence-no-import\"");
    assert_contains(json, "\"checkCode\":\"packet-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"stable-entrypoint-no-import\"");
    assert_contains(json, "\"checkCode\":\"loader-renderer-no-execution\"");
    assert_contains(json, "\"checkCode\":\"frozen-java-v61-reference-only\"");
    assert_contains(json, "\"checkCode\":\"frozen-minikv-v70-reference-only\"");
    assert_contains(json, "\"checkCode\":\"release-dry-run-envelope-no-execution\"");
    assert_contains(json, "\"checkCode\":\"status-routes-no-execution\"");
    assert_contains(json, "\"checkCode\":\"endpoint-credential-boundary-closed\"");
    assert_contains(json, "\"checkCode\":\"router-write-wal-execution-disabled\"");
    assert_contains(json, "\"checkCode\":\"command-shardjson-fixture-parity\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationOnly\":true");
    assert_contains(json, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeStatusRoutesExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvReplaysRestoreDrillEvidence\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_release_window_readiness_packet_split_non_participation_checks_json(16);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
