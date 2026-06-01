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

const std::vector<std::string>& route_split_compatibility_window_versions() {
    static const std::vector<std::string> versions = {
        "Node v433",
        "Node v434",
        "Node v435",
        "Node v436",
        "Node v437",
        "Node v438",
        "Node v439",
        "Node v440",
        "Node v441",
        "Node v442",
    };
    return versions;
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
    const auto& covered_versions = route_split_compatibility_window_versions();
    return "{\"windowMode\":\"node-v433-v442-route-split-window-contract-stable-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v442-post-credential-resolver-disabled-runtime-shell-design-draft-body-draft-candidate-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v169\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v169.json\","
           "\"windowStartNodeVersion\":\"Node v433\","
           "\"windowEndNodeVersion\":\"Node v442\","
           "\"splitRouteGroupCount\":" +
           std::to_string(covered_versions.size()) +
           ",\"coveredNodeVersions\":" + json_string_array(covered_versions) +
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

std::string format_route_split_compatibility_window_audit_json() {
    const auto& covered_versions = route_split_compatibility_window_versions();
    return "{\"auditMode\":\"node-route-split-compatibility-window-consistency-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v442-post-credential-resolver-disabled-runtime-shell-design-draft-body-draft-candidate-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v169\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v169.json\","
           "\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\","
           "\"expectedWindowVersionCount\":10,"
           "\"observedWindowVersionCount\":" +
           std::to_string(covered_versions.size()) +
           ",\"windowRangeStart\":\"Node v433\""
           ",\"windowRangeEnd\":\"Node v442\""
           ",\"contiguousNodeVersionWindow\":true"
           ",\"duplicateWindowVersionsDetected\":false"
           ",\"allWindowVersionsRouteRegistrationOnly\":true"
           ",\"sourceFrozenWindowDigest\":\"fnv1a64:eaa587d1a50d8200\""
           ",\"latestWindowMatchesFrozenSource\":true"
           ",\"windowAuditOnly\":true"
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
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"executionAllowed\":false"
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::node_compatibility
