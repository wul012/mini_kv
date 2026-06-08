#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_instruction_preflight_release_number = 761;

constexpr std::array<StageRecord, 25> instruction_preflight_stages = {{
    {1, "v761", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-request-manifest-slot", "v760",
     "fixtures/release/shard-readiness-v760.json",
     "starts draft instruction preflight with a request manifest slot and no materialized instruction text"},
    {2, "v762", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-artifact-preflight-digest-slot", "v761",
     "fixtures/release/shard-readiness-v761.json",
     "pins artifact preflight digest instruction input without creating a signed draft"},
    {3, "v763", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-template-digest-slot", "v762",
     "fixtures/release/shard-readiness-v762.json",
     "pins template digest instruction input while draft text stays absent"},
    {4, "v764", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-review-digest-slot", "v763",
     "fixtures/release/shard-readiness-v763.json",
     "pins review digest instruction input without approval statement materialization"},
    {5, "v765", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-operator-identity-slot", "v764",
     "fixtures/release/shard-readiness-v764.json",
     "preflights operator identity instruction material while credentials remain absent"},
    {6, "v766", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-operator-role-slot", "v765",
     "fixtures/release/shard-readiness-v765.json",
     "preflights operator role instruction material without enabling approval capture"},
    {7, "v767", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-window-id-slot", "v766",
     "fixtures/release/shard-readiness-v766.json",
     "preflights capture window id instruction material while the window remains closed"},
    {8, "v768", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-channel-policy-slot", "v767",
     "fixtures/release/shard-readiness-v767.json",
     "preflights channel policy instruction material without raw endpoint storage"},
    {9, "v769", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-signature-algorithm-slot", "v768",
     "fixtures/release/shard-readiness-v768.json",
     "preflights signature algorithm instruction material without signature bytes"},
    {10, "v770", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-detached-signature-slot", "v769",
     "fixtures/release/shard-readiness-v769.json",
     "preflights detached signature instruction material while payload capture stays blocked"},
    {11, "v771", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-signature-redaction-slot", "v770",
     "fixtures/release/shard-readiness-v770.json",
     "preflights signature redaction instruction material while raw signature material stays absent"},
    {12, "v772", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-approval-statement-digest-slot", "v771",
     "fixtures/release/shard-readiness-v771.json",
     "preflights approval statement digest instruction material without writing statement text"},
    {13, "v773", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-source-version-slot", "v772",
     "fixtures/release/shard-readiness-v772.json",
     "preflights source version citation instruction material without live source lookup"},
    {14, "v774", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-source-file-slot", "v773",
     "fixtures/release/shard-readiness-v773.json",
     "preflights source file citation instruction material without runtime file walking"},
    {15, "v775", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-source-snippet-slot", "v774",
     "fixtures/release/shard-readiness-v774.json",
     "preflights source snippet citation instruction material without assembling draft text"},
    {16, "v776", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-redacted-value-digest-slot", "v775",
     "fixtures/release/shard-readiness-v775.json",
     "preflights redacted value digest instruction material while raw values remain absent"},
    {17, "v777", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-value-shape-slot", "v776",
     "fixtures/release/shard-readiness-v776.json",
     "preflights value shape instruction material without accepting supplied values"},
    {18, "v778", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-redaction-policy-slot", "v777",
     "fixtures/release/shard-readiness-v777.json",
     "preflights redaction policy instruction material and keeps raw secrets embargoed"},
    {19, "v779", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-provenance-policy-slot", "v778",
     "fixtures/release/shard-readiness-v778.json",
     "preflights provenance policy instruction material for later manual review"},
    {20, "v780", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-raw-secret-embargo-slot", "v779",
     "fixtures/release/shard-readiness-v779.json",
     "preflights raw secret embargo instruction material and proves no secret storage"},
    {21, "v781", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-approval-grant-embargo-slot", "v780",
     "fixtures/release/shard-readiness-v780.json",
     "preflights approval grant embargo instruction material without grant emission"},
    {22, "v782", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-zero-value-import-embargo-slot", "v781",
     "fixtures/release/shard-readiness-v781.json",
     "preflights zero value import embargo instruction material with all value counts at zero"},
    {23, "v783", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-write-route-embargo-slot", "v782",
     "fixtures/release/shard-readiness-v782.json",
     "preflights write route embargo instruction material while active routing stays disabled"},
    {24, "v784", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-sibling-non-mutation-slot", "v783",
     "fixtures/release/shard-readiness-v783.json",
     "preflights sibling non-mutation instruction material without touching Node Java or mini-kv sibling state"},
    {25, "v785", "route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-closeout", "v784",
     "fixtures/release/shard-readiness-v784.json",
     "closes draft instruction preflight before any signed draft text or approval payload exists"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_draft_instruction_preflight_stages() {
    return {instruction_preflight_stages.data(), instruction_preflight_stages.size()};
}

int first_signed_approval_capture_artifact_draft_instruction_preflight_release_number() {
    return first_instruction_preflight_release_number;
}

int planned_signed_approval_capture_artifact_draft_instruction_preflight_stage_count() {
    return static_cast<int>(instruction_preflight_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages