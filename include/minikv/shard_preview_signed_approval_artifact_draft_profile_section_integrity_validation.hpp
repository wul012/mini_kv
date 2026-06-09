#pragma once

#include <string>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation {

std::string format_signed_approval_artifact_draft_profile_section_integrity_validation_json(
    int source_profile_section_stage_count,
    bool source_profile_section_complete,
    int signed_approval_draft_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_signed_approval_material,
    bool signed_approval_created,
    bool material_intake_opened,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation
