#include "minikv/shard_preview_candidate_material_request_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_material_request_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_material_request_release_number_value = 956;

constexpr std::array<StageRecord, 10> candidate_material_request_stage_catalog = {{
    {1, "v956", "route-preview-candidate-material-request-source-freeze", "v955",
     "fixtures/release/shard-readiness-v955.json",
     "freezes the v955 intake packet before material request closeout starts"},
    {2, "v957", "route-preview-candidate-material-request-node-plan-pin", "v956",
     "fixtures/release/shard-readiness-v956.json",
     "pins Node v1446 material request package as read-only planning context"},
    {3, "v958", "route-preview-candidate-material-request-item-catalog", "v957",
     "fixtures/release/shard-readiness-v957.json",
     "declares twenty-five external material request items without reading material"},
    {4, "v959", "route-preview-candidate-material-request-acceptance-check-catalog", "v958",
     "fixtures/release/shard-readiness-v958.json",
     "declares twenty-five acceptance checks without accepting candidate documents"},
    {5, "v960", "route-preview-candidate-material-request-field-map", "v959",
     "fixtures/release/shard-readiness-v959.json",
     "carries twenty requested material fields as metadata only"},
    {6, "v961", "route-preview-candidate-material-request-external-material-boundary", "v960",
     "fixtures/release/shard-readiness-v960.json",
     "keeps external reviewed real material absent unread and unaccepted"},
    {7, "v962", "route-preview-candidate-material-request-import-evaluation-boundary", "v961",
     "fixtures/release/shard-readiness-v961.json",
     "keeps payload import staging evaluation acceptance and rejection disabled"},
    {8, "v963", "route-preview-candidate-material-request-router-write-wal-boundary", "v962",
     "fixtures/release/shard-readiness-v962.json",
     "keeps document router write routing store mutation and WAL touch disabled"},
    {9, "v964", "route-preview-candidate-material-request-cli-tcp-smoke-boundary", "v963",
     "fixtures/release/shard-readiness-v963.json",
     "keeps CLI and TCP smoke scoped to the short read-only material request command"},
    {10, "v965", "route-preview-candidate-material-request-closeout-summary", "v964",
     "fixtures/release/shard-readiness-v964.json",
     "closes the material request package without consuming reviewed material"},
}};

} // namespace

std::span<const StageRecord> candidate_material_request_stages() {
    return {candidate_material_request_stage_catalog.data(), candidate_material_request_stage_catalog.size()};
}

int first_candidate_material_request_release_number() {
    return first_candidate_material_request_release_number_value;
}

int planned_candidate_material_request_stage_count() {
    return static_cast<int>(candidate_material_request_stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_material_request_stages
