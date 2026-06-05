#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_stage_catalog {

struct StageRecord {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

std::string format_stage_json(const StageRecord& stage);
std::string format_stage_catalog_json(std::span<const StageRecord> stages, int published_stage_count);
std::string format_digest_marker(const StageRecord& stage, int published_stage_count, int planned_stage_count);

} // namespace minikv::shard_route_preview_stage_catalog
