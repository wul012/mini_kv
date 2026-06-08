#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_draft_text_package_review_preflight_release_number = 786;

constexpr std::array<StageRecord, 25> draft_text_package_review_preflight_stages = {{
    {1, "v786", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-package-manifest-criterion", "v785",
     "fixtures/release/shard-readiness-v785.json",
     "starts draft text package review preflight with package manifest criteria and no package parsing"},
    {2, "v787", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-instruction-preflight-digest-criterion", "v786",
     "fixtures/release/shard-readiness-v786.json",
     "binds the v785 instruction preflight closeout digest before any review package is accepted"},
    {3, "v788", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-package-schema-version-criterion", "v787",
     "fixtures/release/shard-readiness-v787.json",
     "checks the expected package schema version without parsing submitted text"},
    {4, "v789", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-request-correlation-criterion", "v788",
     "fixtures/release/shard-readiness-v788.json",
     "requires request correlation metadata while keeping approval capture closed"},
    {5, "v790", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-artifact-digest-binding-criterion", "v789",
     "fixtures/release/shard-readiness-v789.json",
     "requires artifact digest binding without materializing artifacts"},
    {6, "v791", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-template-digest-binding-criterion", "v790",
     "fixtures/release/shard-readiness-v790.json",
     "requires template digest binding without generating draft text"},
    {7, "v792", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-review-digest-binding-criterion", "v791",
     "fixtures/release/shard-readiness-v791.json",
     "requires review digest binding without writing approval statements"},
    {8, "v793", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-detached-signature-envelope-criterion", "v792",
     "fixtures/release/shard-readiness-v792.json",
     "reviews detached signature envelope metadata while payload bytes stay absent"},
    {9, "v794", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-signature-algorithm-criterion", "v793",
     "fixtures/release/shard-readiness-v793.json",
     "reviews signature algorithm metadata without accepting signatures"},
    {10, "v795", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-signature-redaction-criterion", "v794",
     "fixtures/release/shard-readiness-v794.json",
     "requires signature redaction policy and keeps raw signature material blocked"},
    {11, "v796", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-source-release-handle-criterion", "v795",
     "fixtures/release/shard-readiness-v795.json",
     "reviews source release handles from frozen evidence only"},
    {12, "v797", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-source-file-handle-criterion", "v796",
     "fixtures/release/shard-readiness-v796.json",
     "reviews source file handles without walking the filesystem"},
    {13, "v798", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-source-snippet-handle-criterion", "v797",
     "fixtures/release/shard-readiness-v797.json",
     "reviews source snippet handles without assembling draft text"},
    {14, "v799", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-operator-identity-handle-criterion", "v798",
     "fixtures/release/shard-readiness-v798.json",
     "reviews operator identity handles without credential values"},
    {15, "v800", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-operator-role-handle-criterion", "v799",
     "fixtures/release/shard-readiness-v799.json",
     "reviews operator role handles without granting authority"},
    {16, "v801", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-redacted-value-digest-criterion", "v800",
     "fixtures/release/shard-readiness-v800.json",
     "reviews redacted value digests while raw values remain absent"},
    {17, "v802", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-value-shape-criterion", "v801",
     "fixtures/release/shard-readiness-v801.json",
     "reviews value shape metadata without accepting supplied values"},
    {18, "v803", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-redaction-policy-criterion", "v802",
     "fixtures/release/shard-readiness-v802.json",
     "reviews redaction policy without secret storage"},
    {19, "v804", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-provenance-policy-criterion", "v803",
     "fixtures/release/shard-readiness-v803.json",
     "reviews provenance policy without mutable provenance"},
    {20, "v805", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-separate-reviewer-control", "v804",
     "fixtures/release/shard-readiness-v804.json",
     "requires separate reviewer evidence while capture remains closed"},
    {21, "v806", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-unreviewable-package-rejection-control", "v805",
     "fixtures/release/shard-readiness-v805.json",
     "rejects unreviewable packages before parsing or acceptance"},
    {22, "v807", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-runtime-payload-rejection-control", "v806",
     "fixtures/release/shard-readiness-v806.json",
     "rejects runtime payloads and keeps live reads disabled"},
    {23, "v808", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-write-route-rejection-control", "v807",
     "fixtures/release/shard-readiness-v807.json",
     "rejects write routes and keeps router activation disabled"},
    {24, "v809", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-sibling-non-mutation-control", "v808",
     "fixtures/release/shard-readiness-v808.json",
     "proves Node Java and sibling mini-kv state are not mutated"},
    {25, "v810", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-closeout", "v809",
     "fixtures/release/shard-readiness-v809.json",
     "closes draft text package review preflight without accepting a draft text package"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_draft_text_package_review_preflight_stages() {
    return {draft_text_package_review_preflight_stages.data(), draft_text_package_review_preflight_stages.size()};
}

int first_signed_approval_capture_artifact_draft_text_package_review_preflight_release_number() {
    return first_draft_text_package_review_preflight_release_number;
}

int planned_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_count() {
    return static_cast<int>(draft_text_package_review_preflight_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages