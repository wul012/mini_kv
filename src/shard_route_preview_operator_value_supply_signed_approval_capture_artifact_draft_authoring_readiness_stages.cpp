#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_authoring_readiness_release_number = 736;

constexpr std::array<StageRecord, 25> authoring_readiness_stages = {{
    {1, "v736", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-request-manifest", "v735",
     "fixtures/release/shard-readiness-v735.json",
     "starts draft authoring readiness with request manifest requirements and no draft text"},
    {2, "v737", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-artifact-preflight-digest", "v736",
     "fixtures/release/shard-readiness-v736.json",
     "pins the artifact preflight closeout digest before any signed draft wording"},
    {3, "v738", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-template-digest", "v737",
     "fixtures/release/shard-readiness-v737.json",
     "pins signed approval template digest for later human authoring"},
    {4, "v739", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-review-digest", "v738",
     "fixtures/release/shard-readiness-v738.json",
     "pins approval review digest without authoring a statement"},
    {5, "v740", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-operator-identity", "v739",
     "fixtures/release/shard-readiness-v739.json",
     "requires reviewed operator identity while credentials remain absent"},
    {6, "v741", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-operator-role", "v740",
     "fixtures/release/shard-readiness-v740.json",
     "requires reviewed operator role without enabling approval capture"},
    {7, "v742", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-window-id", "v741",
     "fixtures/release/shard-readiness-v741.json",
     "requires capture window id while keeping the window closed"},
    {8, "v743", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-channel-policy", "v742",
     "fixtures/release/shard-readiness-v742.json",
     "requires channel policy while raw endpoint and channel values stay blocked"},
    {9, "v744", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-signature-algorithm", "v743",
     "fixtures/release/shard-readiness-v743.json",
     "requires signature algorithm policy without accepting signature material"},
    {10, "v745", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-detached-signature", "v744",
     "fixtures/release/shard-readiness-v744.json",
     "requires detached signature handling to remain external and absent"},
    {11, "v746", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-signature-redaction", "v745",
     "fixtures/release/shard-readiness-v745.json",
     "requires signature redaction policy while raw signature bytes stay blocked"},
    {12, "v747", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-statement-digest", "v746",
     "fixtures/release/shard-readiness-v746.json",
     "requires approval statement digest without generating signed approval text"},
    {13, "v748", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-source-version", "v747",
     "fixtures/release/shard-readiness-v747.json",
     "keeps source evidence version visible for manual citation"},
    {14, "v749", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-source-file", "v748",
     "fixtures/release/shard-readiness-v748.json",
     "keeps source file id visible without runtime file lookup"},
    {15, "v750", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-source-snippet", "v749",
     "fixtures/release/shard-readiness-v749.json",
     "keeps source snippet id visible without assembling a draft artifact"},
    {16, "v751", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-redacted-value-digest", "v750",
     "fixtures/release/shard-readiness-v750.json",
     "pins redacted value digest and keeps raw operator value material absent"},
    {17, "v752", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-value-shape", "v751",
     "fixtures/release/shard-readiness-v751.json",
     "exposes value shape metadata without accepting supplied values"},
    {18, "v753", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-redaction-policy", "v752",
     "fixtures/release/shard-readiness-v752.json",
     "binds redaction policy and prevents raw secret leakage"},
    {19, "v754", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-provenance-policy", "v753",
     "fixtures/release/shard-readiness-v753.json",
     "keeps provenance policy visible for immutable evidence citation"},
    {20, "v755", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-raw-secret-embargo", "v754",
     "fixtures/release/shard-readiness-v754.json",
     "proves draft authoring readiness carries no raw secret value"},
    {21, "v756", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-approval-grant-embargo", "v755",
     "fixtures/release/shard-readiness-v755.json",
     "proves no approval grant exists and authoring remains separate from capture"},
    {22, "v757", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-zero-value-import-embargo", "v756",
     "fixtures/release/shard-readiness-v756.json",
     "keeps supplied accepted and imported operator value counts at zero"},
    {23, "v758", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-write-route-embargo", "v757",
     "fixtures/release/shard-readiness-v757.json",
     "keeps write routing blocked until a separate approved path exists"},
    {24, "v759", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-sibling-non-mutation", "v758",
     "fixtures/release/shard-readiness-v758.json",
     "proves Java Node and mini-kv sibling state are not mutated by readiness evidence"},
    {25, "v760", "route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-closeout", "v759",
     "fixtures/release/shard-readiness-v759.json",
     "closes authoring readiness before any real signed approval artifact draft package"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_draft_authoring_readiness_stages() {
    return {authoring_readiness_stages.data(), authoring_readiness_stages.size()};
}

int first_signed_approval_capture_artifact_draft_authoring_readiness_release_number() {
    return first_authoring_readiness_release_number;
}

int planned_signed_approval_capture_artifact_draft_authoring_readiness_stage_count() {
    return static_cast<int>(authoring_readiness_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages
