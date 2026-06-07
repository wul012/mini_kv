#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles {
namespace {

struct AuthoringProfile {
    std::string_view profile_code;
    std::string_view boundary;
};

constexpr std::array<AuthoringProfile, 25> authoring_profiles = {{
    {"AUTHORING_READINESS_PROFILE_REQUEST_MANIFEST", "request-manifest-review-only"},
    {"AUTHORING_READINESS_PROFILE_ARTIFACT_PREFLIGHT_DIGEST", "artifact-preflight-digest-pin-only"},
    {"AUTHORING_READINESS_PROFILE_TEMPLATE_DIGEST", "template-digest-pin-only"},
    {"AUTHORING_READINESS_PROFILE_REVIEW_DIGEST", "review-digest-pin-only"},
    {"AUTHORING_READINESS_PROFILE_OPERATOR_IDENTITY", "operator-identity-handle-only"},
    {"AUTHORING_READINESS_PROFILE_OPERATOR_ROLE", "operator-role-review-only"},
    {"AUTHORING_READINESS_PROFILE_WINDOW_ID", "capture-window-closed"},
    {"AUTHORING_READINESS_PROFILE_CHANNEL_POLICY", "channel-policy-no-raw-endpoint"},
    {"AUTHORING_READINESS_PROFILE_SIGNATURE_ALGORITHM", "signature-algorithm-no-payload"},
    {"AUTHORING_READINESS_PROFILE_DETACHED_SIGNATURE", "detached-signature-external-only"},
    {"AUTHORING_READINESS_PROFILE_SIGNATURE_REDACTION", "raw-signature-redacted"},
    {"AUTHORING_READINESS_PROFILE_STATEMENT_DIGEST", "statement-digest-no-text"},
    {"AUTHORING_READINESS_PROFILE_SOURCE_VERSION", "source-version-no-live-read"},
    {"AUTHORING_READINESS_PROFILE_SOURCE_FILE", "source-file-no-filesystem-read"},
    {"AUTHORING_READINESS_PROFILE_SOURCE_SNIPPET", "source-snippet-no-copy"},
    {"AUTHORING_READINESS_PROFILE_REDACTED_VALUE_DIGEST", "redacted-value-digest-no-raw-value"},
    {"AUTHORING_READINESS_PROFILE_VALUE_SHAPE", "value-shape-no-operator-value"},
    {"AUTHORING_READINESS_PROFILE_REDACTION_POLICY", "redaction-policy-no-secret"},
    {"AUTHORING_READINESS_PROFILE_PROVENANCE_POLICY", "provenance-policy-no-import"},
    {"AUTHORING_READINESS_PROFILE_RAW_SECRET_EMBARGO", "raw-secret-embargo"},
    {"AUTHORING_READINESS_PROFILE_APPROVAL_GRANT_EMBARGO", "approval-grant-embargo"},
    {"AUTHORING_READINESS_PROFILE_ZERO_VALUE_IMPORT", "zero-value-import"},
    {"AUTHORING_READINESS_PROFILE_WRITE_ROUTE_EMBARGO", "write-route-embargo"},
    {"AUTHORING_READINESS_PROFILE_SIBLING_NON_MUTATION", "sibling-non-mutation"},
    {"AUTHORING_READINESS_PROFILE_CLOSEOUT", "closeout-before-real-draft-package"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_profiles_json(
    int published_stage_count) {
    const int limit =
        std::clamp(published_stage_count, 0,
                   planned_signed_approval_capture_artifact_draft_authoring_readiness_profile_count());
    std::string json = "[";
    for (int index = 0; index < limit; ++index) {
        const auto& profile = authoring_profiles[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"profileCode\":" + json_string(profile.profile_code) +
                ",\"boundary\":" + json_string(profile.boundary) +
                ",\"readOnly\":true"
                ",\"executionAllowed\":false"
                ",\"draftTextAllowed\":false"
                ",\"artifactMaterializationAllowed\":false}";
    }
    json += "]";
    return json;
}

int planned_signed_approval_capture_artifact_draft_authoring_readiness_profile_count() {
    return static_cast<int>(authoring_profiles.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles
