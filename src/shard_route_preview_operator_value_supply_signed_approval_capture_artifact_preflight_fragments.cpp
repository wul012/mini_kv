#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments {
namespace {

struct ArtifactPreflightFragment {
    int sequence;
    std::string_view fragment_code;
    std::string_view artifact_field;
    std::string_view fragment_role;
    std::string_view materialization_blocker;
};

constexpr std::array<ArtifactPreflightFragment, 25> artifact_preflight_fragments = {{
    {1, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REQUEST_ID", "artifact_preflight_request_id",
     "opaque request id for artifact preflight review", "request id is not minted by mini-kv"},
    {2, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CAPTURE_PREFLIGHT_DIGEST", "capture_preflight_digest",
     "source capture preflight digest binding", "digest is referenced, not recalculated from sibling runtime"},
    {3, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_TEMPLATE_DIGEST", "signed_approval_template_digest",
     "source signed approval template digest binding", "template digest is read-only source evidence"},
    {4, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REVIEW_DIGEST", "approval_packet_review_digest",
     "approval packet review digest placeholder", "review digest is not fabricated"},
    {5, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_OPERATOR_IDENTITY", "operator_identity_fragment",
     "operator identity artifact fragment", "operator identity value is not stored"},
    {6, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_OPERATOR_ROLE", "operator_role_fragment",
     "operator role artifact fragment", "operator role value is not stored"},
    {7, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_WINDOW_ID", "capture_window_id_fragment",
     "capture window id artifact fragment", "capture window is not opened"},
    {8, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CHANNEL_POLICY", "capture_channel_policy_fragment",
     "capture channel policy artifact fragment", "raw endpoint or channel value is not accepted"},
    {9, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIGNATURE_ALGORITHM", "signature_algorithm_fragment",
     "signature algorithm artifact fragment", "signature algorithm is metadata only"},
    {10, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_DETACHED_SIGNATURE_PLACEHOLDER",
     "detached_signature_placeholder", "detached signature placeholder", "signature bytes are not captured"},
    {11, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIGNATURE_REDACTION", "signature_redaction_policy",
     "signature material redaction policy", "raw signature material is rejected"},
    {12, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_APPROVAL_STATEMENT_DIGEST",
     "approval_statement_digest_placeholder", "approval statement digest placeholder",
     "approval statement text is not generated"},
    {13, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_VERSION", "source_evidence_version_fragment",
     "source evidence version fragment", "source version is pinned, not rolling"},
    {14, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_FILE", "source_evidence_file_fragment",
     "source evidence file fragment", "runtime file lookup is not performed"},
    {15, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SOURCE_SNIPPET", "source_evidence_snippet_fragment",
     "source evidence snippet fragment", "runtime snippet lookup is not performed"},
    {16, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REDACTED_VALUE_DIGEST", "redacted_value_digest_fragment",
     "redacted value digest fragment", "raw value material is not accepted"},
    {17, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_VALUE_SHAPE", "value_shape_fragment",
     "value shape fragment", "operator value payload is not accepted"},
    {18, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REDACTION_POLICY", "redaction_policy_fragment",
     "redaction policy fragment", "unredacted evidence is not stored"},
    {19, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_PROVENANCE_POLICY", "provenance_policy_fragment",
     "provenance policy fragment", "evidence import is not performed"},
    {20, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_RAW_SECRET_LOCK", "no_raw_secret_artifact_lock",
     "no raw secret artifact lock", "secret, credential, endpoint, and signature material are absent"},
    {21, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_GRANT_LOCK", "no_approval_grant_artifact_lock",
     "no approval grant artifact lock", "approval grant is not emitted"},
    {22, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_ZERO_VALUE_IMPORT_LOCK", "zero_value_import_artifact_lock",
     "zero value import artifact lock", "operator values are not imported"},
    {23, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_NO_WRITE_ROUTE_LOCK", "no_write_route_artifact_lock",
     "no write route artifact lock", "write route is not installed"},
    {24, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_SIBLING_NON_MUTATION_LOCK",
     "sibling_non_mutation_artifact_lock", "sibling non-mutation artifact lock",
     "Node, Java, and sibling evidence are not mutated"},
    {25, "SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CLOSEOUT", "artifact_preflight_closeout_boundary",
     "closeout boundary for future signed artifact draft readiness", "artifact draft remains separate"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_artifact_preflight_fragment_count() {
    return static_cast<int>(artifact_preflight_fragments.size());
}

std::string format_signed_approval_capture_artifact_preflight_fragments_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_artifact_preflight_fragment_count());

    std::string response = "[";
    bool first = true;
    for (const auto& fragment : artifact_preflight_fragments) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(fragment.sequence) +
                    ",\"fragmentCode\":" + json_string(fragment.fragment_code) +
                    ",\"artifactField\":" + json_string(fragment.artifact_field) +
                    ",\"fragmentRole\":" + json_string(fragment.fragment_role) +
                    ",\"materializationBlocker\":" + json_string(fragment.materialization_blocker) +
                    ",\"publishedByCurrentStage\":" + json_bool(fragment.sequence <= clamped_published_count) +
                    ",\"requiredBeforeSignedApprovalArtifactDraft\":true"
                    ",\"presentInMiniKv\":false"
                    ",\"materializedByMiniKv\":false"
                    ",\"signedApprovalArtifactDraftPresent\":false"
                    ",\"approvalGrantPresent\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValuesPersisted\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"readOnly\":true"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments
