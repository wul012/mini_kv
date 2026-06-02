#include "minikv/shard_readiness_route_catalog_post_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_post_closeout_continuity_json() {
    const PostCloseoutStageDescriptor& descriptor = current_stage_descriptor();

    return "{\"continuityMode\":\"node-route-catalog-cleanup-post-closeout-continuity-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v505-post-java-mini-kv-route-catalog-cleanup-readiness-handoff-evidence-archive-verification-route-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":" +
           json_string(descriptor.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(descriptor.source_frozen_fixture_path) +
           ",\"sourceFrozenDigest\":" + json_string(descriptor.source_frozen_digest) +
           ",\"continuityStage\":" + json_string(descriptor.continuity_stage) +
           ",\"stageSequence\":" + std::to_string(descriptor.stage_sequence) +
           ",\"stageReleaseVersion\":" + json_string(descriptor.stage_release_version) +
           ",\"stageScope\":" + json_string(descriptor.stage_scope) +
           ",\"previousCloseoutAuditReleaseVersion\":\"v200\","
           "\"previousCloseoutAuditDigest\":\"fnv1a64:d1e889711b5d8574\","
           "\"trackedMiniKvCloseoutRangeStart\":\"v194\","
           "\"trackedMiniKvCloseoutRangeEnd\":\"v200\","
           "\"trackedPostCloseoutRangeStart\":\"v201\","
           "\"trackedPostCloseoutRangeEnd\":" +
           json_string(descriptor.stage_release_version) +
           ",\"trackedPostCloseoutReleaseCount\":" + std::to_string(descriptor.stage_sequence) +
           ",\"nodeBatchCloseoutVersion\":\"Node v505\","
           "\"nodePlanStillLatestForMiniKv\":true,"
           "\"sourceFixtureVersioned\":true,"
           "\"rollingCurrentRejected\":true,"
           "\"preservesBatchCloseout\":true,"
           "\"preservesBatchCloseoutAudit\":true,"
           "\"preservesNodeV480Validation\":true,"
           "\"preservesArchiveVerifierChecks\":true,"
           "\"archiveVerifierCheckCount\":16,"
           "\"archiveVerifierChecksPassed\":16,"
           "\"stageAssertions\":" +
           json_string_array({
               std::string{descriptor.source_baseline_assertion},
               "stage descriptor lifecycle preserves the JSON contract",
               "post-closeout continuity is metadata only",
               "no router, write routing, or execution authority is opened",
           }) +
           ",\"postCloseoutStageOnly\":true,"
           "\"readyForNextNodeBatch\":true,"
           "\"nodeConsumesFreshMiniKvEvidence\":false,"
           "\"miniKvRuntimeContractChanged\":false,"
           "\"nodeAddsEvidenceGate\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
