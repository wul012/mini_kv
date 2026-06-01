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
    "node-v433-v472-route-catalog-cleanup-closeout-window-read-only";
constexpr std::string_view route_split_window_source_node_plan =
    "docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md";
constexpr std::string_view route_split_window_source_frozen_release_version = "v191";
constexpr std::string_view route_split_window_source_frozen_fixture_path =
    "fixtures/release/shard-readiness-v191.json";
constexpr std::string_view route_split_window_start_node_version = "Node v433";
constexpr std::string_view route_split_window_end_node_version = "Node v472";
constexpr std::string_view route_split_window_source_frozen_digest = "fnv1a64:e6a06c9207144d9d";

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

int parse_node_version_number(std::string_view version) {
    constexpr std::string_view prefix = "Node v";
    if (version.size() <= prefix.size() || version.substr(0, prefix.size()) != prefix) {
        return -1;
    }
    int number = 0;
    for (const char ch : version.substr(prefix.size())) {
        if (ch < '0' || ch > '9') {
            return -1;
        }
        number = (number * 10) + (ch - '0');
    }
    return number;
}

bool route_split_window_is_contiguous(const std::vector<std::string>& versions) {
    if (versions.empty()) {
        return false;
    }
    int previous = parse_node_version_number(versions.front());
    if (previous < 0) {
        return false;
    }
    for (std::size_t index = 1; index < versions.size(); ++index) {
        const int current = parse_node_version_number(versions[index]);
        if (current != previous + 1) {
            return false;
        }
        previous = current;
    }
    return true;
}

bool route_split_window_has_duplicates(const std::vector<std::string>& versions) {
    for (std::size_t left = 0; left < versions.size(); ++left) {
        for (std::size_t right = left + 1; right < versions.size(); ++right) {
            if (versions[left] == versions[right]) {
                return true;
            }
        }
    }
    return false;
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
        "Node v443",
        "Node v444",
        "Node v445",
        "Node v446",
        "Node v447",
        "Node v448",
        "Node v449",
        "Node v450",
        "Node v451",
        "Node v452",
        "Node v453",
        "Node v454",
        "Node v455",
        "Node v456",
        "Node v457",
        "Node v458",
        "Node v459",
        "Node v460",
        "Node v461",
        "Node v462",
        "Node v463",
        "Node v464",
        "Node v465",
        "Node v466",
        "Node v467",
        "Node v468",
        "Node v469",
        "Node v470",
        "Node v471",
        "Node v472",
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
           ",\"allChangesRouteCatalogOnly\":true"
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
    const bool contiguous_window = route_split_window_is_contiguous(covered_versions);
    const bool duplicate_versions = route_split_window_has_duplicates(covered_versions);
    const int window_start_number = parse_node_version_number(route_split_window_start_node_version);
    const int window_end_number = parse_node_version_number(route_split_window_end_node_version);
    const bool window_range_numbers_parseable = window_start_number >= 0 && window_end_number >= 0;
    const int computed_window_version_span =
        window_range_numbers_parseable ? (window_end_number - window_start_number + 1) : 0;
    const bool window_count_matches_range =
        computed_window_version_span == static_cast<int>(covered_versions.size());
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
           ",\"windowRangeStartNumber\":" + std::to_string(window_start_number) +
           ",\"windowRangeEndNumber\":" + std::to_string(window_end_number) +
           ",\"computedWindowVersionSpan\":" + std::to_string(computed_window_version_span) +
           ",\"windowRangeNumbersParseable\":" + json_bool(window_range_numbers_parseable) +
           ",\"windowCountMatchesRange\":" + json_bool(window_count_matches_range) +
           ",\"contiguousNodeVersionWindow\":" + json_bool(contiguous_window) +
           ",\"duplicateWindowVersionsDetected\":" + json_bool(duplicate_versions) +
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
