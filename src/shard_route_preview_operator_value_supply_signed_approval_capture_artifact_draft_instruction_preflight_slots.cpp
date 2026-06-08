#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots {
namespace {

constexpr std::array<InstructionPreflightSlot, 25> instruction_slots = {{
    {1, "request_manifest_slot", "request manifest", "names the draft instruction request without creating instruction text"},
    {2, "artifact_preflight_digest_slot", "artifact preflight digest", "binds the artifact preflight closeout digest"},
    {3, "template_digest_slot", "template digest", "binds the signed approval template digest"},
    {4, "review_digest_slot", "review digest", "binds the manual review digest"},
    {5, "operator_identity_slot", "operator identity", "captures the identity field expectation without credentials"},
    {6, "operator_role_slot", "operator role", "captures the role field expectation without granting authority"},
    {7, "window_id_slot", "window id", "captures the closed capture window identifier"},
    {8, "channel_policy_slot", "channel policy", "captures channel policy without raw endpoints"},
    {9, "signature_algorithm_slot", "signature algorithm", "captures signature algorithm policy without signatures"},
    {10, "detached_signature_slot", "detached signature", "captures detached signature handling while payload stays absent"},
    {11, "signature_redaction_slot", "signature redaction", "captures signature redaction policy"},
    {12, "approval_statement_digest_slot", "approval statement digest", "captures statement digest without statement text"},
    {13, "source_version_slot", "source version", "captures source release citation"},
    {14, "source_file_slot", "source file", "captures source file citation without runtime lookup"},
    {15, "source_snippet_slot", "source snippet", "captures source snippet citation without assembling draft text"},
    {16, "redacted_value_digest_slot", "redacted value digest", "captures redacted value digest while raw value remains absent"},
    {17, "value_shape_slot", "value shape", "captures value shape metadata without accepting values"},
    {18, "redaction_policy_slot", "redaction policy", "captures redaction policy boundary"},
    {19, "provenance_policy_slot", "provenance policy", "captures provenance policy boundary"},
    {20, "raw_secret_embargo_slot", "raw secret embargo", "proves raw secret value remains unavailable"},
    {21, "approval_grant_embargo_slot", "approval grant embargo", "proves approval grants remain unavailable"},
    {22, "zero_value_import_embargo_slot", "zero value import embargo", "proves imported value counts stay at zero"},
    {23, "write_route_embargo_slot", "write route embargo", "proves write routing remains disabled"},
    {24, "sibling_non_mutation_slot", "sibling non-mutation", "proves sibling projects are not mutated"},
    {25, "closeout_slot", "closeout", "closes preflight without materializing draft instruction text"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(instruction_slots.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const InstructionPreflightSlot> signed_approval_capture_artifact_draft_instruction_preflight_slots() {
    return {instruction_slots.data(), instruction_slots.size()};
}

int planned_signed_approval_capture_artifact_draft_instruction_preflight_slot_count() {
    return static_cast<int>(instruction_slots.size());
}

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_slots_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& slot = instruction_slots[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(slot.sequence) +
                  ",\"slotCode\":" + json_string(slot.slot_code) +
                  ",\"sourceInstruction\":" + json_string(slot.source_instruction) +
                  ",\"meaning\":" + json_string(slot.meaning) +
                  ",\"instructionSlotPrepared\":true"
                  ",\"instructionTextMaterialized\":false"
                  ",\"materializesDraftText\":false"
                  ",\"acceptsSignatureMaterial\":false"
                  ",\"importsOperatorValue\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots
