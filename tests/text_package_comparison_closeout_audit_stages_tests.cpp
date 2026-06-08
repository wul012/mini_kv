#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages;

    const auto records = stages::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages();
    assert(records.size() == 25);
    assert(stages::first_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_release_number() == 836);
    assert(stages::planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stage_count() == 25);
    assert(records.front().sequence == 1);
    assert(records.front().release_version == std::string{"v836"});
    assert(records.front().stage == std::string{"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-source-freeze"});
    assert(records.front().source_frozen_release_version == std::string{"v835"});
    assert(records.back().sequence == 25);
    assert(records.back().release_version == std::string{"v860"});
    assert(records.back().source_frozen_release_version == std::string{"v859"});

    const auto report = minikv::shard_route_preview_stage_chain::inspect_stage_chain(
        records,
        12,
        stages::planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_release_number());
    assert(report.published_stage_count == 12);
    assert(report.sequences_contiguous);
    assert(report.release_versions_contiguous);
    assert(report.source_frozen_release_versions_contiguous);
    assert(report.source_frozen_fixture_paths_contiguous);
    assert(!report.chain_complete);
}
