#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>

namespace minikv::
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards {
namespace {

constexpr std::array<ComparedPackageEvidenceIntakeGuard, 10> compared_package_evidence_intake_guards = {{
    {1, "source_acceptance_precheck_guard", "package acceptance from precheck-only evidence",
     "precheck remains prerequisite evidence only"},
    {2, "manual_submission_reference_guard", "raw endpoint credential or package body intake",
     "manual references are handles only"},
    {3, "offline_comparison_result_guard", "synthetic compared package evidence acceptance",
     "offline result is future evidence, not fabricated here"},
    {4, "identity_binding_guard", "identity secret resolution or approval escalation",
     "identity binding remains handle-only"},
    {5, "digest_match_summary_guard", "digest summary as signed approval", "digest evidence cannot grant approval"},
    {6, "detached_signature_envelope_observation_guard", "signature payload parse or verification execution",
     "signature envelope is observed, not parsed"},
    {7, "source_and_operator_value_handle_guard", "operator value import or persistence",
     "handles are not concrete values"},
    {8, "policy_and_execution_lock_guard", "runtime enablement or execution unlock",
     "policy evidence keeps execution locked"},
    {9, "approval_grant_separation_guard", "approval grant capture or emission",
     "grant review is separated from grant capture"},
    {10, "archive_closeout_guard", "runtime archive walk sibling mutation or service start",
     "archive closeout remains read-only metadata"},
}};

std::string json_string(std::string_view value) { return runtime_evidence::json_string(value); }

} // namespace

std::span<const ComparedPackageEvidenceIntakeGuard>
signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards() {
    return {compared_package_evidence_intake_guards.data(), compared_package_evidence_intake_guards.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guard_count() {
    return static_cast<int>(compared_package_evidence_intake_guards.size());
}

std::string
format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards_json(
    int completed_guard_count) {
    const int visible_count = std::clamp(
        completed_guard_count, 0,
        planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guard_count());
    std::string json = "[";
    for (int index = 0; index < visible_count; ++index) {
        const auto& guard = compared_package_evidence_intake_guards[static_cast<std::size_t>(index)];
        if (index > 0) {
            json += ",";
        }
        json += "{\"sequence\":" + std::to_string(guard.sequence) + ",\"guardCode\":" + json_string(guard.guard_code) +
                ",\"blocks\":" + json_string(guard.blocks) + ",\"reason\":" + json_string(guard.reason) +
                ",\"readOnly\":true"
                ",\"guardBypassed\":false}";
    }
    json += "]";
    return json;
}

} // namespace
  // minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards