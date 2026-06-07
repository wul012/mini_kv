#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_clauses.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_clauses {
namespace {

struct SignedApprovalTemplateClause {
    int sequence;
    std::string_view clause_code;
    std::string_view rejection_code;
    std::string_view non_execution_clause;
    std::string_view source_review_control;
};

constexpr std::array<SignedApprovalTemplateClause, 25> signed_approval_template_clauses = {{
    {1,
     "SIGNED_APPROVAL_TEMPLATE_PACKET_ID_CLAUSE",
     "REJECT_MISSING_SIGNED_APPROVAL_PACKET_ID",
     "do not capture approval without a packet id",
     "approval-packet-review-control-01"},
    {2,
     "SIGNED_APPROVAL_TEMPLATE_OPERATOR_IDENTITY_CLAUSE",
     "REJECT_MISSING_OPERATOR_IDENTITY",
     "do not capture approval without a human operator identity",
     "approval-packet-review-control-02"},
    {3,
     "SIGNED_APPROVAL_TEMPLATE_OPERATOR_ROLE_CLAUSE",
     "REJECT_MISSING_OPERATOR_ROLE",
     "do not capture approval without a scoped operator role",
     "approval-packet-review-control-03"},
    {4,
     "SIGNED_APPROVAL_TEMPLATE_APPROVAL_TIMESTAMP_CLAUSE",
     "REJECT_MISSING_APPROVAL_TIMESTAMP",
     "do not capture approval without an approval timestamp",
     "approval-packet-review-control-04"},
    {5,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_REVIEW_PACKAGE_DIGEST_CLAUSE",
     "REJECT_MISSING_SOURCE_REVIEW_PACKAGE_DIGEST",
     "do not trust unsigned review material",
     "approval-packet-review-control-05"},
    {6,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_DRAFT_DIGEST_CLAUSE",
     "REJECT_MISSING_SOURCE_DRAFT_DIGEST",
     "do not detach signed approval from the draft packet",
     "approval-packet-review-control-06"},
    {7,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_RELEASE_VERSION_CLAUSE",
     "REJECT_MISSING_SOURCE_RELEASE_VERSION",
     "do not approve rolling current evidence as source",
     "approval-packet-review-control-07"},
    {8,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_FILE_ID_CLAUSE",
     "REJECT_MISSING_SOURCE_FILE_ID",
     "do not approve unbound source files",
     "approval-packet-review-control-08"},
    {9,
     "SIGNED_APPROVAL_TEMPLATE_SOURCE_SNIPPET_ID_CLAUSE",
     "REJECT_MISSING_SOURCE_SNIPPET_ID",
     "do not approve unbound source snippets",
     "approval-packet-review-control-09"},
    {10,
     "SIGNED_APPROVAL_TEMPLATE_TYPED_VALUE_ENVELOPE_CLAUSE",
     "REJECT_MISSING_TYPED_VALUE_ENVELOPE",
     "do not accept operator values through the signed template",
     "approval-packet-review-control-10"},
    {11,
     "SIGNED_APPROVAL_TEMPLATE_REDACTED_VALUE_DIGEST_CLAUSE",
     "REJECT_MISSING_REDACTED_VALUE_DIGEST",
     "do not accept raw values as approval material",
     "approval-packet-review-control-11"},
    {12,
     "SIGNED_APPROVAL_TEMPLATE_REDACTION_POLICY_CLAUSE",
     "REJECT_MISSING_REDACTION_POLICY_ACK",
     "do not persist anything before redaction is proven",
     "approval-packet-review-control-12"},
    {13,
     "SIGNED_APPROVAL_TEMPLATE_PROVENANCE_BINDING_CLAUSE",
     "REJECT_MISSING_PROVENANCE_BINDING",
     "do not import evidence without provenance binding",
     "approval-packet-review-control-13"},
    {14,
     "SIGNED_APPROVAL_TEMPLATE_MISSING_VALUE_POLICY_CLAUSE",
     "REJECT_MISSING_MISSING_VALUE_POLICY_ACK",
     "fail closed for missing values",
     "approval-packet-review-control-14"},
    {15,
     "SIGNED_APPROVAL_TEMPLATE_MALFORMED_VALUE_POLICY_CLAUSE",
     "REJECT_MISSING_MALFORMED_VALUE_POLICY_ACK",
     "reject malformed values before any future import",
     "approval-packet-review-control-15"},
    {16,
     "SIGNED_APPROVAL_TEMPLATE_SIGNED_APPROVAL_POLICY_CLAUSE",
     "REJECT_MISSING_SIGNED_APPROVAL_POLICY_ACK",
     "do not treat policy acknowledgement as an approval grant",
     "approval-packet-review-control-16"},
    {17,
     "SIGNED_APPROVAL_TEMPLATE_DRAFT_ONLY_PACKET_POLICY_CLAUSE",
     "REJECT_MISSING_DRAFT_ONLY_PACKET_POLICY_ACK",
     "do not promote draft-only packets into runtime state",
     "approval-packet-review-control-17"},
    {18,
     "SIGNED_APPROVAL_TEMPLATE_JUSTIFICATION_CLAUSE",
     "REJECT_MISSING_JUSTIFICATION_SUMMARY",
     "do not capture approval without an operator justification",
     "approval-packet-review-control-18"},
    {19,
     "SIGNED_APPROVAL_TEMPLATE_RISK_ACKNOWLEDGEMENT_CLAUSE",
     "REJECT_MISSING_RISK_ACKNOWLEDGEMENT",
     "do not capture approval without risk acknowledgement",
     "approval-packet-review-control-19"},
    {20,
     "SIGNED_APPROVAL_TEMPLATE_EVIDENCE_COVERAGE_CLAUSE",
     "REJECT_MISSING_EVIDENCE_COVERAGE_STATEMENT",
     "do not capture approval for incomplete evidence coverage",
     "approval-packet-review-control-20"},
    {21,
     "SIGNED_APPROVAL_TEMPLATE_REVIEW_CONTROL_CHECKLIST_CLAUSE",
     "REJECT_MISSING_REVIEW_CONTROL_CHECKLIST",
     "do not bypass the review controls from the source package",
     "approval-packet-review-control-21"},
    {22,
     "SIGNED_APPROVAL_TEMPLATE_NO_PRIOR_APPROVAL_CLAUSE",
     "REJECT_PRIOR_APPROVAL_ASSUMPTION",
     "do not assume a prior approval exists",
     "approval-packet-review-control-22"},
    {23,
     "SIGNED_APPROVAL_TEMPLATE_ZERO_VALUE_COUNT_CLAUSE",
     "REJECT_NONZERO_VALUE_COUNT",
     "do not carry supplied or accepted values in this template",
     "approval-packet-review-control-23"},
    {24,
     "SIGNED_APPROVAL_TEMPLATE_MUTATION_BLOCK_CLAUSE",
     "REJECT_WRITE_SERVICE_OR_SIBLING_MUTATION",
     "do not write, start services, or mutate sibling evidence",
     "approval-packet-review-control-24"},
    {25,
     "SIGNED_APPROVAL_TEMPLATE_CAPTURE_PREFLIGHT_HANDOFF_CLAUSE",
     "REJECT_CAPTURE_OR_IMPORT_IN_TEMPLATE_LAYER",
     "hand off only to a future capture preflight, not import or execution",
     "approval-packet-review-control-25"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_template_clause_count() {
    return static_cast<int>(signed_approval_template_clauses.size());
}

std::string format_signed_approval_template_clauses_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_template_clause_count());

    std::string response = "[";
    bool first = true;
    for (const auto& clause : signed_approval_template_clauses) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(clause.sequence) +
                    ",\"clauseCode\":" + json_string(clause.clause_code) +
                    ",\"rejectionCode\":" + json_string(clause.rejection_code) +
                    ",\"nonExecutionClause\":" + json_string(clause.non_execution_clause) +
                    ",\"sourceReviewControl\":" + json_string(clause.source_review_control) +
                    ",\"publishedByCurrentStage\":" + json_bool(clause.sequence <= clamped_published_count) +
                    ",\"blocksSignedApprovalCapture\":true"
                    ",\"blocksOperatorValueImport\":true"
                    ",\"blocksAutomaticSiblingImport\":true"
                    ",\"blocksRuntimePayload\":true"
                    ",\"blocksRouterActivation\":true"
                    ",\"blocksWrites\":true"
                    ",\"blocksExecution\":true"
                    ",\"readOnly\":true}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_clauses
