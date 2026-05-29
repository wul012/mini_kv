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
constexpr std::string_view release_version = "v149";
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
           "\"nodeConsumer\":\"Node v382+ may consume v149 after v148 frozen evidence handoff\","
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
           }) +
           ",\"changesArchivedNodeEvidence\":false,"
           "\"futureNodeConsumer\":\"Node v382 or later after v148 frozen evidence handoff\"}";
}

std::string format_historical_fallback_json() {
    return "{\"previousConsumedReleaseVersion\":\"v148\","
           "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v148.json\","
           "\"previousConsumptionNodeVersion\":\"Node v382 pending completed evidence intake\","
           "\"olderPrototypeFixturePath\":\"fixtures/release/shard-readiness-v144.json\","
           "\"rollingCurrentUsedForHistoricalBaseline\":false,"
           "\"nodeV381ArchiveVerificationPreserved\":true,"
           "\"nodeV382ReadsUnfinishedUpstream\":false}";
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
    return "{\"frozenReleaseVersion\":\"v148\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v148.json\","
           "\"frozenStatus\":\"active-prototype-plan-frozen-read-only\","
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
           "\"frozenReleaseVersion\":\"v148\","
           "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v148.json\","
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

std::string evidence_digest() {
    return runtime_evidence::digest(
        "mini-kv-shard-readiness-v149",
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
            {"historicalFallback=v148-frozen-no-rolling-current"},
            {"archivedNodeEvidence=v370-v381-preserved"},
            {"activePrototypePlan=prerequisite-only-no-activation"},
            {"activePrototypePlanFreeze=v148-frozen-no-router-no-write"},
            {"consumerHandoff=frozen-evidence-only-no-live-read"},
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
           ",\"status\":\"frozen-evidence-handoff-read-only\"" +
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
           }) +
           ",\"evidenceDigest\":" + json_string(evidence_digest()) +
           ",\"notes\":" + json_string_array({
               "read-only shard readiness hardening",
               "single logical shard only",
               "slot table is evidence, not active storage routing",
               "does not create shard directories or start extra processes",
               "freezes v148 activePrototypePlanFreeze evidence for handoff",
               "active shard prototype remains plan-prerequisite only",
               "does not mutate Node v370-v381 archived evidence",
               "not order or audit authoritative",
           }) +
           "}";
}

} // namespace minikv::shard_readiness
