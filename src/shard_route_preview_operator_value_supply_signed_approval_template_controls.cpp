#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_controls.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_controls {
namespace {

struct SignedApprovalTemplateReviewControl {
    int sequence;
    std::string_view review_control_code;
    std::string_view source_review_package_control;
    std::string_view required_operator_review;
    std::string_view control_plane_instruction;
};

constexpr std::array<SignedApprovalTemplateReviewControl, 25> signed_approval_template_review_controls = {{
    {1,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_PACKET_ID",
     "APPROVAL_PACKET_REVIEW_PACKET_ID",
     "confirm the signed approval packet id is a future required field",
     "treat missing packet id as blocked"},
    {2,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_OPERATOR_IDENTITY",
     "APPROVAL_PACKET_REVIEW_OPERATOR_IDENTITY",
     "confirm operator identity is required and currently absent",
     "do not infer identity from local process state"},
    {3,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_OPERATOR_ROLE",
     "APPROVAL_PACKET_REVIEW_OPERATOR_ROLE",
     "confirm role scope is required and currently absent",
     "do not infer role from command access"},
    {4,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_APPROVAL_TIMESTAMP",
     "APPROVAL_PACKET_REVIEW_APPROVAL_TIMESTAMP",
     "confirm approval timestamp is required and currently absent",
     "do not synthesize timestamps"},
    {5,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SOURCE_REVIEW_PACKAGE_DIGEST",
     "APPROVAL_PACKET_REVIEW_PACKAGE_DIGEST",
     "confirm source review package digest is required and currently absent",
     "do not trust unbound review material"},
    {6,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SOURCE_DRAFT_DIGEST",
     "APPROVAL_PACKET_REVIEW_DRAFT_DIGEST",
     "confirm source draft digest is required and currently absent",
     "do not detach template from draft evidence"},
    {7,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SOURCE_RELEASE_VERSION",
     "APPROVAL_PACKET_REVIEW_SOURCE_RELEASE",
     "confirm source release version is required and currently absent",
     "do not use rolling current as signed source"},
    {8,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SOURCE_FILE_ID",
     "APPROVAL_PACKET_REVIEW_SOURCE_FILE_ID",
     "confirm source file id is required and currently absent",
     "do not resolve source files at runtime"},
    {9,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SOURCE_SNIPPET_ID",
     "APPROVAL_PACKET_REVIEW_SOURCE_SNIPPET_ID",
     "confirm source snippet id is required and currently absent",
     "do not resolve source snippets at runtime"},
    {10,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_TYPED_VALUE_ENVELOPE",
     "APPROVAL_PACKET_REVIEW_TYPED_VALUE_ENVELOPE",
     "confirm typed value envelope is required before future approval",
     "do not accept values in this template"},
    {11,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_REDACTED_VALUE_DIGEST",
     "APPROVAL_PACKET_REVIEW_REDACTED_DIGEST",
     "confirm redacted value digest is required before future approval",
     "do not store raw values"},
    {12,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_REDACTION_POLICY",
     "APPROVAL_PACKET_REVIEW_REDACTION_POLICY",
     "confirm redaction policy acknowledgement is required",
     "do not persist evidence through this template"},
    {13,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_PROVENANCE_BINDING",
     "APPROVAL_PACKET_REVIEW_PROVENANCE",
     "confirm provenance binding is required",
     "do not import evidence without provenance"},
    {14,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_MISSING_VALUE_POLICY",
     "APPROVAL_PACKET_REVIEW_MISSING_VALUE_POLICY",
     "confirm missing value fail-closed policy is required",
     "treat missing values as blocked"},
    {15,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_MALFORMED_VALUE_POLICY",
     "APPROVAL_PACKET_REVIEW_MALFORMED_VALUE_POLICY",
     "confirm malformed value rejection policy is required",
     "treat malformed values as blocked"},
    {16,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_SIGNED_APPROVAL_POLICY",
     "APPROVAL_PACKET_REVIEW_SIGNED_APPROVAL_POLICY",
     "confirm signed approval policy acknowledgement is required",
     "do not treat acknowledgement as a grant"},
    {17,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_DRAFT_ONLY_POLICY",
     "APPROVAL_PACKET_REVIEW_DRAFT_ONLY_POLICY",
     "confirm draft-only packet policy acknowledgement is required",
     "do not promote drafts to runtime state"},
    {18,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_JUSTIFICATION",
     "APPROVAL_PACKET_REVIEW_JUSTIFICATION",
     "confirm justification summary is required",
     "do not fabricate operator justification"},
    {19,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_RISK_ACK",
     "APPROVAL_PACKET_REVIEW_RISK_ACK",
     "confirm risk acknowledgement is required",
     "do not infer risk acknowledgement"},
    {20,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_EVIDENCE_COVERAGE",
     "APPROVAL_PACKET_REVIEW_EVIDENCE_COVERAGE",
     "confirm evidence coverage statement is required",
     "do not approve incomplete coverage"},
    {21,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_CONTROL_CHECKLIST",
     "APPROVAL_PACKET_REVIEW_CONTROL_CHECKLIST",
     "confirm review control checklist is required",
     "do not bypass review controls"},
    {22,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_NO_PRIOR_APPROVAL",
     "APPROVAL_PACKET_REVIEW_NO_PRIOR_APPROVAL",
     "confirm prior approval is not assumed",
     "do not carry approval state from previous layers"},
    {23,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_ZERO_VALUE_COUNT",
     "APPROVAL_PACKET_REVIEW_ZERO_VALUE_COUNT",
     "confirm supplied and accepted value counts remain zero",
     "do not carry values in the template"},
    {24,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_MUTATION_BLOCK",
     "APPROVAL_PACKET_REVIEW_MUTATION_BLOCK",
     "confirm write, service, and sibling mutation blocks remain active",
     "do not write or start sibling services"},
    {25,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_CAPTURE_PREFLIGHT_HANDOFF",
     "APPROVAL_PACKET_REVIEW_CAPTURE_PREFLIGHT_HANDOFF",
     "confirm the next step is capture preflight only",
     "do not import values or execute live reads"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_template_review_control_count() {
    return static_cast<int>(signed_approval_template_review_controls.size());
}

std::string format_signed_approval_template_review_controls_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_template_review_control_count());

    std::string response = "[";
    bool first = true;
    for (const auto& control : signed_approval_template_review_controls) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(control.sequence) +
                    ",\"reviewControlCode\":" + json_string(control.review_control_code) +
                    ",\"sourceReviewPackageControl\":" + json_string(control.source_review_package_control) +
                    ",\"requiredOperatorReview\":" + json_string(control.required_operator_review) +
                    ",\"controlPlaneInstruction\":" + json_string(control.control_plane_instruction) +
                    ",\"publishedByCurrentStage\":" + json_bool(control.sequence <= clamped_published_count) +
                    ",\"sourceReviewControlBound\":true"
                    ",\"operatorReviewRequired\":true"
                    ",\"operatorReviewPresent\":false"
                    ",\"signedApprovalCaptureAllowed\":false"
                    ",\"valueImportAllowed\":false"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_controls
