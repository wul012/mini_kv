#pragma once

#include <string>

namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation {

std::string format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
    int source_profile_integrity_stage_count,
    bool source_profile_integrity_complete,
    int source_review_preflight_stage_count,
    bool source_review_preflight_complete,
    int source_review_closeout_audit_stage_count,
    bool source_review_closeout_audit_complete,
    int source_comparison_closeout_audit_stage_count,
    bool source_comparison_closeout_audit_complete,
    int source_compared_package_intake_audit_stage_count,
    bool source_compared_package_intake_audit_complete,
    int text_package_profile_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_text_package_material,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation
