#include "minikv/shard_preview_candidate_material_submission_precheck_checkpoints.hpp"

#include <array>

namespace minikv::shard_preview_candidate_material_submission_precheck_checkpoints {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_material_submission_precheck_release_number_value = 976;

constexpr std::array<StageRecord, 10> checkpoint_catalog = {{
    {1, "v976", "route-preview-candidate-material-submission-precheck-source-freeze", "v975",
     "fixtures/release/shard-readiness-v975.json",
     "freezes the v975 material request integrity fixture before material submission precheck starts"},
    {2, "v977", "route-preview-candidate-material-submission-precheck-source-package-checkpoint", "v976",
     "fixtures/release/shard-readiness-v976.json",
     "maps the frozen v965 material request package into submission checkpoint form"},
    {3, "v978", "route-preview-candidate-material-submission-precheck-reviewer-identity-checkpoint", "v977",
     "fixtures/release/shard-readiness-v977.json",
     "keeps reviewer identity and source ownership as handles only"},
    {4, "v979", "route-preview-candidate-material-submission-precheck-origin-and-digest-checkpoint", "v978",
     "fixtures/release/shard-readiness-v978.json",
     "requires origin digest and canonical body shape without reading body content"},
    {5, "v980", "route-preview-candidate-material-submission-precheck-field-table-checkpoint", "v979",
     "fixtures/release/shard-readiness-v979.json",
     "declares twenty required material fields and twenty submission material fields"},
    {6, "v981", "route-preview-candidate-material-submission-precheck-comparison-binding-checkpoint", "v980",
     "fixtures/release/shard-readiness-v980.json",
     "keeps comparison binding and request lineage visible without evaluating material"},
    {7, "v982", "route-preview-candidate-material-submission-precheck-signature-attestation-checkpoint", "v981",
     "fixtures/release/shard-readiness-v981.json",
     "requires signature attestation metadata without parsing or verifying signatures"},
    {8, "v983", "route-preview-candidate-material-submission-precheck-redaction-secret-boundary-checkpoint", "v982",
     "fixtures/release/shard-readiness-v982.json",
     "keeps redaction secret and credential values absent from mini-kv evidence"},
    {9, "v984", "route-preview-candidate-material-submission-precheck-runtime-import-freeze-checkpoint", "v983",
     "fixtures/release/shard-readiness-v983.json",
     "keeps runtime payload import material intake router writes WAL and execution disabled"},
    {10, "v985", "route-preview-candidate-material-submission-precheck-closeout-archive-checkpoint", "v984",
     "fixtures/release/shard-readiness-v984.json",
     "closes the material submission precheck archive without accepting reviewed material"},
}};

} // namespace

std::span<const StageRecord> candidate_material_submission_precheck_checkpoints() {
    return {checkpoint_catalog.data(), checkpoint_catalog.size()};
}

int first_candidate_material_submission_precheck_release_number() {
    return first_candidate_material_submission_precheck_release_number_value;
}

int planned_candidate_material_submission_precheck_checkpoint_count() {
    return static_cast<int>(checkpoint_catalog.size());
}

} // namespace minikv::shard_preview_candidate_material_submission_precheck_checkpoints
