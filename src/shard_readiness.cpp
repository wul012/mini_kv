#include "minikv/shard_readiness.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness {
namespace {

constexpr std::string_view contract_version = "shard-readiness.v1";
constexpr std::string_view release_version = "v152";
constexpr int slot_count = 16;

struct RouteSample {
    std::string_view key;
    int slot;
};

constexpr std::array<RouteSample, 3> route_samples = {{
    {"orderops:alpha", 5},
    {"audit:receipt:001", 9},
    {"mini-kv:health", 14},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string format_route_samples_json() {
    std::string response = "[";
    bool first = true;
    for (const auto& sample : route_samples) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"key\":" + json_string(sample.key) +
                    ",\"slot\":" + std::to_string(sample.slot) +
                    ",\"shardId\":\"shard-0\"}";
    }
    response += "]";
    return response;
}

std::string format_shard_map_json() {
    return "[{\"shardId\":\"shard-0\",\"slotStart\":0,\"slotEnd\":15,"
           "\"storagePath\":\"not-created\",\"readOnly\":true,\"writesAllowed\":false}]";
}

std::string format_boundaries_json() {
    return "{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false,"
           "\"activeRouterInstalled\":false,\"archivedNodeEvidenceMutated\":false}";
}

std::string format_diagnostics_json() {
    return "{\"writeCommandsExecuted\":false,\"adminCommandsExecuted\":false,"
           "\"loadRestoreCompactExecuted\":false,"
           "\"nodeConsumer\":\"Node v388+ may consume v152 after declared operator lifecycle evidence\","
           "\"javaEchoExpected\":\"Java shard-readiness echo may consume the same shard-readiness.v1 fields\","
           "\"nodeArchivedEvidencePreserved\":true}";
}

std::string format_command_catalog_json() {
    return "{\"command\":\"SHARDJSON\",\"category\":\"read\",\"mutatesStore\":false,"
           "\"touchesWal\":false,\"stable\":true,\"extraArgsAllowed\":false,"
           "\"sideEffects\":[\"metadata_read\"]}";
}

std::string format_fixture_parity_json() {
    return "{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\","
           "\"historicalFixturePaths\":" +
           json_string_array({
               "fixtures/release/shard-readiness-v144.json",
               "fixtures/release/shard-readiness-v145.json",
               "fixtures/release/shard-readiness-v146.json",
               "fixtures/release/shard-readiness-v147.json",
               "fixtures/release/shard-readiness-v148.json",
               "fixtures/release/shard-readiness-v149.json",
               "fixtures/release/shard-readiness-v150.json",
               "fixtures/release/shard-readiness-v151.json",
           }) +
           ",\"runtimeMatchesCurrentFixture\":true,\"historicalFixturesPreserved\":true}";
}

std::string format_archive_compatibility_json() {
    return "{\"preservesNodeArchivedEvidence\":true,"
           "\"archivedNodeVersions\":" +
           json_string_array({
               "Node v370",
               "Node v371",
               "Node v372",
               "Node v373",
               "Node v374",
               "Node v375",
               "Node v376",
               "Node v377",
               "Node v378",
               "Node v379",
               "Node v380",
               "Node v381",
               "Node v382",
               "Node v383",
               "Node v384",
               "Node v385",
               "Node v386",
               "Node v387",
           }) +
           ",\"changesArchivedNodeEvidence\":false,"
           "\"futureNodeConsumer\":\"Node v388 or later after declared operator lifecycle evidence\"}";
}

std::string format_historical_fallback_json() {
    return "{\"previousConsumedReleaseVersion\":\"v151\","
           "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
           "\"previousConsumptionNodeVersion\":\"Node v388 pending separate runtime gate approval\","
           "\"olderPrototypeFixturePath\":\"fixtures/release/shard-readiness-v144.json\","
           "\"rollingCurrentUsedForHistoricalBaseline\":false,"
           "\"nodeV387ArchiveVerificationPreserved\":true,"
           "\"nodeV388ReadsUnfinishedUpstream\":false}";
}

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
    return "{\"frozenReleaseVersion\":\"v151\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
           "\"frozenStatus\":\"operator-service-lifecycle-template-read-only\","
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
           "\"frozenReleaseVersion\":\"v151\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
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
    return "{\"frozenReleaseVersion\":\"v151\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
           "\"frozenStatus\":\"operator-service-lifecycle-template-read-only\","
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
           "\"sourceFrozenReleaseVersion\":\"v151\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
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
    return "{\"frozenReleaseVersion\":\"v151\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
           "\"frozenStatus\":\"operator-service-lifecycle-template-read-only\","
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
           "\"sourceFrozenReleaseVersion\":\"v151\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v151.json\","
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

std::string evidence_digest() {
    return runtime_evidence::digest(
        "mini-kv-shard-readiness-v152",
        {
            {std::string{contract_version}},
            {std::string{version}},
            {std::string{release_version}},
            {"shardEnabled=false"},
            {"shardCount=1"},
            {"slotCount=16"},
            {"routingMode=single-shard-readiness-prototype"},
            {fixture_path()},
            {"commandCatalog=read-no-mutate-no-wal"},
            {"fixtureParity=runtime-matches-current-fixture"},
            {"historicalFallback=v151-frozen-no-rolling-current"},
            {"archivedNodeEvidence=v370-v387-preserved"},
            {"activePrototypePlan=prerequisite-only-no-activation"},
            {"activePrototypePlanFreeze=v151-frozen-no-router-no-write"},
            {"consumerHandoff=frozen-evidence-only-no-live-read"},
            {"liveReadGatePlan=prerequisite-only-no-start-no-execution"},
            {"liveReadGatePlanFreeze=v151-frozen-no-runtime-probe"},
            {"operatorServiceLifecycleTemplate=template-only-no-runtime"},
            {"operatorServiceLifecycleTemplateFreeze=v151-frozen-template-only"},
            {"operatorServiceLifecycleEvidence=declared-no-runtime-gate"},
        });
}

} // namespace

std::string fixture_path() {
    return "fixtures/release/shard-readiness.json";
}

std::string format_json() {
    return "{\"contract\":" + json_string(contract_version) +
           ",\"evidenceType\":\"shard_readiness\"" +
           ",\"project\":\"mini-kv\"" +
           ",\"version\":" + json_string(version) +
           ",\"releaseVersion\":" + json_string(release_version) +
           ",\"readOnly\":true" +
           ",\"executionAllowed\":false" +
           ",\"shardEnabled\":false" +
           ",\"shardCount\":1" +
           ",\"slotCount\":" + std::to_string(slot_count) +
           ",\"routingMode\":\"single-shard-readiness-prototype\"" +
           ",\"evidencePath\":" + json_string(fixture_path()) +
           ",\"status\":\"declared-operator-lifecycle-no-runtime-read-only\"" +
           ",\"shardMap\":" + format_shard_map_json() +
           ",\"keyRoutingSamples\":" + format_route_samples_json() +
           ",\"boundaries\":" + format_boundaries_json() +
           ",\"diagnostics\":" + format_diagnostics_json() +
           ",\"commandCatalog\":" + format_command_catalog_json() +
           ",\"fixtureParity\":" + format_fixture_parity_json() +
           ",\"archiveCompatibility\":" + format_archive_compatibility_json() +
           ",\"historicalFallback\":" + format_historical_fallback_json() +
           ",\"activePrototypePlan\":" + format_active_prototype_plan_json() +
           ",\"activePrototypePlanFreeze\":" + format_active_prototype_plan_freeze_json() +
           ",\"consumerHandoff\":" + format_consumer_handoff_json() +
           ",\"liveReadGatePlan\":" + format_live_read_gate_plan_json() +
           ",\"liveReadGatePlanFreeze\":" + format_live_read_gate_plan_freeze_json() +
           ",\"operatorServiceLifecycleTemplate\":" + format_operator_service_lifecycle_template_json() +
           ",\"operatorServiceLifecycleTemplateFreeze\":" + format_operator_service_lifecycle_template_freeze_json() +
           ",\"operatorServiceLifecycleEvidence\":" + format_operator_service_lifecycle_evidence_json() +
           ",\"readOnlyBoundaryFields\":" + json_string_array({
               "readOnly",
               "executionAllowed",
               "boundaries.writeCommandsAllowed",
               "boundaries.adminCommandsAllowed",
               "boundaries.loadRestoreCompactAllowed",
               "boundaries.archivedNodeEvidenceMutated",
               "archiveCompatibility.changesArchivedNodeEvidence",
               "historicalFallback.rollingCurrentUsedForHistoricalBaseline",
               "activePrototypePlan.activeShardPrototypeAllowed",
               "activePrototypePlan.routerActivationAllowed",
               "activePrototypePlan.shardDirectoryCreationAllowed",
               "activePrototypePlan.writeRoutingAllowed",
               "activePrototypePlanFreeze.rollingCurrentUsedForFrozenBaseline",
               "activePrototypePlanFreeze.frozenRouterActivationAllowed",
               "activePrototypePlanFreeze.frozenWriteRoutingAllowed",
               "consumerHandoff.startsServices",
               "consumerHandoff.routerActivationAllowed",
               "consumerHandoff.writeRoutingAllowed",
               "consumerHandoff.executionAllowed",
               "liveReadGatePlan.liveReadGateAllowed",
               "liveReadGatePlan.runtimeProbeAllowed",
               "liveReadGatePlan.startsServices",
               "liveReadGatePlan.routerActivationAllowed",
               "liveReadGatePlan.writeRoutingAllowed",
               "liveReadGatePlan.executionAllowed",
               "liveReadGatePlanFreeze.rollingCurrentUsedForFrozenBaseline",
               "liveReadGatePlanFreeze.frozenRuntimeProbeAllowed",
               "liveReadGatePlanFreeze.frozenStartsServices",
               "liveReadGatePlanFreeze.frozenExecutionAllowed",
               "operatorServiceLifecycleTemplate.serviceOwnerDeclared",
               "operatorServiceLifecycleTemplate.startupCommandDeclared",
               "operatorServiceLifecycleTemplate.portListDeclared",
               "operatorServiceLifecycleTemplate.getOnlySmokeTargetDeclared",
               "operatorServiceLifecycleTemplate.startsServices",
               "operatorServiceLifecycleTemplate.runtimeProbeAllowed",
               "operatorServiceLifecycleTemplate.liveReadAllowed",
               "operatorServiceLifecycleTemplate.executionAllowed",
               "operatorServiceLifecycleTemplateFreeze.rollingCurrentUsedForFrozenBaseline",
               "operatorServiceLifecycleTemplateFreeze.frozenRuntimeProbeAllowed",
               "operatorServiceLifecycleTemplateFreeze.frozenExecutionAllowed",
               "operatorServiceLifecycleEvidence.runtimeGateApproved",
               "operatorServiceLifecycleEvidence.startsServices",
               "operatorServiceLifecycleEvidence.runtimeProbeAllowed",
               "operatorServiceLifecycleEvidence.liveReadAllowed",
               "operatorServiceLifecycleEvidence.routerActivationAllowed",
               "operatorServiceLifecycleEvidence.writeRoutingAllowed",
               "operatorServiceLifecycleEvidence.executionAllowed",
           }) +
           ",\"evidenceDigest\":" + json_string(evidence_digest()) +
           ",\"notes\":" + json_string_array({
               "read-only shard readiness hardening",
               "single logical shard only",
               "slot table is evidence, not active storage routing",
               "does not create shard directories or start extra processes",
               "freezes v151 operator lifecycle template evidence",
               "declares operator lifecycle evidence without runtime gate approval",
               "live-read gate remains prerequisite-only and does not start services",
               "operator service lifecycle evidence still has no runtime probe",
               "active shard prototype remains plan-prerequisite only",
               "does not mutate Node v370-v387 archived evidence",
               "not order or audit authoritative",
           }) +
           "}";
}

} // namespace minikv::shard_readiness
