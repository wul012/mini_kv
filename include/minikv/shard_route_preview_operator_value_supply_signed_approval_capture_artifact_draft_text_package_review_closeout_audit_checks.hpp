#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks {

struct DraftTextPackageReviewCloseoutAuditCheck {
    int sequence;
    std::string_view check_code;
    std::string_view audited_area;
    std::string_view expected_evidence;
};

std::span<const DraftTextPackageReviewCloseoutAuditCheck>
signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks();
int planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_check_count();
std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks_json(
    int published_stage_count);

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks
