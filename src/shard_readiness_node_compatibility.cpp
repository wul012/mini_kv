#include "minikv/shard_readiness_node_compatibility.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness::node_compatibility {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

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

std::string format_route_split_compatibility_window_json() {
    return "{\"windowMode\":\"node-v433-v440-route-split-window-contract-stable-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v440-post-credential-resolver-disabled-runtime-shell-design-draft-body-pre-draft-decision-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v166\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v166.json\","
           "\"windowStartNodeVersion\":\"Node v433\","
           "\"windowEndNodeVersion\":\"Node v440\","
           "\"splitRouteGroupCount\":8,"
           "\"coveredNodeVersions\":" +
           json_string_array({
               "Node v433",
               "Node v434",
               "Node v435",
               "Node v436",
               "Node v437",
               "Node v438",
               "Node v439",
               "Node v440",
           }) +
           ",\"allChangesRouteRegistrationOnly\":true"
           ",\"nodeApiPathChanged\":false"
           ",\"nodeResponseShapeChanged\":false"
           ",\"nodeAddsEvidenceGate\":false"
           ",\"nodeStartsJavaService\":false"
           ",\"nodeStartsMiniKvService\":false"
           ",\"miniKvContractChangedForNodeRouteSplit\":false"
           ",\"miniKvFixturePathChanged\":false"
           ",\"miniKvShardJsonCommandChanged\":false"
           ",\"miniKvRequiresNodeRouteTableChange\":false"
           ",\"runtimeGateApprovalPresent\":false"
           ",\"runtimeExecutionPacketExecutable\":false"
           ",\"startsJavaService\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"activeShardPrototypeEnabled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false"
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::node_compatibility
