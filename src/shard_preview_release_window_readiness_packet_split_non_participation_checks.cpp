#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ReleaseWindowReadinessPacketSplitNonParticipationCheck, 15> check_catalog = {{
    {1, "source-implementation-plan-closeout-freeze", "mini-kv v1475", "v1475 implementation-plan upstream echo closeout remains the frozen source"},
    {2, "node-release-window-packet-plan-intake", "Node v1935-v1937", "Node release-window readiness packet split is observed as Node-owned maintenance"},
    {3, "packet-types-no-import", "releaseWindowReadinessPacketTypes.ts", "profile step message forbidden-operation Java fixture and mini-kv evidence types stay outside C++"},
    {4, "packet-evidence-no-import", "releaseWindowReadinessPacketEvidence.ts", "frozen Java v61 and mini-kv v70 evidence stay as metadata only"},
    {5, "packet-policy-no-execution", "releaseWindowReadinessPacketPolicy.ts", "checks steps forbidden operations blockers warnings and recommendations are not executed by mini-kv"},
    {6, "stable-entrypoint-no-import", "releaseWindowReadinessPacket.ts", "public service entrypoint stability does not create a C++ import"},
    {7, "loader-renderer-no-execution", "Node packet loader and renderer", "loader renderer summary projection digesting and Markdown rendering remain Node-only"},
    {8, "frozen-java-v61-reference-only", "Java v61 rollback approval fixture", "Java historical evidence is referenced without service startup or ledger authority"},
    {9, "frozen-minikv-v70-reference-only", "mini-kv v70 restore drill evidence", "historical restore drill evidence is referenced without replaying restore load or compact"},
    {10, "release-dry-run-envelope-no-execution", "production release dry-run envelope", "later consumer remains outside mini-kv runtime execution"},
    {11, "status-routes-no-execution", "status routes", "route projection remains Node-owned and unexecuted by mini-kv"},
    {12, "endpoint-credential-boundary-closed", "endpoint and credential boundary", "endpoint handles raw endpoints credentials secrets and managed connections stay unread"},
    {13, "router-write-wal-execution-disabled", "storage execution boundary", "router write WAL load restore compact and execution remain disabled"},
    {14, "command-shardjson-fixture-parity", "mini-kv command and fixtures", "standalone command current SHARDJSON and versioned fixtures stay aligned"},
    {15, "clean-workspace-ci-closeout", "GitHub Actions closeout", "closeout keeps Node imports checks endpoints credentials routers writes WAL and execution disabled"},
}};

std::string format_check_json(const ReleaseWindowReadinessPacketSplitNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationOnly\":true"
           ",\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true"
           ",\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketChecksExecutedByMiniKv\":false"
           ",\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false"
           ",\"nodeStatusRoutesExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":false"
           ",\"miniKvExecutesNodeChecks\":false"
           ",\"miniKvStartsServices\":false"
           ",\"miniKvReadsEndpoints\":false"
           ",\"miniKvReadsCredentials\":false"
           ",\"miniKvReplaysRestoreDrillEvidence\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ReleaseWindowReadinessPacketSplitNonParticipationCheck>
release_window_readiness_packet_split_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_release_window_readiness_packet_split_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_release_window_readiness_packet_split_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("release window readiness packet split non-participation check count out of range");
    }

    std::string json = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            json += ",";
        }
        json += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_checks
