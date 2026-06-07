#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_fields.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_fields {
namespace {

struct SignedApprovalTemplateField {
    int sequence;
    std::string_view field_code;
    std::string_view template_field;
    std::string_view value_class;
    std::string_view missing_field_blocker;
};

constexpr std::array<SignedApprovalTemplateField, 25> signed_approval_template_fields = {{
    {1,
     "SIGNED_APPROVAL_TEMPLATE_PACKET_ID",
     "signed_approval_packet_id",
     "opaque packet identifier",
     "missing signed approval packet id blocks capture preflight"},
    {2,
     "SIGNED_APPROVAL_TEMPLATE_OPERATOR_IDENTITY",
     "operator_identity",
     "human operator identity handle",
     "missing operator identity blocks capture preflight"},
    {3,
     "SIGNED_APPROVAL_TEMPLATE_OPERATOR_ROLE",
     "operator_role",
     "operator approval role",
     "missing operator role blocks capture preflight"},
    {4,
     "SIGNED_APPROVAL_TEMPLATE_APPROVAL_TIMESTAMP",
     "approval_timestamp",
     "review timestamp",
     "missing approval timestamp blocks capture preflight"},
    {5,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_REVIEW_PACKAGE_DIGEST",
     "source_review_package_digest",
     "Node v1011 review package digest",
     "missing source review package digest blocks capture preflight"},
    {6,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_DRAFT_DIGEST",
     "source_approval_draft_digest",
     "Node v986 draft package digest",
     "missing source draft digest blocks capture preflight"},
    {7,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_RELEASE_VERSION",
     "source_release_version",
     "versioned source release",
     "missing source release version blocks capture preflight"},
    {8,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_FILE_ID",
     "source_file_id",
     "source evidence file id",
     "missing source file id blocks capture preflight"},
    {9,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_SNIPPET_ID",
     "source_snippet_id",
     "source evidence snippet id",
     "missing source snippet id blocks capture preflight"},
    {10,
     "SIGNED_APPROVAL_TEMPLATE_TYPED_VALUE_ENVELOPE",
     "typed_value_envelope",
     "typed value envelope reference",
     "missing typed value envelope blocks value approval"},
    {11,
     "SIGNED_APPROVAL_TEMPLATE_REDACTED_VALUE_DIGEST",
     "redacted_value_digest",
     "redacted value digest",
     "missing redacted value digest blocks value approval"},
    {12,
     "SIGNED_APPROVAL_TEMPLATE_REDACTION_POLICY_ACK",
     "redaction_policy_ack",
     "redaction policy acknowledgement",
     "missing redaction acknowledgement blocks capture preflight"},
    {13,
     "SIGNED_APPROVAL_TEMPLATE_PROVENANCE_BINDING",
     "provenance_binding",
     "per-value provenance binding",
     "missing provenance binding blocks capture preflight"},
    {14,
     "SIGNED_APPROVAL_TEMPLATE_MISSING_VALUE_POLICY_ACK",
     "missing_value_policy_ack",
     "missing value policy acknowledgement",
     "missing missing-value policy acknowledgement blocks capture preflight"},
    {15,
     "SIGNED_APPROVAL_TEMPLATE_MALFORMED_VALUE_POLICY_ACK",
     "malformed_value_policy_ack",
     "malformed value policy acknowledgement",
     "missing malformed-value policy acknowledgement blocks capture preflight"},
    {16,
     "SIGNED_APPROVAL_TEMPLATE_SIGNED_APPROVAL_POLICY_ACK",
     "signed_approval_policy_ack",
     "signed approval policy acknowledgement",
     "missing signed approval policy acknowledgement blocks capture preflight"},
    {17,
     "SIGNED_APPROVAL_TEMPLATE_DRAFT_ONLY_PACKET_POLICY_ACK",
     "draft_only_packet_policy_ack",
     "draft-only packet policy acknowledgement",
     "missing draft-only policy acknowledgement blocks capture preflight"},
    {18,
     "SIGNED_APPROVAL_TEMPLATE_JUSTIFICATION_SUMMARY",
     "justification_summary",
     "operator justification summary",
     "missing justification summary blocks capture preflight"},
    {19,
     "SIGNED_APPROVAL_TEMPLATE_RISK_ACKNOWLEDGEMENT",
     "risk_acknowledgement",
     "risk acknowledgement",
     "missing risk acknowledgement blocks capture preflight"},
    {20,
     "SIGNED_APPROVAL_TEMPLATE_EVIDENCE_COVERAGE_STATEMENT",
     "evidence_coverage_statement",
     "evidence coverage statement",
     "missing evidence coverage statement blocks capture preflight"},
    {21,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_CONTROL_CHECKLIST",
     "review_control_checklist",
     "review control checklist",
     "missing review control checklist blocks capture preflight"},
    {22,
     "SIGNED_APPROVAL_TEMPLATE_NO_PRIOR_APPROVAL_ASSERTION",
     "no_prior_approval_assertion",
     "no prior approval assertion",
     "missing no-prior-approval assertion blocks capture preflight"},
    {23,
     "SIGNED_APPROVAL_TEMPLATE_ZERO_VALUE_COUNT_ASSERTION",
     "zero_value_count_assertion",
     "zero value count assertion",
     "missing zero-value-count assertion blocks capture preflight"},
    {24,
     "SIGNED_APPROVAL_TEMPLATE_WRITE_SERVICE_SIBLING_MUTATION_BLOCK",
     "write_service_sibling_mutation_block",
     "write service sibling mutation block",
     "missing mutation block blocks capture preflight"},
    {25,
     "SIGNED_APPROVAL_TEMPLATE_CAPTURE_PREFLIGHT_HANDOFF",
     "capture_preflight_handoff",
     "capture preflight handoff marker",
     "missing capture-preflight handoff marker blocks next lifecycle"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_template_field_count() {
    return static_cast<int>(signed_approval_template_fields.size());
}

std::string format_signed_approval_template_fields_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_template_field_count());

    std::string response = "[";
    bool first = true;
    for (const auto& field : signed_approval_template_fields) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(field.sequence) +
                    ",\"fieldCode\":" + json_string(field.field_code) +
                    ",\"templateField\":" + json_string(field.template_field) +
                    ",\"valueClass\":" + json_string(field.value_class) +
                    ",\"missingFieldBlocker\":" + json_string(field.missing_field_blocker) +
                    ",\"publishedByCurrentStage\":" + json_bool(field.sequence <= clamped_published_count) +
                    ",\"requiredBeforeSignedApprovalCapture\":true"
                    ",\"presentInMiniKv\":false"
                    ",\"signatureCaptured\":false"
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

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_fields
