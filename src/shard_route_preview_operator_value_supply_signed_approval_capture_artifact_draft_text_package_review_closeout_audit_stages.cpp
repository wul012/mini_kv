#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_draft_text_package_review_closeout_audit_release_number = 811;

constexpr std::array<StageRecord, 25> draft_text_package_review_closeout_audit_stages = {{
    {1, "v811", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-source-freeze", "v810",
     "fixtures/release/shard-readiness-v810.json",
     "freezes the v810 review preflight closeout before auditing it"},
    {2, "v812", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-release-range", "v811",
     "fixtures/release/shard-readiness-v811.json",
     "audits the v786-v810 release range without reopening package review"},
    {3, "v813", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-stage-sequence", "v812",
     "fixtures/release/shard-readiness-v812.json",
     "audits contiguous stage sequence numbering for the review preflight"},
    {4, "v814", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-criterion-slot-count", "v813",
     "fixtures/release/shard-readiness-v813.json",
     "audits that all review criteria remain cataloged"},
    {5, "v815", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-rejection-control-count", "v814",
     "fixtures/release/shard-readiness-v814.json",
     "audits that all rejection controls remain cataloged"},
    {6, "v816", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-profile-count", "v815",
     "fixtures/release/shard-readiness-v815.json",
     "audits that all review profiles remain cataloged"},
    {7, "v817", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-validation-pass", "v816",
     "fixtures/release/shard-readiness-v816.json",
     "audits validation pass evidence without accepting package material"},
    {8, "v818", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-digest-marker", "v817",
     "fixtures/release/shard-readiness-v817.json",
     "audits the source digest marker for the closed preflight chain"},
    {9, "v819", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-source-fixture-path", "v818",
     "fixtures/release/shard-readiness-v818.json",
     "audits the frozen fixture path without walking runtime archives"},
    {10, "v820", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-draft-text-absence", "v819",
      "fixtures/release/shard-readiness-v819.json",
      "audits that draft text package material stays absent"},
    {11, "v821", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-parse-rejection", "v820",
      "fixtures/release/shard-readiness-v820.json",
      "audits that package parsing stays rejected"},
    {12, "v822", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-acceptance-rejection", "v821",
      "fixtures/release/shard-readiness-v821.json",
      "audits that package acceptance stays rejected"},
    {13, "v823", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-approval-grant-rejection", "v822",
      "fixtures/release/shard-readiness-v822.json",
      "audits that approval grants stay absent"},
    {14, "v824", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-operator-value-zero-count", "v823",
      "fixtures/release/shard-readiness-v823.json",
      "audits that submitted accepted imported and persisted operator value counts stay zero"},
    {15, "v825", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-sibling-import-rejection", "v824",
      "fixtures/release/shard-readiness-v824.json",
      "audits that automatic sibling imports remain rejected"},
    {16, "v826", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-runtime-payload-rejection", "v825",
      "fixtures/release/shard-readiness-v825.json",
      "audits that runtime payload acceptance remains rejected"},
    {17, "v827", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-active-router-rejection", "v826",
      "fixtures/release/shard-readiness-v826.json",
      "audits that no active router is installed by review evidence"},
    {18, "v828", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-write-route-rejection", "v827",
      "fixtures/release/shard-readiness-v827.json",
      "audits that write routing remains rejected"},
    {19, "v829", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-wal-touch-rejection", "v828",
      "fixtures/release/shard-readiness-v828.json",
      "audits that WAL touch remains rejected"},
    {20, "v830", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-load-restore-compact-rejection", "v829",
      "fixtures/release/shard-readiness-v829.json",
      "audits that LOAD RESTORE and COMPACT stay rejected"},
    {21, "v831", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-command-catalog-read-only", "v830",
      "fixtures/release/shard-readiness-v830.json",
      "audits command catalog read-only metadata"},
    {22, "v832", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-shardjson-embedding", "v831",
      "fixtures/release/shard-readiness-v831.json",
      "audits SHARDJSON embedding for the closeout audit section"},
    {23, "v833", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-versioned-fixture-catalog", "v832",
      "fixtures/release/shard-readiness-v832.json",
      "audits versioned fixture catalog continuity for the audit chain"},
    {24, "v834", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-archive-explanation-presence", "v833",
      "fixtures/release/shard-readiness-v833.json",
      "audits command screenshot and explanation archive presence"},
    {25, "v835", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-summary", "v834",
      "fixtures/release/shard-readiness-v834.json",
      "closes the review preflight closeout audit without accepting package material"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages() {
    return {draft_text_package_review_closeout_audit_stages.data(),
            draft_text_package_review_closeout_audit_stages.size()};
}

int first_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_release_number() {
    return first_draft_text_package_review_closeout_audit_release_number;
}

int planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stage_count() {
    return static_cast<int>(draft_text_package_review_closeout_audit_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages
