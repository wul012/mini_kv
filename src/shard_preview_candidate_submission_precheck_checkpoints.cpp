#include "minikv/shard_preview_candidate_submission_precheck_checkpoints.hpp"

#include <array>

namespace minikv::shard_preview_candidate_submission_precheck_checkpoints {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_submission_precheck_release_number_value = 921;

constexpr std::array<StageRecord, 10> candidate_submission_precheck_checkpoint_catalog = {{
    {1, "v921", "route-preview-candidate-submission-precheck-source-freeze", "v920",
     "fixtures/release/shard-readiness-v920.json",
     "freezes the v920 integrity fixture before submission precheck handoff starts"},
    {2, "v922", "route-preview-candidate-submission-precheck-checkpoint-catalog", "v921",
     "fixtures/release/shard-readiness-v921.json",
     "declares mini-kv submission precheck checkpoints without opening intake"},
    {3, "v923", "route-preview-candidate-submission-precheck-validator-catalog", "v922",
     "fixtures/release/shard-readiness-v922.json",
     "declares validator readiness while keeping real document reads disabled"},
    {4, "v924", "route-preview-candidate-submission-precheck-field-carry-forward", "v923",
     "fixtures/release/shard-readiness-v923.json",
     "carries the twenty requested candidate fields from the frozen request package"},
    {5, "v925", "route-preview-candidate-submission-precheck-gate-alignment", "v924",
     "fixtures/release/shard-readiness-v924.json",
     "records Node v1411 forty gate expectation as read-only metadata"},
    {6, "v926", "route-preview-candidate-submission-precheck-document-absence-boundary", "v925",
     "fixtures/release/shard-readiness-v925.json",
     "keeps reviewed real and synthetic candidate documents absent"},
    {7, "v927", "route-preview-candidate-submission-precheck-payload-intake-boundary", "v926",
     "fixtures/release/shard-readiness-v926.json",
     "keeps staged payload import and candidate materialization disabled"},
    {8, "v928", "route-preview-candidate-submission-precheck-approval-signature-boundary", "v927",
     "fixtures/release/shard-readiness-v927.json",
     "keeps approval capture signature parsing and execution disabled"},
    {9, "v929", "route-preview-candidate-submission-precheck-command-smoke-boundary", "v928",
     "fixtures/release/shard-readiness-v928.json",
     "keeps CLI and TCP smoke scoped to the short read-only precheck command"},
    {10, "v930", "route-preview-candidate-submission-precheck-closeout-summary", "v929",
     "fixtures/release/shard-readiness-v929.json",
     "closes the submission precheck handoff without accepting a candidate document"},
}};

} // namespace

std::span<const StageRecord> candidate_submission_precheck_checkpoints() {
    return {candidate_submission_precheck_checkpoint_catalog.data(),
            candidate_submission_precheck_checkpoint_catalog.size()};
}

int first_candidate_submission_precheck_release_number() {
    return first_candidate_submission_precheck_release_number_value;
}

int planned_candidate_submission_precheck_checkpoint_count() {
    return static_cast<int>(candidate_submission_precheck_checkpoint_catalog.size());
}

} // namespace minikv::shard_preview_candidate_submission_precheck_checkpoints
