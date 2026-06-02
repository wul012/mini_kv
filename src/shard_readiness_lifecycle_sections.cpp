#include "shard_readiness_lifecycle_sections.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::lifecycle_sections {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_active_prototype_plan_json() {
    return "{\"planMode\":\"prerequisite-only\","
           "\"activeShardPrototypeAllowed\":false,"
           "\"routerActivationAllowed\":false,"
           "\"shardDirectoryCreationAllowed\":false,"
           "\"multiProcessStartAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"requiredBeforeActivation\":" + json_string_array({
               "separate active shard prototype plan",
               "explicit service startup and cleanup responsibility",
               "read-only Java echo",
               "Node consumer gate after frozen mini-kv evidence",
               "fail-closed smoke target",
           }) +
           "}";
}

std::string format_active_prototype_plan_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesActivePrototypePlan\":true,"
           "\"frozenActiveShardPrototypeAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenShardDirectoryCreationAllowed\":false,"
           "\"frozenMultiProcessStartAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_consumer_handoff_json() {
    return "{\"handoffMode\":\"frozen-evidence-only\","
           "\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"readyForNodeConsumption\":true,"
           "\"liveReadGateRequiredBeforeRuntimeProbe\":true,"
           "\"startsServices\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredConsumerChecks\":" + json_string_array({
               "read frozen fixture instead of rolling current",
               "verify releaseVersion and status",
               "verify readOnly true and executionAllowed false",
               "verify no router and no write routing",
               "fail closed on missing frozen evidence",
           }) +
           "}";
}

std::string format_live_read_gate_plan_json() {
    return "{\"planMode\":\"service-lifecycle-prerequisite-only\","
           "\"liveReadGateAllowed\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"startsServices\":false,"
           "\"requiresServiceOwner\":true,"
           "\"requiresPortList\":true,"
           "\"requiresSmokeTarget\":true,"
           "\"requiresFailClosedBehavior\":true,"
           "\"requiresCleanup\":true,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredBeforeLiveRead\":" + json_string_array({
               "explicit live-read gate plan",
               "service owner and startup command",
               "port list and conflict behavior",
               "read-only smoke target",
               "fail-closed missing evidence behavior",
               "cleanup responsibility and stop command",
           }) +
           "}";
}

std::string format_live_read_gate_plan_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesLiveReadGatePlan\":true,"
           "\"frozenLiveReadGateAllowed\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenStartsServices\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_operator_service_lifecycle_template_json() {
    return "{\"evidenceMode\":\"template-only-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v153\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"operatorOwnedServiceLifecycleRequired\":true,"
           "\"serviceOwnerDeclared\":false,"
           "\"startupCommandDeclared\":false,"
           "\"portListDeclared\":false,"
           "\"getOnlySmokeTargetDeclared\":false,"
           "\"failClosedBehaviorRequired\":true,"
           "\"cleanupResponsibilityDeclared\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiredOperatorEvidence\":" + json_string_array({
               "service owner",
               "startup command",
               "port list",
               "GET-only smoke target",
               "fail-closed missing evidence behavior",
               "cleanup responsibility and stop command",
           }) +
           "}";
}

std::string format_operator_service_lifecycle_template_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesOperatorLifecycleTemplate\":true,"
           "\"frozenServiceOwnerDeclared\":false,"
           "\"frozenStartupCommandDeclared\":false,"
           "\"frozenPortListDeclared\":false,"
           "\"frozenGetOnlySmokeTargetDeclared\":false,"
           "\"frozenCleanupResponsibilityDeclared\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

std::string format_operator_service_lifecycle_evidence_json() {
    return "{\"evidenceMode\":\"declared-lifecycle-no-runtime\","
           "\"sourceFrozenReleaseVersion\":\"v153\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"operatorOwnedServiceLifecycleDeclared\":true,"
           "\"serviceOwnerDeclared\":true,"
           "\"serviceOwnerHandle\":\"mini-kv-local-operator\","
           "\"startupCommandDeclared\":true,"
           "\"startupCommand\":\"minikv_server.exe <operator-approved-loopback-port> 127.0.0.1\","
           "\"portListDeclared\":true,"
           "\"declaredPortHandles\":" + json_string_array({
               "operator-approved-loopback-port",
           }) +
           ",\"getOnlySmokeTargetDeclared\":true,"
           "\"getOnlySmokeTarget\":\"GET-only SHARDJSON evidence read\","
           "\"failClosedBehaviorDeclared\":true,"
           "\"failClosedBehavior\":\"missing evidence or port conflict blocks runtime gate\","
           "\"cleanupResponsibilityDeclared\":true,"
           "\"cleanupResponsibility\":\"operator stops service and verifies no LISTENING port\","
           "\"runtimeGateApproved\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"requiresSeparateRuntimeGate\":true,"
           "\"requiredBeforeRuntimeGate\":" + json_string_array({
               "operator approval record",
               "concrete loopback port assignment",
               "GET-only smoke command",
               "cleanup proof",
           }) +
           "}";
}

std::string format_operator_service_lifecycle_evidence_freeze_json() {
    return "{\"frozenReleaseVersion\":\"v153\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\","
           "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\","
           "\"preservesDeclaredLifecycleEvidence\":true,"
           "\"frozenOperatorOwnedServiceLifecycleDeclared\":true,"
           "\"frozenServiceOwnerDeclared\":true,"
           "\"frozenStartupCommandDeclared\":true,"
           "\"frozenPortListDeclared\":true,"
           "\"frozenGetOnlySmokeTargetDeclared\":true,"
           "\"frozenFailClosedBehaviorDeclared\":true,"
           "\"frozenCleanupResponsibilityDeclared\":true,"
           "\"frozenRuntimeGateApproved\":false,"
           "\"frozenStartsServices\":false,"
           "\"frozenRuntimeProbeAllowed\":false,"
           "\"frozenLiveReadAllowed\":false,"
           "\"frozenRouterActivationAllowed\":false,"
           "\"frozenWriteRoutingAllowed\":false,"
           "\"frozenExecutionAllowed\":false,"
           "\"rollingCurrentUsedForFrozenBaseline\":false}";
}

} // namespace minikv::shard_readiness::lifecycle_sections
