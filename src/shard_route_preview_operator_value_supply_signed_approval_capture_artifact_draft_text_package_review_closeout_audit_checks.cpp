#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks {
namespace {

constexpr std::array<DraftTextPackageReviewCloseoutAuditCheck, 25> audit_checks = {{
    {1, "source_freeze_audit", "v810 frozen source", "v810 fixture is frozen before closeout audit starts"},
    {2, "release_range_audit", "v786-v810 range", "review preflight release range is visible and closed"},
    {3, "stage_sequence_audit", "stage sequence", "review preflight stage sequence remains contiguous"},
    {4, "criterion_slot_count_audit", "review criteria", "twenty five criteria remain cataloged"},
    {5, "rejection_control_count_audit", "rejection controls", "twenty five rejection controls remain cataloged"},
    {6, "profile_count_audit", "review profiles", "twenty five review profiles remain cataloged"},
    {7, "validation_pass_audit", "validation", "review preflight validation is closed and true"},
    {8, "digest_marker_audit", "digest marker", "source digest marker remains available"},
    {9, "source_fixture_path_audit", "source fixture path", "frozen fixture path remains versioned"},
    {10, "draft_text_absence_audit", "draft text package material", "draft text package material remains absent"},
    {11, "parse_rejection_audit", "package parser", "package parsing remains rejected"},
    {12, "acceptance_rejection_audit", "package acceptance", "package acceptance remains rejected"},
    {13, "approval_grant_rejection_audit", "approval grant", "approval grant remains absent"},
    {14, "operator_value_zero_count_audit", "operator values", "operator value counts remain zero"},
    {15, "sibling_import_rejection_audit", "automatic sibling import", "automatic sibling import remains rejected"},
    {16, "runtime_payload_rejection_audit", "runtime payload", "runtime payload acceptance remains rejected"},
    {17, "active_router_rejection_audit", "active router", "active router remains absent"},
    {18, "write_route_rejection_audit", "write route", "write routing remains rejected"},
    {19, "wal_touch_rejection_audit", "WAL touch", "WAL touch remains rejected"},
    {20, "load_restore_compact_rejection_audit", "LOAD RESTORE COMPACT", "storage admin execution remains rejected"},
    {21, "command_catalog_read_only_audit", "command catalog", "command metadata remains read only"},
    {22, "shardjson_embedding_audit", "SHARDJSON embedding", "closeout audit is embedded in SHARDJSON"},
    {23, "versioned_fixture_catalog_audit", "versioned fixtures", "audit chain keeps versioned fixtures"},
    {24, "archive_explanation_presence_audit", "archive explanation", "command evidence archive stays explainable"},
    {25, "closeout_audit_summary", "closeout summary", "audit closes without accepting package material"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_checks.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewCloseoutAuditCheck>
signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks() {
    return {audit_checks.data(), audit_checks.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_check_count() {
    return static_cast<int>(audit_checks.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& check = audit_checks[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"auditedArea\":" + json_string(check.audited_area) +
                  ",\"expectedEvidence\":" + json_string(check.expected_evidence) +
                  ",\"auditCheckPrepared\":true"
                  ",\"readsRuntimeArchive\":false"
                  ",\"parsesDraftTextPackage\":false"
                  ",\"acceptsDraftTextPackage\":false"
                  ",\"mutatesSiblingState\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks
