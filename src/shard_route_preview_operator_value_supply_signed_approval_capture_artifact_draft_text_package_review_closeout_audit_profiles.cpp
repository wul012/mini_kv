#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles {
namespace {

constexpr std::array<DraftTextPackageReviewCloseoutAuditProfile, 25> audit_profiles = {{
    {1, "draft_text_package_review_closeout_audit_source_freeze_profile", "source-fixture-only", "review v810 frozen fixture evidence only"},
    {2, "draft_text_package_review_closeout_audit_release_range_profile", "release-range-only", "review v786-v810 range evidence only"},
    {3, "draft_text_package_review_closeout_audit_stage_sequence_profile", "stage-chain-only", "review contiguous stage evidence only"},
    {4, "draft_text_package_review_closeout_audit_criterion_slot_count_profile", "catalog-count-only", "review criterion count evidence only"},
    {5, "draft_text_package_review_closeout_audit_rejection_control_count_profile", "catalog-count-only", "review rejection control count evidence only"},
    {6, "draft_text_package_review_closeout_audit_profile_count_profile", "catalog-count-only", "review profile count evidence only"},
    {7, "draft_text_package_review_closeout_audit_validation_pass_profile", "validation-only", "review validation evidence only"},
    {8, "draft_text_package_review_closeout_audit_digest_marker_profile", "digest-only", "review digest marker evidence only"},
    {9, "draft_text_package_review_closeout_audit_source_fixture_path_profile", "fixture-path-only", "review source fixture path evidence only"},
    {10, "draft_text_package_review_closeout_audit_draft_text_absence_profile", "absence-only", "review absence of draft package material"},
    {11, "draft_text_package_review_closeout_audit_parse_rejection_profile", "parse-rejection-only", "review parser lock evidence only"},
    {12, "draft_text_package_review_closeout_audit_acceptance_rejection_profile", "acceptance-rejection-only", "review acceptance lock evidence only"},
    {13, "draft_text_package_review_closeout_audit_approval_grant_rejection_profile", "grant-rejection-only", "review no-grant evidence only"},
    {14, "draft_text_package_review_closeout_audit_operator_value_zero_count_profile", "zero-value-only", "review zero value count evidence only"},
    {15, "draft_text_package_review_closeout_audit_sibling_import_rejection_profile", "sibling-import-rejection-only", "review sibling import lock evidence only"},
    {16, "draft_text_package_review_closeout_audit_runtime_payload_rejection_profile", "runtime-payload-rejection-only", "review runtime payload lock evidence only"},
    {17, "draft_text_package_review_closeout_audit_active_router_rejection_profile", "router-rejection-only", "review router lock evidence only"},
    {18, "draft_text_package_review_closeout_audit_write_route_rejection_profile", "write-route-rejection-only", "review write route lock evidence only"},
    {19, "draft_text_package_review_closeout_audit_wal_touch_rejection_profile", "wal-rejection-only", "review WAL lock evidence only"},
    {20, "draft_text_package_review_closeout_audit_load_restore_compact_rejection_profile", "storage-admin-rejection-only", "review storage admin lock evidence only"},
    {21, "draft_text_package_review_closeout_audit_command_catalog_read_only_profile", "command-catalog-only", "review command catalog metadata only"},
    {22, "draft_text_package_review_closeout_audit_shardjson_embedding_profile", "shardjson-embedding-only", "review SHARDJSON embedding evidence only"},
    {23, "draft_text_package_review_closeout_audit_versioned_fixture_catalog_profile", "fixture-catalog-only", "review versioned fixture catalog evidence only"},
    {24, "draft_text_package_review_closeout_audit_archive_explanation_presence_profile", "archive-explanation-only", "review archive explanation evidence only"},
    {25, "draft_text_package_review_closeout_audit_summary_profile", "closeout-only", "review final audit summary only"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_profiles.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewCloseoutAuditProfile>
signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles() {
    return {audit_profiles.data(), audit_profiles.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profile_count() {
    return static_cast<int>(audit_profiles.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& profile = audit_profiles[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(profile.sequence) +
                  ",\"profileCode\":" + json_string(profile.profile_code) +
                  ",\"boundary\":" + json_string(profile.boundary) +
                  ",\"reviewPrompt\":" + json_string(profile.review_prompt) +
                  ",\"readOnly\":true"
                  ",\"draftTextPackageAcceptanceAllowed\":false"
                  ",\"runtimePayloadAllowed\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"executionAllowed\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles
