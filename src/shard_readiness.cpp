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
constexpr std::string_view release_version = "v144";
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

std::string evidence_digest() {
    return runtime_evidence::digest(
        "mini-kv-shard-readiness-v144",
        {
            {std::string{contract_version}},
            {std::string{version}},
            {std::string{release_version}},
            {"shardEnabled=false"},
            {"shardCount=1"},
            {"slotCount=16"},
            {"routingMode=single-shard-readiness-prototype"},
            {fixture_path()},
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
           ",\"status\":\"prototype-ready-read-only\"" +
           ",\"shardMap\":[{\"shardId\":\"shard-0\",\"slotStart\":0,\"slotEnd\":15,"
           "\"storagePath\":\"not-created\",\"readOnly\":true,\"writesAllowed\":false}]" +
           ",\"keyRoutingSamples\":" + format_route_samples_json() +
           ",\"boundaries\":{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false}" +
           ",\"diagnostics\":{\"writeCommandsExecuted\":false,\"adminCommandsExecuted\":false,"
           "\"loadRestoreCompactExecuted\":false,\"nodeConsumer\":\"Node v370 shard readiness contract consumer gate\","
           "\"javaEchoExpected\":\"Java shard-readiness echo may consume the same shard-readiness.v1 fields\"}" +
           ",\"evidenceDigest\":" + json_string(evidence_digest()) +
           ",\"notes\":" + json_string_array({
               "read-only shard readiness prototype",
               "single logical shard only",
               "slot table is evidence, not active storage routing",
               "does not create shard directories or start extra processes",
               "not order or audit authoritative",
           }) +
           "}";
}

} // namespace minikv::shard_readiness
