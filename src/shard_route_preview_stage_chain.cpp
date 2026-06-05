#include "minikv/shard_route_preview_stage_chain.hpp"

#include "minikv/runtime_evidence.hpp"

#include <cstddef>
#include <string>

namespace minikv::shard_route_preview_stage_chain {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string release_label(int release_number) {
    return "v" + std::to_string(release_number);
}

std::string fixture_path(int release_number) {
    return "fixtures/release/shard-readiness-v" + std::to_string(release_number) + ".json";
}

} // namespace

StageChainReport inspect_stage_chain(std::span<const shard_route_preview_stage_catalog::StageRecord> stages,
                                     int published_stage_count,
                                     int planned_stage_count,
                                     int first_release_number) {
    StageChainReport report;
    report.published_stage_count = published_stage_count;
    report.planned_stage_count = planned_stage_count;
    report.first_release_number = first_release_number;
    report.last_published_release_number =
        published_stage_count > 0 ? first_release_number + published_stage_count - 1 : first_release_number - 1;
    report.published_count_within_catalog =
        published_stage_count >= 0 && static_cast<std::size_t>(published_stage_count) <= stages.size();
    report.published_count_matches_plan =
        report.published_count_within_catalog && published_stage_count == planned_stage_count;

    if (!report.published_count_within_catalog || published_stage_count == 0) {
        return report;
    }

    report.sequences_contiguous = true;
    report.release_versions_contiguous = true;
    report.source_frozen_release_versions_contiguous = true;
    report.source_frozen_fixture_paths_contiguous = true;

    for (int index = 0; index < published_stage_count; ++index) {
        const auto& stage = stages[static_cast<std::size_t>(index)];
        const int release_number = first_release_number + index;
        const int source_release_number = release_number - 1;

        report.sequences_contiguous = report.sequences_contiguous && stage.sequence == index + 1;
        report.release_versions_contiguous =
            report.release_versions_contiguous && std::string{stage.release_version} == release_label(release_number);
        report.source_frozen_release_versions_contiguous =
            report.source_frozen_release_versions_contiguous &&
            std::string{stage.source_frozen_release_version} == release_label(source_release_number);
        report.source_frozen_fixture_paths_contiguous =
            report.source_frozen_fixture_paths_contiguous &&
            std::string{stage.source_frozen_fixture_path} == fixture_path(source_release_number);
    }

    report.chain_complete = report.published_count_matches_plan && report.sequences_contiguous &&
                            report.release_versions_contiguous &&
                            report.source_frozen_release_versions_contiguous &&
                            report.source_frozen_fixture_paths_contiguous;
    return report;
}

std::string format_stage_chain_report_json(const StageChainReport& report) {
    return "{\"publishedStageCount\":" + std::to_string(report.published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(report.planned_stage_count) +
           ",\"firstReleaseVersion\":\"v" + std::to_string(report.first_release_number) +
           "\",\"lastPublishedReleaseVersion\":\"v" + std::to_string(report.last_published_release_number) +
           "\",\"publishedCountWithinCatalog\":" + json_bool(report.published_count_within_catalog) +
           ",\"publishedCountMatchesPlan\":" + json_bool(report.published_count_matches_plan) +
           ",\"sequencesContiguous\":" + json_bool(report.sequences_contiguous) +
           ",\"releaseVersionsContiguous\":" + json_bool(report.release_versions_contiguous) +
           ",\"sourceFrozenReleaseVersionsContiguous\":" +
           json_bool(report.source_frozen_release_versions_contiguous) +
           ",\"sourceFrozenFixturePathsContiguous\":" +
           json_bool(report.source_frozen_fixture_paths_contiguous) +
           ",\"chainComplete\":" + json_bool(report.chain_complete) +
           ",\"readOnly\":true"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_stage_chain
