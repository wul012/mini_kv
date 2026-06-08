#include "minikv/shard_preview_candidate_request_package_items.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>

namespace minikv::shard_preview_candidate_request_package_items {
namespace {

constexpr std::array<CandidateRequestItem, 15> request_items = {{
    {1, "source_intake_contract_freeze", "source_intake_reference",
     "Provide only the frozen mini-kv v880 compared package evidence intake audit identity and digest reference."},
    {2, "required_candidate_field_manifest", "required_candidate_fields",
     "Provide the twenty named candidate fields as reviewed document fields, not as runtime payload values."},
    {3, "operator_instruction_packet", "operator_instruction",
     "Provide owner, handoff, and review instructions as handles without opening a router or endpoint."},
    {4, "identity_provenance_handles", "identity_and_provenance",
     "Provide operator identity and provenance handles without identity secret material or privilege grant."},
    {5, "digest_lineage_handles", "digest_lineage",
     "Provide candidate digest lineage handles without importing or hashing submitted payload bytes here."},
    {6, "signature_envelope_reference", "signature_envelope_reference",
     "Provide detached signature envelope references without parsing or verifying signature payloads."},
    {7, "policy_lock_handles", "policy_lock",
     "Provide policy lock handles that keep runtime execution and write routing disabled."},
    {8, "approval_separation_handles", "approval_separation",
     "Provide approval review separation handles without approval grant capture or signed approval emission."},
    {9, "missing_document_quarantine", "missing_document",
     "Keep the request package blocked while a reviewed real candidate document is absent."},
    {10, "synthetic_document_rejection_marker", "synthetic_document",
     "Reject synthetic candidate material and keep it outside the package acceptance path."},
    {11, "payload_import_denial_marker", "payload_import",
     "Deny staged payload import; the request package names fields but does not ingest values."},
    {12, "evaluation_acceptance_lock", "evaluation_acceptance",
     "Lock evaluation acceptance and rejection actions until a later real-document intake exists."},
    {13, "runtime_write_lock", "runtime_write",
     "Keep runtime payload import write routing WAL touch and execution disabled."},
    {14, "sibling_mutation_lock", "sibling_mutation",
     "Keep Node Java archive and sibling project mutation disabled for this mini-kv evidence."},
    {15, "archive_closeout_reference", "archive_closeout",
     "Publish archive closeout references for future consumers without walking runtime archives."},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const CandidateRequestItem> candidate_document_request_items() {
    return {request_items.data(), request_items.size()};
}

int planned_candidate_document_request_item_count() {
    return static_cast<int>(request_items.size());
}

std::string format_candidate_document_request_items_json(int completed_item_count) {
    const int visible_count = std::clamp(completed_item_count, 0, planned_candidate_document_request_item_count());
    std::string json = "[";
    for (int index = 0; index < visible_count; ++index) {
        const auto& item = request_items[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"sequence\":" + std::to_string(item.sequence) +
                ",\"itemCode\":" + json_string(item.item_code) +
                ",\"requestedFieldGroup\":" + json_string(item.requested_field_group) +
                ",\"operatorInstruction\":" + json_string(item.operator_instruction) +
                ",\"readOnly\":true"
                ",\"documentMaterialAccepted\":false"
                ",\"payloadImported\":false}";
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_candidate_request_package_items
