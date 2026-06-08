#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_draft_text_package_comparison_closeout_audit_release_number = 836;

constexpr std::array<StageRecord, 25> draft_text_package_comparison_closeout_audit_stages = {{
    {1, "v836", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-source-freeze", "v835",
     "fixtures/release/shard-readiness-v835.json",
     "freezes the v835 review closeout audit before comparison closeout auditing"},
    {2, "v837", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-release-range", "v836",
     "fixtures/release/shard-readiness-v836.json",
     "audits the v1287-v1311 comparison preflight planning range without accepting packages"},
    {3, "v838", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-comparison-lane-catalog-sequence", "v837",
     "fixtures/release/shard-readiness-v837.json",
     "audits contiguous comparison lane sequence evidence"},
    {4, "v839", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-identity-lane", "v838",
     "fixtures/release/shard-readiness-v838.json",
     "audits identity comparison lane evidence without parsing submitted packages"},
    {5, "v840", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-request-correlation-lane", "v839",
     "fixtures/release/shard-readiness-v839.json",
     "audits request correlation comparison lane evidence"},
    {6, "v841", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-artifact-digest-binding-lane", "v840",
     "fixtures/release/shard-readiness-v840.json",
     "audits artifact digest binding comparison lane evidence"},
    {7, "v842", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-template-digest-binding-lane", "v841",
     "fixtures/release/shard-readiness-v841.json",
     "audits template digest binding comparison lane evidence"},
    {8, "v843", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-review-digest-recheck-control", "v842",
     "fixtures/release/shard-readiness-v842.json",
     "audits review digest recheck acceptance control"},
    {9, "v844", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-detached-signature-envelope-lane", "v843",
     "fixtures/release/shard-readiness-v843.json",
     "audits detached signature envelope comparison lane evidence"},
    {10, "v845", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-signature-metadata-lane", "v844",
     "fixtures/release/shard-readiness-v844.json",
     "audits signature metadata comparison lane evidence"},
    {11, "v846", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-signature-payload-parse-rejection", "v845",
     "fixtures/release/shard-readiness-v845.json",
     "audits detached signature payload parse rejection"},
    {12, "v847", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-source-evidence-handle-lane", "v846",
     "fixtures/release/shard-readiness-v846.json",
     "audits source evidence handle comparison lane evidence"},
    {13, "v848", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-source-snippet-digest-lane", "v847",
     "fixtures/release/shard-readiness-v847.json",
     "audits source snippet digest comparison lane evidence"},
    {14, "v849", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-operator-value-handle-lane", "v848",
     "fixtures/release/shard-readiness-v848.json",
     "audits operator value handle binding comparison lane evidence"},
    {15, "v850", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-operator-value-zero-count", "v849",
     "fixtures/release/shard-readiness-v849.json",
     "audits zero submitted accepted imported and persisted operator values"},
    {16, "v851", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-policy-assertion-lane", "v850",
     "fixtures/release/shard-readiness-v850.json",
     "audits policy assertion comparison lane evidence"},
    {17, "v852", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-review-state-control-lane", "v851",
     "fixtures/release/shard-readiness-v851.json",
     "audits review state comparison control evidence"},
    {18, "v853", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-uncompared-package-rejection", "v852",
     "fixtures/release/shard-readiness-v852.json",
     "audits uncompared package rejection control"},
    {19, "v854", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-unacceptable-material-rejection", "v853",
     "fixtures/release/shard-readiness-v853.json",
     "audits unacceptable material rejection control"},
    {20, "v855", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-acceptance-grant-rejection", "v854",
     "fixtures/release/shard-readiness-v854.json",
     "audits package acceptance and approval grant rejection"},
    {21, "v856", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-execution-lock-control", "v855",
     "fixtures/release/shard-readiness-v855.json",
     "audits execution lock acceptance control"},
    {22, "v857", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-runtime-payload-rejection", "v856",
     "fixtures/release/shard-readiness-v856.json",
     "audits runtime payload rejection control"},
    {23, "v858", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-write-route-rejection", "v857",
     "fixtures/release/shard-readiness-v857.json",
     "audits write route rejection control"},
    {24, "v859", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-sibling-mutation-rejection", "v858",
     "fixtures/release/shard-readiness-v858.json",
     "audits sibling mutation rejection control"},
    {25, "v860", "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-summary", "v859",
     "fixtures/release/shard-readiness-v859.json",
     "closes the comparison preflight closeout audit without accepting package material"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages() {
    return {draft_text_package_comparison_closeout_audit_stages.data(),
            draft_text_package_comparison_closeout_audit_stages.size()};
}

int first_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_release_number() {
    return first_draft_text_package_comparison_closeout_audit_release_number;
}

int planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stage_count() {
    return static_cast<int>(draft_text_package_comparison_closeout_audit_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages
