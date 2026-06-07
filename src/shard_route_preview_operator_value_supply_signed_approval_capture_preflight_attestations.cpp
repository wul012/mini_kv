#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations {
namespace {

struct CapturePreflightAttestation {
    int sequence;
    std::string_view attestation_code;
    std::string_view required_review;
    std::string_view evidence_scope;
    std::string_view sibling_boundary;
};

constexpr std::array<CapturePreflightAttestation, 25> capture_preflight_attestations = {{
    {1,
     "CAPTURE_PREFLIGHT_ATTEST_REQUEST_ID_INPUT",
     "confirm capture preflight request id is required and not minted",
     "request metadata only",
     "no sibling request is created"},
    {2,
     "CAPTURE_PREFLIGHT_ATTEST_TEMPLATE_DIGEST_BINDING",
     "confirm v685 signed approval template digest marker is pinned",
     "source template digest reference",
     "no sibling fixture is rewritten"},
    {3,
     "CAPTURE_PREFLIGHT_ATTEST_REVIEW_DIGEST_BINDING",
     "confirm source review digest remains required and absent",
     "source review digest placeholder",
     "no Node review file is created"},
    {4,
     "CAPTURE_PREFLIGHT_ATTEST_OPERATOR_IDENTITY",
     "confirm operator identity remains required and absent",
     "identity mirror",
     "no identity value is stored"},
    {5,
     "CAPTURE_PREFLIGHT_ATTEST_OPERATOR_ROLE",
     "confirm operator role remains required and absent",
     "role mirror",
     "no role value is stored"},
    {6,
     "CAPTURE_PREFLIGHT_ATTEST_APPROVAL_TIMESTAMP",
     "confirm approval timestamp remains required and absent",
     "timestamp mirror",
     "no timestamp is generated"},
    {7,
     "CAPTURE_PREFLIGHT_ATTEST_CAPTURE_WINDOW",
     "confirm manual capture window stays closed",
     "capture window placeholder",
     "no sibling service is started"},
    {8,
     "CAPTURE_PREFLIGHT_ATTEST_CHANNEL_POLICY",
     "confirm capture channel is policy-only",
     "channel policy placeholder",
     "no raw endpoint is accepted"},
    {9,
     "CAPTURE_PREFLIGHT_ATTEST_SIGNATURE_ALGORITHM",
     "confirm signature algorithm is metadata only",
     "signature algorithm placeholder",
     "no signature bytes are parsed"},
    {10,
     "CAPTURE_PREFLIGHT_ATTEST_SIGNATURE_REDACTION",
     "confirm raw signature material is locked out",
     "signature redaction policy",
     "no raw signature is stored"},
    {11,
     "CAPTURE_PREFLIGHT_ATTEST_APPROVAL_STATEMENT",
     "confirm approval statement is absent",
     "approval statement placeholder",
     "no approval text is fabricated"},
    {12,
     "CAPTURE_PREFLIGHT_ATTEST_JUSTIFICATION",
     "confirm justification remains required and absent",
     "justification mirror",
     "no justification is fabricated"},
    {13,
     "CAPTURE_PREFLIGHT_ATTEST_SOURCE_VERSION",
     "confirm source evidence version remains required",
     "source release mirror",
     "no rolling current source is used"},
    {14,
     "CAPTURE_PREFLIGHT_ATTEST_SOURCE_FILE",
     "confirm source file id remains required",
     "source file mirror",
     "no runtime file lookup is performed"},
    {15,
     "CAPTURE_PREFLIGHT_ATTEST_SOURCE_SNIPPET",
     "confirm source snippet id remains required",
     "source snippet mirror",
     "no runtime snippet lookup is performed"},
    {16,
     "CAPTURE_PREFLIGHT_ATTEST_REDACTED_DIGEST",
     "confirm redacted value digest remains required",
     "redacted digest mirror",
     "no raw value material is accepted"},
    {17,
     "CAPTURE_PREFLIGHT_ATTEST_VALUE_SHAPE",
     "confirm value shape remains required",
     "typed envelope mirror",
     "no value payload is accepted"},
    {18,
     "CAPTURE_PREFLIGHT_ATTEST_REDACTION_POLICY",
     "confirm redaction policy remains required",
     "redaction policy mirror",
     "no unredacted evidence is stored"},
    {19,
     "CAPTURE_PREFLIGHT_ATTEST_PROVENANCE",
     "confirm provenance policy remains required",
     "provenance mirror",
     "no evidence import is performed"},
    {20,
     "CAPTURE_PREFLIGHT_ATTEST_RAW_MATERIAL_LOCK",
     "confirm credential, secret, endpoint, and signature material are locked",
     "raw material storage lock",
     "no credential or endpoint value is stored"},
    {21,
     "CAPTURE_PREFLIGHT_ATTEST_GRANT_LOCK",
     "confirm approval grant is absent and not emitted",
     "approval grant lock",
     "no approval state is shared"},
    {22,
     "CAPTURE_PREFLIGHT_ATTEST_VALUE_LOCK",
     "confirm supplied, accepted, imported, and persisted value counts are zero",
     "value submission lock",
     "no sibling import is triggered"},
    {23,
     "CAPTURE_PREFLIGHT_ATTEST_WRITE_ROUTE_LOCK",
     "confirm write route is not installed",
     "write route lock",
     "no router is activated"},
    {24,
     "CAPTURE_PREFLIGHT_ATTEST_SIBLING_NON_MUTATION",
     "confirm Node, Java, and sibling evidence are not mutated",
     "sibling non-mutation lock",
     "no sibling project is edited or executed"},
    {25,
     "CAPTURE_PREFLIGHT_ATTEST_CLOSEOUT_BOUNDARY",
     "confirm closeout stays preflight-only",
     "closeout boundary",
     "next signed artifact review remains separate"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_preflight_attestation_count() {
    return static_cast<int>(capture_preflight_attestations.size());
}

std::string format_signed_approval_capture_preflight_attestations_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_preflight_attestation_count());

    std::string response = "[";
    bool first = true;
    for (const auto& attestation : capture_preflight_attestations) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(attestation.sequence) +
                    ",\"attestationCode\":" + json_string(attestation.attestation_code) +
                    ",\"requiredReview\":" + json_string(attestation.required_review) +
                    ",\"evidenceScope\":" + json_string(attestation.evidence_scope) +
                    ",\"siblingBoundary\":" + json_string(attestation.sibling_boundary) +
                    ",\"publishedByCurrentStage\":" + json_bool(attestation.sequence <= clamped_published_count) +
                    ",\"operatorReviewRequired\":true"
                    ",\"operatorReviewPresent\":false"
                    ",\"signedApprovalCaptureAllowed\":false"
                    ",\"approvalGrantAllowed\":false"
                    ",\"valueImportAllowed\":false"
                    ",\"siblingMutationAllowed\":false"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations
