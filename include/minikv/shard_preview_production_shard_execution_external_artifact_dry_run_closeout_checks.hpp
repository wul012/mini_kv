#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks {

struct ProductionShardExecutionExternalArtifactDryRunCloseoutCheck {
    int sequence;
    std::string_view check_code;
    std::string_view artifact_slot;
    std::string_view protects;
};

std::span<const ProductionShardExecutionExternalArtifactDryRunCloseoutCheck>
production_shard_execution_external_artifact_dry_run_closeout_checks();
int planned_production_shard_execution_external_artifact_dry_run_closeout_check_count();
std::string format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks
