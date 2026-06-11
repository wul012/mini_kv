#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages.hpp"

#include <array>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 5> stages{{
    {1, "v1602", "production-shard-execution-external-artifact-dry-run-closeout-plan-intake", "v1601",
     "fixtures/release/shard-readiness-v1601.json",
     "freezes the v1601 mini-kv owner receipt bundle before external artifact closeout intake"},
    {2, "v1603", "production-shard-execution-external-artifact-dry-run-closeout-batch-stop", "v1602",
     "fixtures/release/shard-readiness-v1602.json",
     "records that Node dry-run artifact growth should stop until a real external artifact arrives"},
    {3, "v1604", "production-shard-execution-external-artifact-dry-run-closeout-missing-artifact-gate", "v1603",
     "fixtures/release/shard-readiness-v1603.json",
     "keeps real external artifact intake absent and production execution blocked"},
    {4, "v1605", "production-shard-execution-external-artifact-dry-run-closeout-real-receipt-gap", "v1604",
     "fixtures/release/shard-readiness-v1604.json",
     "keeps real signed approval, managed audit binding, Java, mini-kv signed, and cleanup receipts missing"},
    {5, "v1606", "production-shard-execution-external-artifact-dry-run-closeout-clean-ci-closeout", "v1605",
     "fixtures/release/shard-readiness-v1605.json",
     "closes the external artifact dry-run closeout evidence with f-root archives and no production authority"},
}};

} // namespace

std::span<const StageRecord> production_shard_execution_external_artifact_dry_run_closeout_stages() {
    return stages;
}

int first_production_shard_execution_external_artifact_dry_run_closeout_release_number() {
    return 1602;
}

int planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages
