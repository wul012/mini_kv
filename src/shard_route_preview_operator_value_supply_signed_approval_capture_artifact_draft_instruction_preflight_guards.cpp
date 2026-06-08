#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards {
namespace {

constexpr std::array<InstructionPreflightGuard, 25> instruction_guards = {{
    {1, "request_manifest_guard", "draft_instruction_text", "request manifest is metadata only"},
    {2, "artifact_preflight_digest_guard", "artifact_materialization", "digest binding must not create artifacts"},
    {3, "template_digest_guard", "template_materialization", "template digest must not generate text"},
    {4, "review_digest_guard", "approval_statement_text", "review digest must not write approval statements"},
    {5, "operator_identity_guard", "credential_value", "operator identity must not carry credentials"},
    {6, "operator_role_guard", "approval_grant", "operator role must not grant authority"},
    {7, "window_id_guard", "approval_capture", "window id must not open capture"},
    {8, "channel_policy_guard", "raw_endpoint", "channel policy must not store endpoints"},
    {9, "signature_algorithm_guard", "signature_payload", "algorithm policy must not accept signatures"},
    {10, "detached_signature_guard", "detached_signature_payload", "detached signature stays outside mini-kv"},
    {11, "signature_redaction_guard", "raw_signature_material", "raw signature bytes stay blocked"},
    {12, "approval_statement_digest_guard", "approval_statement_text", "digest only blocks statement text"},
    {13, "source_version_guard", "runtime_source_lookup", "source version is cited from frozen evidence"},
    {14, "source_file_guard", "runtime_file_walk", "source file id is cited without walking files"},
    {15, "source_snippet_guard", "draft_text_assembly", "source snippet stays citation metadata only"},
    {16, "redacted_value_digest_guard", "raw_operator_value", "redacted digest blocks raw values"},
    {17, "value_shape_guard", "operator_value_acceptance", "value shape must not accept values"},
    {18, "redaction_policy_guard", "raw_secret_value", "redaction policy keeps secrets absent"},
    {19, "provenance_policy_guard", "mutable_provenance", "provenance policy remains read-only"},
    {20, "raw_secret_embargo_guard", "secret_storage", "raw secret storage stays forbidden"},
    {21, "approval_grant_embargo_guard", "grant_emission", "approval grant emission stays forbidden"},
    {22, "zero_value_import_embargo_guard", "operator_value_import", "operator value import stays at zero"},
    {23, "write_route_embargo_guard", "write_routing", "write routes remain disabled"},
    {24, "sibling_non_mutation_guard", "sibling_project_mutation", "sibling projects are not mutated"},
    {25, "closeout_guard", "execution", "closeout remains evidence-only and non-executing"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(instruction_guards.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const InstructionPreflightGuard> signed_approval_capture_artifact_draft_instruction_preflight_guards() {
    return {instruction_guards.data(), instruction_guards.size()};
}

int planned_signed_approval_capture_artifact_draft_instruction_preflight_guard_count() {
    return static_cast<int>(instruction_guards.size());
}

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_guards_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& guard = instruction_guards[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(guard.sequence) +
                  ",\"guardCode\":" + json_string(guard.guard_code) +
                  ",\"blocks\":" + json_string(guard.blocks) +
                  ",\"reason\":" + json_string(guard.reason) +
                  ",\"draftInstructionTextBlocked\":true"
                  ",\"runtimeExecutionBlocked\":true"
                  ",\"writeRouteBlocked\":true"
                  ",\"siblingMutationBlocked\":true}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards
