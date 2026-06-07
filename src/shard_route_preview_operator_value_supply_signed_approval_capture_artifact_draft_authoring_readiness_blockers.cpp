#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers {
namespace {

struct AuthoringBlocker {
    std::string_view code;
    std::string_view blocks;
    std::string_view reason;
};

constexpr std::array<AuthoringBlocker, 25> authoring_blockers = {{
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST_BLOCKER", "draft_text",
     "request manifest is evidence only and cannot mint instructions"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_ARTIFACT_PREFLIGHT_DIGEST_BLOCKER", "draft_text",
     "artifact preflight digest is pinned but does not authorize draft creation"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_TEMPLATE_DIGEST_BLOCKER", "draft_text",
     "template digest is reference material and not signed approval wording"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REVIEW_DIGEST_BLOCKER", "draft_text",
     "review digest cannot be expanded into approval statement text"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_OPERATOR_IDENTITY_BLOCKER", "identity_value",
     "operator identity value is not stored by mini-kv"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_OPERATOR_ROLE_BLOCKER", "approval_capture",
     "operator role does not grant capture authority"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_WINDOW_ID_BLOCKER", "capture_window",
     "capture window remains closed and no services are started"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CHANNEL_POLICY_BLOCKER", "raw_endpoint",
     "raw channel and endpoint values are blocked"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIGNATURE_ALGORITHM_BLOCKER",
     "signature_material", "algorithm metadata does not accept signature bytes"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_DETACHED_SIGNATURE_BLOCKER",
     "signature_material", "detached signature payload is external and absent"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIGNATURE_REDACTION_BLOCKER",
     "raw_signature", "raw signature material stays redacted"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_APPROVAL_STATEMENT_DIGEST_BLOCKER",
     "draft_text", "approval statement text is not generated from the digest"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_VERSION_BLOCKER", "runtime_lookup",
     "source version is named without runtime evidence lookup"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_FILE_BLOCKER", "filesystem_read",
     "source file id is named without reading the file"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SOURCE_SNIPPET_BLOCKER", "snippet_copy",
     "source snippet id is named without copying snippet text"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REDACTED_VALUE_DIGEST_BLOCKER",
     "raw_value", "redacted digest does not permit raw value material"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_VALUE_SHAPE_BLOCKER", "operator_value",
     "value shape metadata does not accept a supplied value"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REDACTION_POLICY_BLOCKER", "raw_secret",
     "redaction policy prevents raw secret leakage"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_PROVENANCE_POLICY_BLOCKER",
     "evidence_import", "provenance policy cites evidence without importing it"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_RAW_SECRET_EMBARGO_BLOCKER", "raw_secret",
     "raw secret values are embargoed"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_APPROVAL_GRANT_EMBARGO_BLOCKER",
     "approval_grant", "approval grant remains absent"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_ZERO_VALUE_IMPORT_EMBARGO_BLOCKER",
     "value_import", "operator value import is blocked"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_WRITE_ROUTE_EMBARGO_BLOCKER", "write_route",
     "write routing remains disabled"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_SIBLING_NON_MUTATION_BLOCKER",
     "sibling_mutation", "sibling state mutation is blocked"},
    {"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CLOSEOUT_BLOCKER", "execution",
     "closeout requires a separate package before any real signed draft path"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_blockers_json(
    int published_stage_count) {
    const int limit =
        std::clamp(published_stage_count, 0,
                   planned_signed_approval_capture_artifact_draft_authoring_readiness_blocker_count());
    std::string json = "[";
    for (int index = 0; index < limit; ++index) {
        const auto& blocker = authoring_blockers[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"blockerCode\":" + json_string(blocker.code) +
                ",\"blocks\":" + json_string(blocker.blocks) +
                ",\"reason\":" + json_string(blocker.reason) +
                ",\"runtimeExecutionBlocked\":true"
                ",\"writeRouteBlocked\":true"
                ",\"siblingMutationBlocked\":true}";
    }
    json += "]";
    return json;
}

int planned_signed_approval_capture_artifact_draft_authoring_readiness_blocker_count() {
    return static_cast<int>(authoring_blockers.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers
