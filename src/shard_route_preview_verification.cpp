#include "minikv/shard_route_preview_verification.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification {
namespace {

constexpr int preview_slot_count = 16;
constexpr std::string_view preview_shard_id = "shard-0";
constexpr std::string_view preview_route_mode = "preview-only";
constexpr int current_verification_stage_count = 2;

struct PinnedSampleExpectation {
    std::string_view key;
    int slot;
};

struct VerificationStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<PinnedSampleExpectation, 3> pinned_samples = {{
    {"orderops:alpha", 5},
    {"audit:receipt:001", 9},
    {"mini-kv:health", 14},
}};

constexpr std::array<VerificationStage, 21> verification_stages = {{
    {1,
     "v299",
     "route-preview-verification-command-contract",
     "v298",
     "fixtures/release/shard-readiness-v298.json",
     "adds read-only SHARDROUTEVERIFYJSON key verification command"},
    {2,
     "v300",
     "route-preview-verification-command-catalog-alignment",
     "v299",
     "fixtures/release/shard-readiness-v299.json",
     "aligns route preview verification with command catalog and help surfaces"},
    {3,
     "v301",
     "route-preview-verification-explain-contract",
     "v300",
     "fixtures/release/shard-readiness-v300.json",
     "freezes EXPLAINJSON route preview verification semantics as metadata read only"},
    {4,
     "v302",
     "route-preview-verification-checkjson-contract",
     "v301",
     "fixtures/release/shard-readiness-v301.json",
     "freezes CHECKJSON route preview verification no-execution contract"},
    {5,
     "v303",
     "route-preview-verification-cli-smoke-contract",
     "v302",
     "fixtures/release/shard-readiness-v302.json",
     "records CLI smoke expectations for read-only route preview verification"},
    {6,
     "v304",
     "route-preview-verification-tcp-smoke-contract",
     "v303",
     "fixtures/release/shard-readiness-v303.json",
     "records TCP smoke expectations for read-only route preview verification"},
    {7,
     "v305",
     "route-preview-verification-fixture-freeze",
     "v304",
     "fixtures/release/shard-readiness-v304.json",
     "freezes route preview verification rollout fixture baseline"},
    {8,
     "v306",
     "route-preview-verification-sample-parity-audit",
     "v305",
     "fixtures/release/shard-readiness-v305.json",
     "audits pinned sample verification against existing route preview samples"},
    {9,
     "v307",
     "route-preview-verification-rollout-catalog",
     "v306",
     "fixtures/release/shard-readiness-v306.json",
     "catalogs route preview verification rollout stages"},
    {10,
     "v308",
     "route-preview-verification-rollout-catalog-audit",
     "v307",
     "fixtures/release/shard-readiness-v307.json",
     "audits route preview verification rollout catalog continuity"},
    {11,
     "v309",
     "route-preview-verification-history-fallback-freeze",
     "v308",
     "fixtures/release/shard-readiness-v308.json",
     "freezes route preview verification historical fallback source"},
    {12,
     "v310",
     "route-preview-verification-operator-handoff",
     "v309",
     "fixtures/release/shard-readiness-v309.json",
     "documents operator handoff for read-only route preview verification"},
    {13,
     "v311",
     "route-preview-verification-node-consumer-handoff",
     "v310",
     "fixtures/release/shard-readiness-v310.json",
     "documents Node consumer handoff for route preview verification evidence"},
    {14,
     "v312",
     "route-preview-verification-java-echo-handoff",
     "v311",
     "fixtures/release/shard-readiness-v311.json",
     "documents Java echo handoff for route preview verification evidence"},
    {15,
     "v313",
     "route-preview-verification-boundary-field-audit",
     "v312",
     "fixtures/release/shard-readiness-v312.json",
     "audits route preview verification read-only boundary fields"},
    {16,
     "v314",
     "route-preview-verification-no-router-audit",
     "v313",
     "fixtures/release/shard-readiness-v313.json",
     "audits that route preview verification still installs no active router"},
    {17,
     "v315",
     "route-preview-verification-no-write-audit",
     "v314",
     "fixtures/release/shard-readiness-v314.json",
     "audits that route preview verification still performs no writes"},
    {18,
     "v316",
     "route-preview-verification-no-execution-audit",
     "v315",
     "fixtures/release/shard-readiness-v315.json",
     "audits that route preview verification still permits no execution"},
    {19,
     "v317",
     "route-preview-verification-release-package",
     "v316",
     "fixtures/release/shard-readiness-v316.json",
     "packages route preview verification rollout evidence for release consumption"},
    {20,
     "v318",
     "route-preview-verification-release-package-audit",
     "v317",
     "fixtures/release/shard-readiness-v317.json",
     "audits route preview verification rollout release package"},
    {21,
     "v319",
     "route-preview-verification-closeout-audit",
     "v318",
     "fixtures/release/shard-readiness-v318.json",
     "audits route preview verification closeout continuity"},
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

const VerificationStage& current_stage() {
    return verification_stages.at(static_cast<std::size_t>(current_verification_stage_count - 1));
}

const PinnedSampleExpectation* find_pinned_sample(std::string_view key) {
    for (const auto& sample : pinned_samples) {
        if (sample.key == key) {
            return &sample;
        }
    }
    return nullptr;
}

bool has_fnv1a64_prefix(std::string_view value) {
    return value.rfind("fnv1a64:", 0) == 0 && value.size() == 24;
}

int count_passed(const RoutePreviewVerification& verification) {
    int passed = 0;
    passed += verification.slot_within_range ? 1 : 0;
    passed += verification.single_shard ? 1 : 0;
    passed += verification.preview_only ? 1 : 0;
    passed += verification.pinned_sample_consistent ? 1 : 0;
    passed += verification.hash_present ? 1 : 0;
    passed += verification.read_only ? 1 : 0;
    passed += verification.no_active_router ? 1 : 0;
    passed += verification.no_write_routing ? 1 : 0;
    passed += verification.no_execution ? 1 : 0;
    return passed;
}

std::string format_check_json(std::string_view name, bool passed) {
    return "{\"name\":" + json_string(name) +
           ",\"passed\":" + json_bool(passed) +
           ",\"readOnly\":true"
           ",\"executesRoute\":false"
           ",\"mutatesStore\":false}";
}

std::string format_checks_json(const RoutePreviewVerification& verification) {
    return "[" +
           format_check_json("slot_within_range", verification.slot_within_range) + "," +
           format_check_json("single_shard", verification.single_shard) + "," +
           format_check_json("preview_only", verification.preview_only) + "," +
           format_check_json("pinned_sample_consistent", verification.pinned_sample_consistent) + "," +
           format_check_json("hash_present", verification.hash_present) + "," +
           format_check_json("read_only", verification.read_only) + "," +
           format_check_json("no_active_router", verification.no_active_router) + "," +
           format_check_json("no_write_routing", verification.no_write_routing) + "," +
           format_check_json("no_execution", verification.no_execution) + "]";
}

std::string format_stage_json(const VerificationStage& stage) {
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
    for (int index = 0; index < current_verification_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(verification_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

} // namespace

RoutePreviewVerification verify_key(std::string_view key) {
    RoutePreviewVerification verification;
    verification.preview = shard_route_preview::preview_key(key);
    verification.slot_within_range = verification.preview.slot >= 0 && verification.preview.slot < preview_slot_count;
    verification.single_shard = verification.preview.shard_id == preview_shard_id;
    verification.preview_only = verification.preview.route_mode == preview_route_mode;
    verification.hash_present = has_fnv1a64_prefix(verification.preview.preview_hash);

    if (const auto* sample = find_pinned_sample(key); sample != nullptr) {
        verification.pinned_sample_consistent =
            verification.preview.pinned_fixture_sample && verification.preview.slot == sample->slot;
    } else {
        verification.pinned_sample_consistent = !verification.preview.pinned_fixture_sample;
    }

    verification.check_count = 9;
    verification.passed_count = count_passed(verification);
    return verification;
}

std::string format_verification_json(std::string_view key) {
    const auto verification = verify_key(key);
    const auto& stage = current_stage();
    return "{\"contract\":\"shard-route-preview-verification.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYJSON\","
           "\"checkedCommand\":\"SHARDROUTEJSON\","
           "\"verificationMode\":\"read-only-route-preview-field-check\","
           "\"key\":" + json_string(verification.preview.key) +
           ",\"slot\":" + std::to_string(verification.preview.slot) +
           ",\"slotCount\":" + std::to_string(preview_slot_count) +
           ",\"shardId\":" + json_string(verification.preview.shard_id) +
           ",\"routeMode\":" + json_string(verification.preview.route_mode) +
           ",\"previewHash\":" + json_string(verification.preview.preview_hash) +
           ",\"pinnedFixtureSample\":" + json_bool(verification.preview.pinned_fixture_sample) +
           ",\"passed\":" + json_bool(verification.passed_count == verification.check_count) +
           ",\"checkCount\":" + std::to_string(verification.check_count) +
           ",\"passedCount\":" + std::to_string(verification.passed_count) +
           ",\"failedCount\":" + std::to_string(verification.check_count - verification.passed_count) +
           ",\"checks\":" + format_checks_json(verification) +
           ",\"rolloutStage\":" + json_string(stage.stage) +
           ",\"rolloutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"rolloutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"executesRoute\":false"
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
    const auto& stage = current_stage();
    return "{\"rolloutMode\":\"read-only-shard-route-preview-verification-rollout\","
           "\"sourceNodePlan\":\"docs/plans3/v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"rolloutStage\":" + json_string(stage.stage) +
           ",\"rolloutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"rolloutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_verification_stage_count) +
           ",\"latestStageScope\":" + json_string(stage.scope) +
           ",\"verificationCommandAvailable\":true"
           ",\"verifiedCommand\":\"SHARDROUTEJSON\""
           ",\"commands\":" + json_string_array({"SHARDROUTEVERIFYJSON"}) +
           ",\"sampleKeys\":" + json_string_array({"orderops:alpha", "audit:receipt:001", "mini-kv:health"}) +
           ",\"checks\":" + json_string_array({"slot_within_range",
                                               "single_shard",
                                               "preview_only",
                                               "pinned_sample_consistent",
                                               "hash_present",
                                               "read_only",
                                               "no_active_router",
                                               "no_write_routing",
                                               "no_execution"}) +
           ",\"stageCatalog\":" + format_stage_catalog_json() +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"executesRoute\":false"
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
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-stage-" +
           std::to_string(stage.sequence);
}

} // namespace minikv::shard_route_preview_verification
