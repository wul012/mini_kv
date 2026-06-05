#include "minikv/shard_route_preview_stage_catalog.hpp"

#include "minikv/runtime_evidence.hpp"

#include <stdexcept>

namespace minikv::shard_route_preview_stage_catalog {

namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::string format_stage_json(const StageRecord& stage) {
    return "{\"sequence\":" + std::to_string(stage.sequence) +
           ",\"releaseVersion\":" + json_string(stage.release_version) +
           ",\"stage\":" + json_string(stage.stage) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"scope\":" + json_string(stage.scope) +
           ",\"readOnly\":true"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string format_stage_catalog_json(std::span<const StageRecord> stages, int published_stage_count) {
    if (published_stage_count < 0 || static_cast<std::size_t>(published_stage_count) > stages.size()) {
        throw std::out_of_range{"published stage count exceeds stage catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(stages[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

std::string format_digest_marker(const StageRecord& stage, int published_stage_count, int planned_stage_count) {
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-" +
           std::to_string(published_stage_count) + "-of-" + std::to_string(planned_stage_count) + "-stages";
}

} // namespace minikv::shard_route_preview_stage_catalog
