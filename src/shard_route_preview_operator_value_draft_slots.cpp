#include "minikv/shard_route_preview_operator_value_draft_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_draft_slots {
namespace {

struct ValueDraftSlot {
    int sequence;
    std::string_view draft_code;
    std::string_view source_import_preflight_code;
    std::string_view operator_instruction;
};

constexpr std::array<ValueDraftSlot, 25> value_draft_slots = {{
    {1,
     "VALUE_DRAFT_SLOT_TEMPLATE_SPLIT",
     "IMPORT_PREFLIGHT_BOUNDARY_HELPER_SPLIT",
     "declare the value draft slot template in a split helper without accepting a value"},
    {2,
     "VALUE_DRAFT_SOURCE_PREFLIGHT_CONTRACT",
     "IMPORT_PREFLIGHT_SOURCE_WORKSHEET_CONTRACT",
     "bind each draft slot to the frozen import preflight command contract"},
    {3,
     "VALUE_DRAFT_SOURCE_RELEASE_FREEZE",
     "IMPORT_PREFLIGHT_SOURCE_RELEASE_FREEZE",
     "pin v560 import preflight as source evidence before any operator value appears"},
    {4,
     "VALUE_DRAFT_STAGE_CHAIN_AUDIT",
     "IMPORT_PREFLIGHT_STAGE_CHAIN_AUDIT",
     "audit value draft stage ordering without looking up runtime files"},
    {5,
     "VALUE_DRAFT_FIXTURE_CHAIN_AUDIT",
     "IMPORT_PREFLIGHT_FIXTURE_CHAIN_AUDIT",
     "audit fixture continuity while keeping current fixture generation read-only"},
    {6,
     "VALUE_DRAFT_COMMAND_CATALOG_CONTRACT",
     "IMPORT_PREFLIGHT_COMMAND_CATALOG_CONTRACT",
     "publish command catalog metadata for value drafts only"},
    {7,
     "VALUE_DRAFT_EXPLAIN_CONTRACT",
     "IMPORT_PREFLIGHT_EXPLAIN_CONTRACT",
     "explain the value draft command as metadata_read only"},
    {8,
     "VALUE_DRAFT_CHECKJSON_CONTRACT",
     "IMPORT_PREFLIGHT_CHECKJSON_CONTRACT",
     "check the value draft command without executing or importing evidence"},
    {9,
     "VALUE_DRAFT_SHARDJSON_EMBEDDING",
     "IMPORT_PREFLIGHT_SHARDJSON_EMBEDDING",
     "embed one value draft section in SHARDJSON"},
    {10,
     "VALUE_DRAFT_SHARDJSON_EMBEDDING_AUDIT",
     "IMPORT_PREFLIGHT_SHARDJSON_EMBEDDING_AUDIT",
     "audit that SHARDJSON contains exactly one value draft section"},
    {11,
     "VALUE_DRAFT_SLOT_BUILDER_CONTRACT",
     "IMPORT_PREFLIGHT_NORMALIZER_CONTRACT",
     "declare the slot builder as a template renderer rather than a normalizer"},
    {12,
     "VALUE_DRAFT_SLOT_COUNT_AUDIT",
     "IMPORT_PREFLIGHT_BLOCKER_CATALOG",
     "audit that the draft slot count matches the expected operator evidence slots"},
    {13,
     "VALUE_DRAFT_ACTUAL_VALUE_STATE",
     "IMPORT_PREFLIGHT_MISSING_VALUE_POLICY",
     "pin actualValueState to not-supplied for every slot"},
    {14,
     "VALUE_DRAFT_IMPORT_STATE_BLOCKED",
     "IMPORT_PREFLIGHT_REDACTION_PRESERVATION",
     "keep import state blocked and redact any hypothetical value shape"},
    {15,
     "VALUE_DRAFT_REDACTION_BOUNDARY",
     "IMPORT_PREFLIGHT_OPERATOR_SLOT_DRAFT_BOUNDARY",
     "prove placeholders cannot store credentials, secrets, or raw endpoints"},
    {16,
     "VALUE_DRAFT_NO_EVIDENCE_IMPORT_AUDIT",
     "IMPORT_PREFLIGHT_NO_EVIDENCE_IMPORT_AUDIT",
     "audit that no operator evidence value import path is enabled"},
    {17,
     "VALUE_DRAFT_NO_MANUAL_ENTRY_AUDIT",
     "IMPORT_PREFLIGHT_NO_MANUAL_ENTRY_AUDIT",
     "audit that manual entry remains blocked from mini-kv storage"},
    {18,
     "VALUE_DRAFT_NO_RUNTIME_PAYLOAD_AUDIT",
     "IMPORT_PREFLIGHT_NO_RUNTIME_PAYLOAD_AUDIT",
     "audit that runtime payloads are not accepted as draft values"},
    {19,
     "VALUE_DRAFT_NO_SYNTHETIC_EVIDENCE_AUDIT",
     "IMPORT_PREFLIGHT_NO_SYNTHETIC_EVIDENCE_AUDIT",
     "audit that synthetic evidence cannot become an accepted value"},
    {20,
     "VALUE_DRAFT_NO_SECRET_STORAGE_AUDIT",
     "IMPORT_PREFLIGHT_REDACTION_PRESERVATION",
     "audit that draft placeholders store no credential, secret, or endpoint value"},
    {21,
     "VALUE_DRAFT_NO_ROUTER_AUDIT",
     "IMPORT_PREFLIGHT_NO_ROUTER_AUDIT",
     "audit that value drafts install no active shard router"},
    {22,
     "VALUE_DRAFT_NO_WRITE_AUDIT",
     "IMPORT_PREFLIGHT_NO_WRITE_AUDIT",
     "audit that value drafts route no writes and touch no WAL"},
    {23,
     "VALUE_DRAFT_NO_EXECUTION_AUDIT",
     "IMPORT_PREFLIGHT_NO_EXECUTION_AUDIT",
     "audit that value drafts cannot trigger restore, load, compact, routing, or service execution"},
    {24,
     "VALUE_DRAFT_RELEASE_READINESS_AUDIT",
     "IMPORT_PREFLIGHT_RELEASE_READINESS_AUDIT",
     "audit release readiness while keeping actual values absent"},
    {25,
     "VALUE_DRAFT_RELEASE_PACKAGE",
     "IMPORT_PREFLIGHT_RELEASE_PACKAGE",
     "package the read-only value draft chain for downstream controlled review"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_value_draft_slot_count() {
    return static_cast<int>(value_draft_slots.size());
}

std::string format_value_draft_slots_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_value_draft_slot_count());

    std::string response = "[";
    bool first = true;
    for (const auto& slot : value_draft_slots) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(slot.sequence) +
                    ",\"draftCode\":" + json_string(slot.draft_code) +
                    ",\"sourceImportPreflightCode\":" +
                    json_string(slot.source_import_preflight_code) +
                    ",\"operatorInstruction\":" + json_string(slot.operator_instruction) +
                    ",\"publishedByCurrentStage\":" +
                    json_bool(slot.sequence <= clamped_published_count) +
                    ",\"draftValueState\":\"awaiting-operator-value\""
                    ",\"actualValueState\":\"not-supplied\""
                    ",\"operatorValueSupplied\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValueImported\":false"
                    ",\"readOnly\":true"
                    ",\"evidenceImportAllowed\":false"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_draft_slots
