#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs {
namespace {

struct CapturePreflightInput {
    int sequence;
    std::string_view input_code;
    std::string_view input_field;
    std::string_view source_binding;
    std::string_view absent_state;
};

constexpr std::array<CapturePreflightInput, 25> capture_preflight_inputs = {{
    {1,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_REQUEST_ID",
     "capture_preflight_request_id",
     "new opaque capture preflight request id",
     "not minted by mini-kv"},
    {2,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SOURCE_TEMPLATE_DIGEST",
     "source_signed_approval_template_digest",
     "v685 signed approval template digest marker",
     "source digest is referenced but not recomputed from runtime state"},
    {3,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SOURCE_REVIEW_DIGEST",
     "source_approval_packet_review_digest",
     "future approval packet review digest",
     "review digest is required and absent"},
    {4,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_OPERATOR_IDENTITY",
     "operator_identity_mirror",
     "signed template operator identity field",
     "operator identity is mirrored as required and absent"},
    {5,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_OPERATOR_ROLE",
     "operator_role_mirror",
     "signed template operator role field",
     "operator role is mirrored as required and absent"},
    {6,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_APPROVAL_TIMESTAMP",
     "approval_timestamp_source",
     "signed template approval timestamp field",
     "approval timestamp is required and absent"},
    {7,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_CAPTURE_WINDOW",
     "manual_capture_window_id",
     "manual review window id",
     "manual capture window is not opened"},
    {8,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_CHANNEL_POLICY",
     "capture_channel_policy",
     "controlled capture channel policy",
     "capture channel is policy-only and closed"},
    {9,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SIGNATURE_ALGORITHM",
     "signature_algorithm_policy",
     "signature algorithm policy placeholder",
     "algorithm is declared but no signature is accepted"},
    {10,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SIGNATURE_REDACTION",
     "signature_material_redaction_policy",
     "raw signature redaction policy",
     "raw signature material is not stored"},
    {11,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_APPROVAL_STATEMENT",
     "approval_statement_placeholder",
     "approval statement placeholder",
     "approval statement is not supplied"},
    {12,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_OPERATOR_JUSTIFICATION",
     "operator_justification_mirror",
     "signed template justification summary",
     "operator justification is required and absent"},
    {13,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SOURCE_EVIDENCE_VERSION",
     "source_evidence_version_mirror",
     "signed template source release version",
     "source evidence version is required and absent"},
    {14,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SOURCE_FILE_ID",
     "source_evidence_file_id_mirror",
     "signed template source file id",
     "source file id is required and absent"},
    {15,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SOURCE_SNIPPET_ID",
     "source_evidence_snippet_id_mirror",
     "signed template source snippet id",
     "source snippet id is required and absent"},
    {16,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_REDACTED_VALUE_DIGEST",
     "redacted_value_digest_reference",
     "signed template redacted value digest",
     "redacted value digest is required and absent"},
    {17,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_VALUE_SHAPE",
     "value_shape_binding",
     "signed template typed value envelope",
     "value shape is required and absent"},
    {18,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_REDACTION_POLICY",
     "redaction_policy_mirror",
     "signed template redaction acknowledgement",
     "redaction policy acknowledgement is required and absent"},
    {19,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_PROVENANCE_POLICY",
     "provenance_policy_mirror",
     "signed template provenance binding",
     "provenance policy is required and absent"},
    {20,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_RAW_MATERIAL_LOCK",
     "raw_secret_signature_material_lock",
     "raw material storage lock",
     "raw secret and signature material are locked out"},
    {21,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_GRANT_LOCK",
     "approval_grant_emission_lock",
     "approval grant emission lock",
     "approval grant is not emitted"},
    {22,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_VALUE_SUBMISSION_LOCK",
     "value_submission_acceptance_import_lock",
     "value submission, acceptance, and import lock",
     "operator value counts remain zero"},
    {23,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_WRITE_ROUTE_LOCK",
     "write_route_lock",
     "write route lock",
     "write routing remains disabled"},
    {24,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_SIBLING_NON_MUTATION",
     "sibling_non_mutation_lock",
     "sibling non-mutation lock",
     "Node, Java, and sibling evidence are not mutated"},
    {25,
     "SIGNED_APPROVAL_CAPTURE_PREFLIGHT_CLOSEOUT",
     "capture_preflight_closeout_boundary",
     "capture preflight closeout boundary",
     "next step remains separate signed artifact review"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_preflight_input_count() {
    return static_cast<int>(capture_preflight_inputs.size());
}

std::string format_signed_approval_capture_preflight_inputs_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_preflight_input_count());

    std::string response = "[";
    bool first = true;
    for (const auto& input : capture_preflight_inputs) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(input.sequence) +
                    ",\"inputCode\":" + json_string(input.input_code) +
                    ",\"inputField\":" + json_string(input.input_field) +
                    ",\"sourceBinding\":" + json_string(input.source_binding) +
                    ",\"absentState\":" + json_string(input.absent_state) +
                    ",\"publishedByCurrentStage\":" + json_bool(input.sequence <= clamped_published_count) +
                    ",\"requiredBeforeSignedApprovalCapture\":true"
                    ",\"presentInMiniKv\":false"
                    ",\"capturedByMiniKv\":false"
                    ",\"signedApprovalPresent\":false"
                    ",\"approvalGrantPresent\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValuesPersisted\":false"
                    ",\"automaticSiblingImportAllowed\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"readOnly\":true"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs
