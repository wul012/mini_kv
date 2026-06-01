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

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::string_view route_split_window_mode =
    "node-v433-v447-route-split-window-contract-stable-read-only";
constexpr std::string_view route_split_window_source_node_plan =
    "docs/plans3/v447-post-managed-audit-dry-run-adapter-route-group-split-roadmap.md";
constexpr std::string_view route_split_window_source_frozen_release_version = "v172";
constexpr std::string_view route_split_window_source_frozen_fixture_path =
    "fixtures/release/shard-readiness-v172.json";
constexpr std::string_view route_split_window_start_node_version = "Node v433";
constexpr std::string_view route_split_window_end_node_version = "Node v447";
constexpr std::string_view route_split_window_source_frozen_digest = "fnv1a64:85f0acb5a011256f";

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
        "Node v443",
        "Node v444",
        "Node v445",
        "Node v446",
        "Node v447",
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
    return std::string{"{\"windowMode\":"} + json_string(route_split_window_mode) +
           ",\"sourceNodePlan\":" + json_string(route_split_window_source_node_plan) +
           ",\"sourceFrozenReleaseVersion\":" +
           json_string(route_split_window_source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(route_split_window_source_frozen_fixture_path) +
           ",\"windowStartNodeVersion\":" + json_string(route_split_window_start_node_version) +
           ",\"windowEndNodeVersion\":" + json_string(route_split_window_end_node_version) +
           ",\"splitRouteGroupCount\":" +
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
    return std::string{"{\"auditMode\":\"node-route-split-compatibility-window-consistency-read-only\","} +
           "\"sourceNodePlan\":" +
           json_string(route_split_window_source_node_plan) +
           ",\"sourceFrozenReleaseVersion\":" +
           json_string(route_split_window_source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(route_split_window_source_frozen_fixture_path) +
           ",\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\","
           "\"expectedWindowVersionCount\":" +
           std::to_string(covered_versions.size()) +
           ","
           "\"observedWindowVersionCount\":" +
           std::to_string(covered_versions.size()) +
           ",\"windowRangeStart\":" +
           json_string(route_split_window_start_node_version) +
           ",\"windowRangeEnd\":" + json_string(route_split_window_end_node_version) +
           ",\"contiguousNodeVersionWindow\":true"
           ",\"duplicateWindowVersionsDetected\":false"
           ",\"allWindowVersionsRouteRegistrationOnly\":true"
           ",\"sourceFrozenWindowDigest\":" + json_string(route_split_window_source_frozen_digest) +
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
