#include "minikv/shard_route_preview.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview {
namespace {

constexpr int preview_slot_count = 16;
constexpr std::string_view preview_shard_id = "shard-0";
constexpr std::string_view preview_route_mode = "preview-only";
constexpr int current_rollout_stage_count = 5;

struct PinnedSample {
    std::string_view key;
    int slot;
};

struct RolloutStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<PinnedSample, 3> pinned_samples = {{
    {"orderops:alpha", 5},
    {"audit:receipt:001", 9},
    {"mini-kv:health", 14},
}};

constexpr std::array<RolloutStage, 20> rollout_stages = {{
    {1,
     "v279",
     "route-preview-command-contract",
     "v278",
     "fixtures/release/shard-readiness-v278.json",
     "adds read-only SHARDROUTE and SHARDROUTEJSON preview commands"},
    {2,
     "v280",
     "route-preview-command-catalog-alignment",
     "v279",
     "fixtures/release/shard-readiness-v279.json",
     "aligns route preview commands with command catalog and help surfaces"},
    {3,
     "v281",
     "route-preview-command-explain-contract",
     "v280",
     "fixtures/release/shard-readiness-v280.json",
     "freezes EXPLAINJSON route preview semantics as metadata read only"},
    {4,
     "v282",
     "route-preview-checkjson-contract",
     "v281",
     "fixtures/release/shard-readiness-v281.json",
     "freezes CHECKJSON route preview no-execution contract"},
    {5,
     "v283",
     "route-preview-cli-smoke-contract",
     "v282",
     "fixtures/release/shard-readiness-v282.json",
     "records CLI smoke expectations for read-only route preview"},
    {6,
     "v284",
     "route-preview-tcp-smoke-contract",
     "v283",
     "fixtures/release/shard-readiness-v283.json",
     "records TCP smoke expectations for read-only route preview"},
    {7,
     "v285",
     "route-preview-fixture-freeze",
     "v284",
     "fixtures/release/shard-readiness-v284.json",
     "freezes route preview rollout fixture baseline"},
    {8,
     "v286",
     "route-preview-sample-parity-audit",
     "v285",
     "fixtures/release/shard-readiness-v285.json",
     "audits pinned sample slots against existing shard readiness samples"},
    {9,
     "v287",
     "route-preview-rollout-catalog",
     "v286",
     "fixtures/release/shard-readiness-v286.json",
     "catalogs route preview rollout stages"},
    {10,
     "v288",
     "route-preview-rollout-catalog-audit",
     "v287",
     "fixtures/release/shard-readiness-v287.json",
     "audits route preview rollout catalog continuity"},
    {11,
     "v289",
     "route-preview-history-fallback-freeze",
     "v288",
     "fixtures/release/shard-readiness-v288.json",
     "freezes route preview historical fallback source"},
    {12,
     "v290",
     "route-preview-operator-handoff",
     "v289",
     "fixtures/release/shard-readiness-v289.json",
     "documents operator handoff for read-only route preview"},
    {13,
     "v291",
     "route-preview-node-consumer-handoff",
     "v290",
     "fixtures/release/shard-readiness-v290.json",
     "documents Node consumer handoff for route preview evidence"},
    {14,
     "v292",
     "route-preview-java-echo-handoff",
     "v291",
     "fixtures/release/shard-readiness-v291.json",
     "documents Java echo handoff for route preview evidence"},
    {15,
     "v293",
     "route-preview-boundary-field-audit",
     "v292",
     "fixtures/release/shard-readiness-v292.json",
     "audits route preview read-only boundary fields"},
    {16,
     "v294",
     "route-preview-no-router-audit",
     "v293",
     "fixtures/release/shard-readiness-v293.json",
     "audits that route preview still installs no active router"},
    {17,
     "v295",
     "route-preview-no-write-audit",
     "v294",
     "fixtures/release/shard-readiness-v294.json",
     "audits that route preview still performs no writes"},
    {18,
     "v296",
     "route-preview-no-execution-audit",
     "v295",
     "fixtures/release/shard-readiness-v295.json",
     "audits that route preview still permits no execution"},
    {19,
     "v297",
     "route-preview-release-package",
     "v296",
     "fixtures/release/shard-readiness-v296.json",
     "packages route preview rollout evidence for release consumption"},
    {20,
     "v298",
     "route-preview-release-package-audit",
     "v297",
     "fixtures/release/shard-readiness-v297.json",
     "audits route preview rollout release package"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::uint64_t fnv1a64(std::string_view text) {
    std::uint64_t hash = 14695981039346656037ull;
    for (const unsigned char ch : text) {
        hash ^= ch;
        hash *= 1099511628211ull;
    }
    return hash;
}

std::string hex64(std::uint64_t value) {
    std::ostringstream output;
    output << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << value;
    return output.str();
}

const PinnedSample* find_pinned_sample(std::string_view key) {
    for (const auto& sample : pinned_samples) {
        if (sample.key == key) {
            return &sample;
        }
    }
    return nullptr;
}

const RolloutStage& current_rollout_stage() {
    return rollout_stages.at(static_cast<std::size_t>(current_rollout_stage_count - 1));
}

std::string format_stage_json(const RolloutStage& stage) {
    return "{\"sequence\":" + std::to_string(stage.sequence) +
           ",\"releaseVersion\":" + json_string(stage.release_version) +
           ",\"stage\":" + json_string(stage.stage) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"scope\":" + json_string(stage.scope) +
           ",\"readOnly\":true"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string format_stage_catalog_json() {
    std::string result = "[";
    for (int index = 0; index < current_rollout_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(rollout_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

} // namespace

RoutePreview preview_key(std::string_view key) {
    const auto hash = fnv1a64(key);

    RoutePreview preview;
    preview.key = std::string{key};
    preview.slot = static_cast<int>(hash % preview_slot_count);
    preview.shard_id = std::string{preview_shard_id};
    preview.route_mode = std::string{preview_route_mode};
    preview.preview_hash = "fnv1a64:" + hex64(hash);

    if (const auto* sample = find_pinned_sample(key); sample != nullptr) {
        preview.slot = sample->slot;
        preview.pinned_fixture_sample = true;
    }

    return preview;
}

std::string format_route_text(std::string_view key) {
    const auto preview = preview_key(key);
    return "slot=" + std::to_string(preview.slot) +
           " shard=" + preview.shard_id +
           " route_mode=" + preview.route_mode +
           " read_only=yes router_installed=no write_routing_allowed=no execution_allowed=no";
}

std::string format_route_json(std::string_view key) {
    const auto preview = preview_key(key);
    return "{\"contract\":\"shard-route-preview.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEJSON\","
           "\"key\":" + json_string(preview.key) +
           ",\"slot\":" + std::to_string(preview.slot) +
           ",\"slotCount\":" + std::to_string(preview_slot_count) +
           ",\"shardId\":" + json_string(preview.shard_id) +
           ",\"routingMode\":\"single-shard-readiness-prototype\","
           "\"routeMode\":" + json_string(preview.route_mode) +
           ",\"previewHash\":" + json_string(preview.preview_hash) +
           ",\"pinnedFixtureSample\":" + json_bool(preview.pinned_fixture_sample) +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"storagePath\":\"not-created\""
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string format_rollout_json() {
    const auto& stage = current_rollout_stage();
    return "{\"rolloutMode\":\"read-only-shard-route-preview-rollout\","
           "\"sourceNodePlan\":\"docs/plans3/v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"rolloutStage\":" + json_string(stage.stage) +
           ",\"rolloutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"rolloutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_rollout_stage_count) +
           ",\"latestStageScope\":" + json_string(stage.scope) +
           ",\"previewCommandAvailable\":true"
           ",\"previewJsonCommandAvailable\":true"
           ",\"commands\":" + json_string_array({"SHARDROUTE", "SHARDROUTEJSON"}) +
           ",\"sampleKeys\":" + json_string_array({"orderops:alpha", "audit:receipt:001", "mini-kv:health"}) +
           ",\"sampleSlotsPinnedToExistingEvidence\":true"
           ",\"stageCatalog\":" + format_stage_catalog_json() +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"storageDirectoriesCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string rollout_digest_marker() {
    const auto& stage = current_rollout_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-stage-" +
           std::to_string(stage.sequence);
}

} // namespace minikv::shard_route_preview
