#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages.hpp"

#include <array>

namespace minikv::
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_compared_package_evidence_intake_audit_release_number = 861;

constexpr std::array<StageRecord, 20> compared_package_evidence_intake_audit_stages = {{
    {1, "v861",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "source-acceptance-precheck-slot",
     "v860", "fixtures/release/shard-readiness-v860.json",
     "declares source acceptance precheck evidence slot without consuming package payload"},
    {2, "v862",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "source-acceptance-precheck-guard",
     "v861", "fixtures/release/shard-readiness-v861.json",
     "guards source acceptance precheck evidence from becoming package acceptance"},
    {3, "v863",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "manual-submission-reference-slot",
     "v862", "fixtures/release/shard-readiness-v862.json",
     "declares manual submission reference evidence slot without creating submission material"},
    {4, "v864",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "manual-submission-reference-guard",
     "v863", "fixtures/release/shard-readiness-v863.json",
     "guards manual submission references from raw endpoint or credential values"},
    {5, "v865",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "offline-comparison-result-slot",
     "v864", "fixtures/release/shard-readiness-v864.json",
     "declares offline comparison result evidence slot without comparing content"},
    {6, "v866",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "offline-comparison-result-guard",
     "v865", "fixtures/release/shard-readiness-v865.json",
     "guards offline comparison result from accepting synthetic evidence"},
    {7, "v867",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "identity-binding-slot",
     "v866", "fixtures/release/shard-readiness-v866.json",
     "declares identity binding evidence slot without resolving identity secrets"},
    {8, "v868",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "identity-binding-guard",
     "v867", "fixtures/release/shard-readiness-v867.json",
     "guards identity binding evidence from privilege or approval escalation"},
    {9, "v869",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "digest-match-summary-slot",
     "v868", "fixtures/release/shard-readiness-v868.json",
     "declares digest match summary evidence slot without recalculating payload material"},
    {10, "v870",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "digest-match-summary-guard",
     "v869", "fixtures/release/shard-readiness-v869.json",
     "guards digest summaries from being treated as signed approval"},
    {11, "v871",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "detached-signature-envelope-observation-slot",
     "v870", "fixtures/release/shard-readiness-v870.json",
     "declares detached signature envelope observation slot without parsing signature payload"},
    {12, "v872",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "detached-signature-envelope-observation-guard",
     "v871", "fixtures/release/shard-readiness-v871.json",
     "guards detached signature observation from payload parse or verification execution"},
    {13, "v873",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "source-and-operator-handle-slot",
     "v872", "fixtures/release/shard-readiness-v872.json",
     "declares source evidence and operator value handle slots without importing values"},
    {14, "v874",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "source-and-operator-handle-guard",
     "v873", "fixtures/release/shard-readiness-v873.json",
     "guards source and operator handles from value import or persistence"},
    {15, "v875",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "policy-and-execution-lock-slot",
     "v874", "fixtures/release/shard-readiness-v874.json",
     "declares policy and execution lock evidence slot without opening execution"},
    {16, "v876",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "policy-and-execution-lock-guard",
     "v875", "fixtures/release/shard-readiness-v875.json",
     "guards policy and execution lock evidence from runtime enablement"},
    {17, "v877",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "approval-grant-separation-slot",
     "v876", "fixtures/release/shard-readiness-v876.json",
     "declares approval grant separation evidence slot without capturing grants"},
    {18, "v878",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "approval-grant-separation-guard",
     "v877", "fixtures/release/shard-readiness-v877.json", "guards approval grant separation from emitting approvals"},
    {19, "v879",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "archive-closeout-slot",
     "v878", "fixtures/release/shard-readiness-v878.json",
     "declares archive closeout evidence slot without walking runtime archives"},
    {20, "v880",
     "route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-"
     "archive-closeout-final-guard-summary",
     "v879", "fixtures/release/shard-readiness-v879.json",
     "closes compared package evidence intake audit without accepting package material"},
}};

} // namespace

std::span<const StageRecord>
signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages() {
    return {compared_package_evidence_intake_audit_stages.data(), compared_package_evidence_intake_audit_stages.size()};
}

int first_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_release_number() {
    return first_compared_package_evidence_intake_audit_release_number;
}

int planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count() {
    return static_cast<int>(compared_package_evidence_intake_audit_stages.size());
}

} // namespace
  // minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages