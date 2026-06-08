#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages;
    const auto records = stages::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages();
    assert(stages::first_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_release_number() == 861);
    assert(stages::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count() == 20);
    assert(records.size() == 20);
    assert(records.front().release_version == std::string{"v861"});
    assert(records.front().source_frozen_release_version == std::string{"v860"});
    assert(records.back().release_version == std::string{"v880"});
    assert(records.back().stage == std::string{"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-archive-closeout-final-guard-summary"});
    const auto report = minikv::shard_route_preview_stage_chain::inspect_stage_chain(
        records,
        2,
        stages::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_release_number());
    assert(report.published_stage_count == 2);
    assert(report.sequences_contiguous);
    assert(report.release_versions_contiguous);
    assert(report.source_frozen_release_versions_contiguous);
    assert(report.source_frozen_fixture_paths_contiguous);
    assert(!report.chain_complete);
}
