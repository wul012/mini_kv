#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements {
namespace {

struct AuthoringRequirement {
    std::string_view code;
    std::string_view source;
    std::string_view meaning;
};

constexpr std::array<AuthoringRequirement, 25> authoring_requirements = {{
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST", "artifact_preflight_request_id",
     "request manifest must be reviewed before any signed draft wording exists"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_ARTIFACT_PREFLIGHT_DIGEST",
     "artifact_preflight_digest", "artifact preflight closeout digest must be pinned"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_TEMPLATE_DIGEST", "signed_approval_template_digest",
     "signed approval template digest must be cited before draft authoring"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REVIEW_DIGEST", "approval_review_digest",
     "approval review digest must be present as source evidence"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_OPERATOR_IDENTITY", "operator_identity",
     "operator identity is required but the identity value is not stored here"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_OPERATOR_ROLE", "operator_role",
     "operator role is required but cannot authorize capture"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_WINDOW_ID", "capture_window_id",
     "capture window id must be named while the window remains closed"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CHANNEL_POLICY", "channel_policy",
     "channel policy must be reviewed without raw endpoint or channel values"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIGNATURE_ALGORITHM", "signature_algorithm",
     "signature algorithm policy must be known without signature bytes"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_DETACHED_SIGNATURE", "detached_signature",
     "detached signature handling must remain external to this artifact"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIGNATURE_REDACTION", "signature_redaction",
     "signature redaction policy must block raw signature disclosure"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_APPROVAL_STATEMENT_DIGEST",
     "approval_statement_digest", "approval statement digest is required without statement text"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_VERSION", "source_version",
     "source evidence version must be visible for human citation"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_FILE", "source_file",
     "source evidence file id must be visible without runtime lookup"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_SNIPPET", "source_snippet",
     "source evidence snippet id must be visible without copying snippet text"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REDACTED_VALUE_DIGEST", "redacted_value_digest",
     "redacted value digest must be pinned and raw value material must stay absent"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_VALUE_SHAPE", "value_shape",
     "value shape metadata may be described but no operator value is accepted"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REDACTION_POLICY", "redaction_policy",
     "redaction policy must prevent raw secret leakage"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_PROVENANCE_POLICY", "provenance_policy",
     "provenance policy must cite immutable evidence only"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_RAW_SECRET_EMBARGO", "raw_secret_embargo",
     "raw secret values are never carried by readiness evidence"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_APPROVAL_GRANT_EMBARGO", "approval_grant_embargo",
     "approval grant must remain absent from authoring readiness"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_ZERO_VALUE_IMPORT_EMBARGO",
     "zero_value_import_embargo", "operator value import count must remain zero"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_WRITE_ROUTE_EMBARGO", "write_route_embargo",
     "write route must remain disabled until a separate approved path exists"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIBLING_NON_MUTATION", "sibling_non_mutation",
     "sibling project state must not be changed by readiness evidence"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CLOSEOUT", "authoring_readiness_closeout",
     "closeout stops before any real signed draft artifact package"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_requirements_json(
    int published_stage_count) {
    const int limit =
        std::clamp(published_stage_count, 0,
                   planned_signed_approval_capture_artifact_draft_authoring_readiness_requirement_count());
    std::string json = "[";
    for (int index = 0; index < limit; ++index) {
        const auto& requirement = authoring_requirements[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"requirementCode\":" + json_string(requirement.code) +
                ",\"sourceRequirement\":" + json_string(requirement.source) +
                ",\"meaning\":" + json_string(requirement.meaning) +
                ",\"materializesDraftText\":false"
                ",\"acceptsSignatureMaterial\":false"
                ",\"importsOperatorValue\":false}";
    }
    json += "]";
    return json;
}

int planned_signed_approval_capture_artifact_draft_authoring_readiness_requirement_count() {
    return static_cast<int>(authoring_requirements.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements
