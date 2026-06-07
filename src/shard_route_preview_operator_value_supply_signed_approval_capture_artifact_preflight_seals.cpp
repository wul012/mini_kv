#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals {
namespace {

struct ArtifactPreflightSeal {
    int sequence;
    std::string_view seal_code;
    std::string_view source_fragment_code;
    std::string_view rejection_rule;
    std::string_view digest_boundary;
};

constexpr std::array<ArtifactPreflightSeal, 25> artifact_preflight_seals = {{
    {1, "ARTIFACT_PREFLIGHT_REQUEST_ID_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REQUEST_ID",
     "reject missing artifact preflight request id", "request id is opaque and not minted"},
    {2, "ARTIFACT_PREFLIGHT_CAPTURE_PREFLIGHT_DIGEST_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CAPTURE_PREFLIGHT_DIGEST",
     "reject artifact preflight detached from v710 capture preflight digest", "source digest is frozen"},
    {3, "ARTIFACT_PREFLIGHT_TEMPLATE_DIGEST_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_TEMPLATE_DIGEST",
     "reject artifact preflight detached from v685 signed approval template digest", "template digest is frozen"},
    {4, "ARTIFACT_PREFLIGHT_REVIEW_DIGEST_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REVIEW_DIGEST",
     "reject missing approval packet review digest", "review digest placeholder remains absent"},
    {5, "ARTIFACT_PREFLIGHT_OPERATOR_IDENTITY_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_OPERATOR_IDENTITY",
     "reject missing operator identity fragment", "identity is not stored"},
    {6, "ARTIFACT_PREFLIGHT_OPERATOR_ROLE_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_OPERATOR_ROLE",
     "reject missing operator role fragment", "role is not stored"},
    {7, "ARTIFACT_PREFLIGHT_WINDOW_ID_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_WINDOW_ID",
     "reject missing capture window id fragment", "window is not opened"},
    {8, "ARTIFACT_PREFLIGHT_CHANNEL_POLICY_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CHANNEL_POLICY",
     "reject missing capture channel policy fragment", "raw channel value is absent"},
    {9, "ARTIFACT_PREFLIGHT_SIGNATURE_ALGORITHM_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIGNATURE_ALGORITHM",
     "reject missing signature algorithm fragment", "algorithm is metadata only"},
    {10, "ARTIFACT_PREFLIGHT_DETACHED_SIGNATURE_PLACEHOLDER_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_DETACHED_SIGNATURE_PLACEHOLDER",
     "reject concrete detached signature material", "signature bytes are never digested"},
    {11, "ARTIFACT_PREFLIGHT_SIGNATURE_REDACTION_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIGNATURE_REDACTION",
     "reject unredacted signature material", "signature redaction boundary is closed"},
    {12, "ARTIFACT_PREFLIGHT_APPROVAL_STATEMENT_DIGEST_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_APPROVAL_STATEMENT_DIGEST",
     "reject fabricated approval statement digest", "statement digest placeholder is absent"},
    {13, "ARTIFACT_PREFLIGHT_SOURCE_VERSION_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_VERSION",
     "reject rolling source evidence version", "source version is pinned"},
    {14, "ARTIFACT_PREFLIGHT_SOURCE_FILE_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_FILE",
     "reject runtime source file lookup", "source file id is a review fragment only"},
    {15, "ARTIFACT_PREFLIGHT_SOURCE_SNIPPET_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_SNIPPET",
     "reject runtime source snippet lookup", "source snippet id is a review fragment only"},
    {16, "ARTIFACT_PREFLIGHT_REDACTED_VALUE_DIGEST_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REDACTED_VALUE_DIGEST",
     "reject raw value material", "redacted digest boundary excludes value bytes"},
    {17, "ARTIFACT_PREFLIGHT_VALUE_SHAPE_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_VALUE_SHAPE",
     "reject operator value payload", "shape is declared without payload"},
    {18, "ARTIFACT_PREFLIGHT_REDACTION_POLICY_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REDACTION_POLICY",
     "reject unredacted evidence", "redaction policy is required"},
    {19, "ARTIFACT_PREFLIGHT_PROVENANCE_POLICY_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_PROVENANCE_POLICY",
     "reject provenance-free artifact preflight", "provenance remains review-only"},
    {20, "ARTIFACT_PREFLIGHT_NO_RAW_SECRET_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_RAW_SECRET_LOCK",
     "reject raw secret, credential, endpoint, or signature material", "raw material digest is forbidden"},
    {21, "ARTIFACT_PREFLIGHT_NO_GRANT_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_GRANT_LOCK",
     "reject approval grant emission", "grant state is absent"},
    {22, "ARTIFACT_PREFLIGHT_ZERO_VALUE_IMPORT_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_ZERO_VALUE_IMPORT_LOCK",
     "reject any imported operator value", "value import count stays zero"},
    {23, "ARTIFACT_PREFLIGHT_NO_WRITE_ROUTE_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_WRITE_ROUTE_LOCK",
     "reject write route activation", "router and write route stay disabled"},
    {24, "ARTIFACT_PREFLIGHT_SIBLING_NON_MUTATION_SEAL",
     "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIBLING_NON_MUTATION_LOCK",
     "reject sibling project mutation", "sibling evidence is not edited or executed"},
    {25, "ARTIFACT_PREFLIGHT_CLOSEOUT_SEAL", "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CLOSEOUT",
     "reject treating closeout as signed artifact draft", "closeout is preflight-only"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_artifact_preflight_seal_count() {
    return static_cast<int>(artifact_preflight_seals.size());
}

std::string format_signed_approval_capture_artifact_preflight_seals_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_artifact_preflight_seal_count());

    std::string response = "[";
    bool first = true;
    for (const auto& seal : artifact_preflight_seals) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(seal.sequence) +
                    ",\"sealCode\":" + json_string(seal.seal_code) +
                    ",\"sourceFragmentCode\":" + json_string(seal.source_fragment_code) +
                    ",\"rejectionRule\":" + json_string(seal.rejection_rule) +
                    ",\"digestBoundary\":" + json_string(seal.digest_boundary) +
                    ",\"publishedByCurrentStage\":" + json_bool(seal.sequence <= clamped_published_count) +
                    ",\"sealSatisfiedByAbsence\":true"
                    ",\"sealMaterialized\":false"
                    ",\"artifactMaterializationAllowed\":false"
                    ",\"approvalGrantAllowed\":false"
                    ",\"valueImportAllowed\":false"
                    ",\"writeRouteAllowed\":false"
                    ",\"siblingMutationAllowed\":false"
                    ",\"readOnly\":true"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals
