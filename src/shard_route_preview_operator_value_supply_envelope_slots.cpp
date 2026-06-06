#include "minikv/shard_route_preview_operator_value_supply_envelope_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_envelope_slots {
namespace {

struct ValueSupplySlot {
    int sequence;
    std::string_view envelope_code;
    std::string_view source_value_draft_code;
    std::string_view provenance_requirement;
    std::string_view redaction_rule;
};

constexpr std::array<ValueSupplySlot, 25> value_supply_slots = {{
    {1,
     "VALUE_SUPPLY_ENVELOPE_POLICY_SPLIT",
     "VALUE_DRAFT_SLOT_TEMPLATE_SPLIT",
     "bind the disabled envelope to the existing value draft slot template",
     "no operator value field is stored"},
    {2,
     "VALUE_SUPPLY_SOURCE_VALUE_DRAFT_CONTRACT",
     "VALUE_DRAFT_SOURCE_PREFLIGHT_CONTRACT",
     "require v585 value draft evidence before any value-supply review",
     "source evidence id only"},
    {3,
     "VALUE_SUPPLY_SOURCE_RELEASE_FREEZE",
     "VALUE_DRAFT_SOURCE_RELEASE_FREEZE",
     "pin v585 value draft as the frozen source for this envelope",
     "fixture path only"},
    {4,
     "VALUE_SUPPLY_STAGE_CHAIN_AUDIT",
     "VALUE_DRAFT_STAGE_CHAIN_AUDIT",
     "audit envelope stage ordering through shared stage-chain helpers",
     "stage labels only"},
    {5,
     "VALUE_SUPPLY_FIXTURE_CHAIN_AUDIT",
     "VALUE_DRAFT_FIXTURE_CHAIN_AUDIT",
     "audit fixture continuity without runtime file reads",
     "release references only"},
    {6,
     "VALUE_SUPPLY_COMMAND_CATALOG_CONTRACT",
     "VALUE_DRAFT_COMMAND_CATALOG_CONTRACT",
     "publish command catalog metadata for the disabled envelope",
     "command metadata only"},
    {7,
     "VALUE_SUPPLY_EXPLAIN_CONTRACT",
     "VALUE_DRAFT_EXPLAIN_CONTRACT",
     "explain the envelope command as metadata_read only",
     "risk metadata only"},
    {8,
     "VALUE_SUPPLY_CHECKJSON_CONTRACT",
     "VALUE_DRAFT_CHECKJSON_CONTRACT",
     "check the envelope command without importing or executing values",
     "execution contract only"},
    {9,
     "VALUE_SUPPLY_SHARDJSON_EMBEDDING",
     "VALUE_DRAFT_SHARDJSON_EMBEDDING",
     "embed one value-supply envelope section in SHARDJSON",
     "single section only"},
    {10,
     "VALUE_SUPPLY_SHARDJSON_EMBEDDING_AUDIT",
     "VALUE_DRAFT_SHARDJSON_EMBEDDING_AUDIT",
     "audit that SHARDJSON contains exactly one envelope section",
     "section count only"},
    {11,
     "VALUE_SUPPLY_SLOT_POLICY_CONTRACT",
     "VALUE_DRAFT_SLOT_BUILDER_CONTRACT",
     "declare per-slot disabled policy without accepting a value",
     "policy labels only"},
    {12,
     "VALUE_SUPPLY_SLOT_COUNT_AUDIT",
     "VALUE_DRAFT_SLOT_COUNT_AUDIT",
     "audit that envelope slots match the value draft slot count",
     "slot count only"},
    {13,
     "VALUE_SUPPLY_REDACTION_POLICY",
     "VALUE_DRAFT_REDACTION_BOUNDARY",
     "declare redaction rules before any operator value can appear",
     "redact-before-persist"},
    {14,
     "VALUE_SUPPLY_MISSING_VALUE_POLICY",
     "VALUE_DRAFT_ACTUAL_VALUE_STATE",
     "fail closed when an operator value is missing",
     "missing value marker only"},
    {15,
     "VALUE_SUPPLY_SOURCE_PROVENANCE_CONTRACT",
     "VALUE_DRAFT_IMPORT_STATE_BLOCKED",
     "require source evidence provenance for each future supplied value",
     "provenance id only"},
    {16,
     "VALUE_SUPPLY_NO_AUTOMATIC_SIBLING_IMPORT_AUDIT",
     "VALUE_DRAFT_NO_EVIDENCE_IMPORT_AUDIT",
     "audit that no sibling evidence is imported automatically",
     "no imported payload"},
    {17,
     "VALUE_SUPPLY_NO_MANUAL_ENTRY_AUDIT",
     "VALUE_DRAFT_NO_MANUAL_ENTRY_AUDIT",
     "audit that manual entry remains blocked from mini-kv storage",
     "no manual value"},
    {18,
     "VALUE_SUPPLY_NO_RUNTIME_PAYLOAD_AUDIT",
     "VALUE_DRAFT_NO_RUNTIME_PAYLOAD_AUDIT",
     "audit that runtime payloads are not accepted by the envelope",
     "no runtime payload"},
    {19,
     "VALUE_SUPPLY_NO_SYNTHETIC_EVIDENCE_AUDIT",
     "VALUE_DRAFT_NO_SYNTHETIC_EVIDENCE_AUDIT",
     "audit that synthetic evidence cannot become a supplied value",
     "reject synthetic value"},
    {20,
     "VALUE_SUPPLY_NO_SECRET_STORAGE_AUDIT",
     "VALUE_DRAFT_NO_SECRET_STORAGE_AUDIT",
     "audit that credentials, secrets, and raw endpoints are never stored",
     "secret-free envelope"},
    {21,
     "VALUE_SUPPLY_NO_ROUTER_AUDIT",
     "VALUE_DRAFT_NO_ROUTER_AUDIT",
     "audit that value supply installs no active shard router",
     "no router handle"},
    {22,
     "VALUE_SUPPLY_NO_WRITE_AUDIT",
     "VALUE_DRAFT_NO_WRITE_AUDIT",
     "audit that value supply routes no writes and touches no WAL",
     "no write record"},
    {23,
     "VALUE_SUPPLY_NO_EXECUTION_AUDIT",
     "VALUE_DRAFT_NO_EXECUTION_AUDIT",
     "audit that value supply cannot trigger restore, load, compact, or service execution",
     "no executable payload"},
    {24,
     "VALUE_SUPPLY_RELEASE_READINESS_AUDIT",
     "VALUE_DRAFT_RELEASE_READINESS_AUDIT",
     "audit release readiness while keeping the envelope disabled",
     "disabled release marker"},
    {25,
     "VALUE_SUPPLY_RELEASE_PACKAGE",
     "VALUE_DRAFT_RELEASE_PACKAGE",
     "package the disabled envelope for downstream controlled review",
     "read-only package marker"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_value_supply_slot_count() {
    return static_cast<int>(value_supply_slots.size());
}

std::string format_value_supply_slots_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_value_supply_slot_count());

    std::string response = "[";
    bool first = true;
    for (const auto& slot : value_supply_slots) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(slot.sequence) +
                    ",\"envelopeCode\":" + json_string(slot.envelope_code) +
                    ",\"sourceValueDraftCode\":" + json_string(slot.source_value_draft_code) +
                    ",\"provenanceRequirement\":" + json_string(slot.provenance_requirement) +
                    ",\"redactionRule\":" + json_string(slot.redaction_rule) +
                    ",\"publishedByCurrentStage\":" +
                    json_bool(slot.sequence <= clamped_published_count) +
                    ",\"envelopeState\":\"disabled-by-default\""
                    ",\"missingValuePolicy\":\"fail-closed\""
                    ",\"valueRedactionState\":\"redact-before-persist\""
                    ",\"sourceEvidenceProvenanceSupplied\":false"
                    ",\"operatorValueSupplied\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValueImported\":false"
                    ",\"operatorValuePersisted\":false"
                    ",\"automaticSiblingImportAllowed\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"readOnly\":true"
                    ",\"evidenceImportAllowed\":false"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_envelope_slots
