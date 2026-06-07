#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies {
namespace {

struct CapturePreflightPolicy {
    int sequence;
    std::string_view policy_code;
    std::string_view rejection_code;
    std::string_view lock_clause;
    std::string_view control_plane_instruction;
};

constexpr std::array<CapturePreflightPolicy, 25> capture_preflight_policies = {{
    {1,
     "CAPTURE_PREFLIGHT_REQUIRE_REQUEST_ID",
     "REJECT_MISSING_CAPTURE_PREFLIGHT_REQUEST_ID",
     "do not capture a signed approval without a preflight request id",
     "treat missing request id as blocked"},
    {2,
     "CAPTURE_PREFLIGHT_REQUIRE_SOURCE_TEMPLATE_DIGEST",
     "REJECT_UNBOUND_SOURCE_TEMPLATE_DIGEST",
     "do not detach capture preflight from v685 template evidence",
     "use the frozen source template digest marker only"},
    {3,
     "CAPTURE_PREFLIGHT_REQUIRE_SOURCE_REVIEW_DIGEST",
     "REJECT_UNBOUND_SOURCE_REVIEW_DIGEST",
     "do not trust review material without digest binding",
     "keep review digest required and absent"},
    {4,
     "CAPTURE_PREFLIGHT_REQUIRE_OPERATOR_IDENTITY",
     "REJECT_MISSING_OPERATOR_IDENTITY",
     "do not infer identity from local process or account state",
     "keep identity external and absent"},
    {5,
     "CAPTURE_PREFLIGHT_REQUIRE_OPERATOR_ROLE",
     "REJECT_MISSING_OPERATOR_ROLE",
     "do not infer approval role from command access",
     "keep role external and absent"},
    {6,
     "CAPTURE_PREFLIGHT_REQUIRE_APPROVAL_TIMESTAMP",
     "REJECT_MISSING_APPROVAL_TIMESTAMP",
     "do not synthesize timestamps in mini-kv",
     "keep timestamp external and absent"},
    {7,
     "CAPTURE_PREFLIGHT_REQUIRE_CAPTURE_WINDOW",
     "REJECT_CLOSED_CAPTURE_WINDOW",
     "do not open capture windows from mini-kv",
     "manual capture window remains closed"},
    {8,
     "CAPTURE_PREFLIGHT_CHANNEL_POLICY_CLOSED",
     "REJECT_UNAPPROVED_CAPTURE_CHANNEL",
     "do not accept signatures through raw endpoints",
     "capture channel is policy-only"},
    {9,
     "CAPTURE_PREFLIGHT_SIGNATURE_ALGORITHM_POLICY_ONLY",
     "REJECT_SIGNATURE_ALGORITHM_MATERIAL",
     "do not validate or persist signature bytes",
     "algorithm policy is metadata only"},
    {10,
     "CAPTURE_PREFLIGHT_REDACT_SIGNATURE_MATERIAL",
     "REJECT_RAW_SIGNATURE_MATERIAL",
     "do not store raw signature or secret material",
     "only redacted future references may be reviewed"},
    {11,
     "CAPTURE_PREFLIGHT_APPROVAL_STATEMENT_ABSENT",
     "REJECT_MISSING_APPROVAL_STATEMENT",
     "do not invent approval statements",
     "approval statement remains absent"},
    {12,
     "CAPTURE_PREFLIGHT_REQUIRE_JUSTIFICATION",
     "REJECT_MISSING_OPERATOR_JUSTIFICATION",
     "do not fabricate operator justification",
     "justification remains external and absent"},
    {13,
     "CAPTURE_PREFLIGHT_REQUIRE_SOURCE_VERSION",
     "REJECT_ROLLING_CURRENT_SOURCE_EVIDENCE",
     "do not approve rolling current evidence",
     "require a frozen source evidence version"},
    {14,
     "CAPTURE_PREFLIGHT_REQUIRE_SOURCE_FILE_ID",
     "REJECT_UNBOUND_SOURCE_FILE",
     "do not resolve source files at runtime",
     "source file id remains an external reference"},
    {15,
     "CAPTURE_PREFLIGHT_REQUIRE_SOURCE_SNIPPET_ID",
     "REJECT_UNBOUND_SOURCE_SNIPPET",
     "do not resolve source snippets at runtime",
     "source snippet id remains an external reference"},
    {16,
     "CAPTURE_PREFLIGHT_REQUIRE_REDACTED_VALUE_DIGEST",
     "REJECT_RAW_OR_UNDIGESTED_VALUE",
     "do not accept raw value material",
     "redacted digest remains required and absent"},
    {17,
     "CAPTURE_PREFLIGHT_REQUIRE_VALUE_SHAPE",
     "REJECT_UNTYPED_VALUE_SHAPE",
     "do not accept values without typed envelope binding",
     "value shape remains required and absent"},
    {18,
     "CAPTURE_PREFLIGHT_REQUIRE_REDACTION_POLICY",
     "REJECT_MISSING_REDACTION_POLICY",
     "do not persist unredacted evidence",
     "redaction policy remains required and absent"},
    {19,
     "CAPTURE_PREFLIGHT_REQUIRE_PROVENANCE_POLICY",
     "REJECT_MISSING_PROVENANCE_POLICY",
     "do not import evidence without provenance",
     "provenance remains required and absent"},
    {20,
     "CAPTURE_PREFLIGHT_LOCK_RAW_MATERIAL_STORAGE",
     "REJECT_SECRET_OR_SIGNATURE_STORAGE",
     "do not store credential, secret, endpoint, or signature material",
     "all raw material storage stays locked"},
    {21,
     "CAPTURE_PREFLIGHT_LOCK_APPROVAL_GRANT",
     "REJECT_APPROVAL_GRANT_EMISSION",
     "do not emit or accept approval grants",
     "grant state stays absent"},
    {22,
     "CAPTURE_PREFLIGHT_LOCK_VALUE_SUBMISSION",
     "REJECT_VALUE_SUBMISSION_ACCEPTANCE_OR_IMPORT",
     "do not submit, accept, import, or persist operator values",
     "all value counts stay zero"},
    {23,
     "CAPTURE_PREFLIGHT_LOCK_WRITE_ROUTE",
     "REJECT_WRITE_ROUTE_INSTALLATION",
     "do not install write routes",
     "route preview remains metadata only"},
    {24,
     "CAPTURE_PREFLIGHT_LOCK_SIBLING_MUTATION",
     "REJECT_SIBLING_PROJECT_MUTATION",
     "do not mutate Node, Java, or sibling evidence",
     "sibling projects remain read-only context"},
    {25,
     "CAPTURE_PREFLIGHT_CLOSEOUT_BOUNDARY",
     "REJECT_CAPTURE_IMPORT_OR_EXECUTION_IN_CLOSEOUT",
     "do not turn closeout into capture, import, router, write, or execution",
     "future signed artifact review must be a separate version chain"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_preflight_policy_count() {
    return static_cast<int>(capture_preflight_policies.size());
}

std::string format_signed_approval_capture_preflight_policies_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_preflight_policy_count());

    std::string response = "[";
    bool first = true;
    for (const auto& policy : capture_preflight_policies) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(policy.sequence) +
                    ",\"policyCode\":" + json_string(policy.policy_code) +
                    ",\"rejectionCode\":" + json_string(policy.rejection_code) +
                    ",\"lockClause\":" + json_string(policy.lock_clause) +
                    ",\"controlPlaneInstruction\":" + json_string(policy.control_plane_instruction) +
                    ",\"publishedByCurrentStage\":" + json_bool(policy.sequence <= clamped_published_count) +
                    ",\"blocksSignedApprovalCapture\":true"
                    ",\"blocksApprovalGrant\":true"
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

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies
