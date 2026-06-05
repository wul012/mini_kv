#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>
#include <string>

namespace minikv::shard_route_preview_stage_chain {

struct StageChainReport {
    int published_stage_count = 0;
    int planned_stage_count = 0;
    int first_release_number = 0;
    int last_published_release_number = 0;
    bool published_count_within_catalog = false;
    bool published_count_matches_plan = false;
    bool sequences_contiguous = false;
    bool release_versions_contiguous = false;
    bool source_frozen_release_versions_contiguous = false;
    bool source_frozen_fixture_paths_contiguous = false;
    bool chain_complete = false;
};

StageChainReport inspect_stage_chain(std::span<const shard_route_preview_stage_catalog::StageRecord> stages,
                                     int published_stage_count,
                                     int planned_stage_count,
                                     int first_release_number);

std::string format_stage_chain_report_json(const StageChainReport& report);

} // namespace minikv::shard_route_preview_stage_chain
