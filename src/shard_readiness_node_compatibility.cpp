#include "minikv/shard_readiness_node_compatibility.hpp"

namespace minikv::shard_readiness::node_compatibility {

std::string format_route_group_split_compatibility_json() {
    return "{\"compatibilityMode\":\"node-route-group-split-contract-stable\","
           "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\","
           "\"sourceNodePlan\":\"docs/plans3/v418-post-sandbox-endpoint-credential-resolver-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v158\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v158.json\","
           "\"nodeChangeType\":\"route-registration-refactor-only\","
           "\"nodeApiPathChanged\":false,"
           "\"nodeResponseShapeChanged\":false,"
           "\"nodeAddsEvidenceGate\":false,"
           "\"nodeStartsJavaService\":false,"
           "\"nodeStartsMiniKvService\":false,"
           "\"miniKvContractChangedForNodeRouteSplit\":false,"
           "\"miniKvFixturePathChanged\":false,"
           "\"miniKvShardJsonCommandChanged\":false,"
           "\"miniKvRequiresNodeRouteTableChange\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"readyForRuntimeExecutionPacket\":false,"
           "\"readyForRuntimeLiveReadGate\":false,"
           "\"canonicalApprovalInputsComplete\":false,"
           "\"templatesAcceptedAsCanonicalInputs\":false,"
           "\"executionAttempted\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"archiveCompatibilityPreserved\":true,"
           "\"failClosedOnMissingCanonicalInputs\":true}";
}

} // namespace minikv::shard_readiness::node_compatibility
