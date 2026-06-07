#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates {
namespace {

struct ArtifactPreflightGate {
    int sequence;
    std::string_view gate_code;
    std::string_view required_condition;
    std::string_view blocked_operation;
    std::string_view gate_owner;
};

constexpr std::array<ArtifactPreflightGate, 25> artifact_preflight_gates = {{
    {1, "ARTIFACT_PREFLIGHT_GATE_REQUEST_ID_REVIEW", "request id remains review-only",
     "mint artifact preflight request id", "operator review"},
    {2, "ARTIFACT_PREFLIGHT_GATE_CAPTURE_PREFLIGHT_DIGEST", "v710 capture preflight digest stays frozen",
     "reload sibling current fixture", "mini-kv fixture"},
    {3, "ARTIFACT_PREFLIGHT_GATE_TEMPLATE_DIGEST", "v685 template digest stays frozen",
     "rewrite source template evidence", "mini-kv fixture"},
    {4, "ARTIFACT_PREFLIGHT_GATE_REVIEW_DIGEST", "review digest is required and absent",
     "fabricate approval packet review digest", "operator review"},
    {5, "ARTIFACT_PREFLIGHT_GATE_OPERATOR_IDENTITY", "operator identity fragment is absent",
     "store operator identity value", "operator review"},
    {6, "ARTIFACT_PREFLIGHT_GATE_OPERATOR_ROLE", "operator role fragment is absent",
     "store operator role value", "operator review"},
    {7, "ARTIFACT_PREFLIGHT_GATE_WINDOW_ID", "capture window id is absent",
     "open capture window", "operator window"},
    {8, "ARTIFACT_PREFLIGHT_GATE_CHANNEL_POLICY", "channel policy is handle-only",
     "accept raw endpoint or channel value", "operator window"},
    {9, "ARTIFACT_PREFLIGHT_GATE_SIGNATURE_ALGORITHM", "signature algorithm is metadata-only",
     "parse signature algorithm into executable verifier", "signature review"},
    {10, "ARTIFACT_PREFLIGHT_GATE_DETACHED_SIGNATURE_PLACEHOLDER", "detached signature bytes are absent",
     "capture detached signature material", "signature review"},
    {11, "ARTIFACT_PREFLIGHT_GATE_SIGNATURE_REDACTION", "signature material is redacted",
     "store raw signature material", "signature review"},
    {12, "ARTIFACT_PREFLIGHT_GATE_APPROVAL_STATEMENT_DIGEST", "approval statement digest is absent",
     "fabricate approval statement", "operator review"},
    {13, "ARTIFACT_PREFLIGHT_GATE_SOURCE_VERSION", "source version is pinned",
     "consume rolling current evidence as source", "mini-kv fixture"},
    {14, "ARTIFACT_PREFLIGHT_GATE_SOURCE_FILE", "source file id is declarative",
     "perform runtime source file lookup", "mini-kv fixture"},
    {15, "ARTIFACT_PREFLIGHT_GATE_SOURCE_SNIPPET", "source snippet id is declarative",
     "perform runtime source snippet lookup", "mini-kv fixture"},
    {16, "ARTIFACT_PREFLIGHT_GATE_REDACTED_VALUE_DIGEST", "redacted value digest is placeholder-only",
     "accept raw value material", "value supply"},
    {17, "ARTIFACT_PREFLIGHT_GATE_VALUE_SHAPE", "value shape has no payload",
     "accept operator value payload", "value supply"},
    {18, "ARTIFACT_PREFLIGHT_GATE_REDACTION_POLICY", "redaction policy is required",
     "store unredacted evidence", "value supply"},
    {19, "ARTIFACT_PREFLIGHT_GATE_PROVENANCE_POLICY", "provenance policy is required",
     "import provenance-free evidence", "value supply"},
    {20, "ARTIFACT_PREFLIGHT_GATE_NO_RAW_SECRET", "raw secrets are forbidden",
     "store credential, endpoint, secret, or signature material", "raw material boundary"},
    {21, "ARTIFACT_PREFLIGHT_GATE_NO_APPROVAL_GRANT", "approval grant remains absent",
     "emit approval grant", "approval boundary"},
    {22, "ARTIFACT_PREFLIGHT_GATE_ZERO_VALUE_IMPORT", "value import count remains zero",
     "import operator value", "value supply"},
    {23, "ARTIFACT_PREFLIGHT_GATE_NO_WRITE_ROUTE", "write route remains absent",
     "install active write route", "router boundary"},
    {24, "ARTIFACT_PREFLIGHT_GATE_SIBLING_NON_MUTATION", "sibling projects remain untouched",
     "mutate Node or Java evidence", "sibling boundary"},
    {25, "ARTIFACT_PREFLIGHT_GATE_CLOSEOUT", "closeout remains preflight-only",
     "treat closeout as signed artifact draft", "batch closeout"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_signed_approval_capture_artifact_preflight_gate_count() {
    return static_cast<int>(artifact_preflight_gates.size());
}

std::string format_signed_approval_capture_artifact_preflight_gates_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_signed_approval_capture_artifact_preflight_gate_count());

    std::string response = "[";
    bool first = true;
    for (const auto& gate : artifact_preflight_gates) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(gate.sequence) +
                    ",\"gateCode\":" + json_string(gate.gate_code) +
                    ",\"requiredCondition\":" + json_string(gate.required_condition) +
                    ",\"blockedOperation\":" + json_string(gate.blocked_operation) +
                    ",\"gateOwner\":" + json_string(gate.gate_owner) +
                    ",\"publishedByCurrentStage\":" + json_bool(gate.sequence <= clamped_published_count) +
                    ",\"gateCheckPassed\":true"
                    ",\"blockedOperationAttempted\":false"
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

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates
