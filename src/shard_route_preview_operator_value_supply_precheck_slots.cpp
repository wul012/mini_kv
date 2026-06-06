#include "minikv/shard_route_preview_operator_value_supply_precheck_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_precheck_slots {
namespace {

struct ValueSupplyPrecheckSlot {
    int sequence;
    std::string_view precheck_code;
    std::string_view source_envelope_code;
    std::string_view approval_input_requirement;
    std::string_view rejection_rule;
};

constexpr std::array<ValueSupplyPrecheckSlot, 25> value_supply_precheck_slots = {{
    {1,
     "VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT",
     "VALUE_SUPPLY_ENVELOPE_POLICY_SPLIT",
     "approval packet contract catalog",
     "no supplied value accepted"},
    {2,
     "VALUE_SUPPLY_PRECHECK_SOURCE_ENVELOPE_CONTRACT",
     "VALUE_SUPPLY_SOURCE_VALUE_DRAFT_CONTRACT",
     "source envelope release id",
     "no runtime source envelope accepted"},
    {3,
     "VALUE_SUPPLY_PRECHECK_SOURCE_RELEASE_FREEZE",
     "VALUE_SUPPLY_SOURCE_RELEASE_FREEZE",
     "versioned source fixture path",
     "no rolling current as source"},
    {4,
     "VALUE_SUPPLY_PRECHECK_STAGE_CHAIN_AUDIT",
     "VALUE_SUPPLY_STAGE_CHAIN_AUDIT",
     "stage chain report",
     "no approval on incomplete chain"},
    {5,
     "VALUE_SUPPLY_PRECHECK_FIXTURE_CHAIN_AUDIT",
     "VALUE_SUPPLY_FIXTURE_CHAIN_AUDIT",
     "fixture chain report",
     "no runtime file read"},
    {6,
     "VALUE_SUPPLY_PRECHECK_COMMAND_CATALOG_CONTRACT",
     "VALUE_SUPPLY_COMMAND_CATALOG_CONTRACT",
     "command catalog metadata",
     "no write command"},
    {7,
     "VALUE_SUPPLY_PRECHECK_EXPLAIN_CONTRACT",
     "VALUE_SUPPLY_EXPLAIN_CONTRACT",
     "EXPLAINJSON risk metadata",
     "no approval authority"},
    {8,
     "VALUE_SUPPLY_PRECHECK_CHECKJSON_CONTRACT",
     "VALUE_SUPPLY_CHECKJSON_CONTRACT",
     "CHECKJSON parser metadata",
     "no execution contract"},
    {9,
     "VALUE_SUPPLY_PRECHECK_SHARDJSON_EMBEDDING",
     "VALUE_SUPPLY_SHARDJSON_EMBEDDING",
     "single SHARDJSON section",
     "no duplicate section"},
    {10,
     "VALUE_SUPPLY_PRECHECK_SHARDJSON_EMBEDDING_AUDIT",
     "VALUE_SUPPLY_SHARDJSON_EMBEDDING_AUDIT",
     "section count audit",
     "no aggregate mutation"},
    {11,
     "VALUE_SUPPLY_PRECHECK_SLOT_REQUIREMENT_CONTRACT",
     "VALUE_SUPPLY_SLOT_POLICY_CONTRACT",
     "slot requirement map",
     "no slot payload"},
    {12,
     "VALUE_SUPPLY_PRECHECK_SLOT_COUNT_AUDIT",
     "VALUE_SUPPLY_SLOT_COUNT_AUDIT",
     "slot count equality",
     "no partial slot map"},
    {13,
     "VALUE_SUPPLY_PRECHECK_PROVENANCE_POLICY",
     "VALUE_SUPPLY_SOURCE_PROVENANCE_CONTRACT",
     "per-value provenance id",
     "reject missing provenance"},
    {14,
     "VALUE_SUPPLY_PRECHECK_REDACTION_POLICY",
     "VALUE_SUPPLY_REDACTION_POLICY",
     "redaction before persist",
     "reject raw value"},
    {15,
     "VALUE_SUPPLY_PRECHECK_MISSING_MALFORMED_POLICY",
     "VALUE_SUPPLY_MISSING_VALUE_POLICY",
     "missing and malformed value rejection",
     "reject invalid value"},
    {16,
     "VALUE_SUPPLY_PRECHECK_APPROVAL_IDENTITY_FIELDS",
     "VALUE_SUPPLY_SOURCE_PROVENANCE_CONTRACT",
     "operator approval identity",
     "reject absent operator identity"},
    {17,
     "VALUE_SUPPLY_PRECHECK_APPROVAL_TIMESTAMP_FIELDS",
     "VALUE_SUPPLY_SOURCE_PROVENANCE_CONTRACT",
     "approval timestamp",
     "reject absent approval timestamp"},
    {18,
     "VALUE_SUPPLY_PRECHECK_NO_AUTOMATIC_IMPORT_AUDIT",
     "VALUE_SUPPLY_NO_AUTOMATIC_SIBLING_IMPORT_AUDIT",
     "explicit import denial",
     "no automatic sibling import"},
    {19,
     "VALUE_SUPPLY_PRECHECK_NO_RUNTIME_PAYLOAD_AUDIT",
     "VALUE_SUPPLY_NO_RUNTIME_PAYLOAD_AUDIT",
     "runtime payload denial",
     "no runtime payload"},
    {20,
     "VALUE_SUPPLY_PRECHECK_NO_SECRET_STORAGE_AUDIT",
     "VALUE_SUPPLY_NO_SECRET_STORAGE_AUDIT",
     "secret-free evidence",
     "no secret storage"},
    {21,
     "VALUE_SUPPLY_PRECHECK_NO_ROUTER_AUDIT",
     "VALUE_SUPPLY_NO_ROUTER_AUDIT",
     "router denial",
     "no router handle"},
    {22,
     "VALUE_SUPPLY_PRECHECK_NO_WRITE_AUDIT",
     "VALUE_SUPPLY_NO_WRITE_AUDIT",
     "write denial",
     "no WAL record"},
    {23,
     "VALUE_SUPPLY_PRECHECK_NO_EXECUTION_AUDIT",
     "VALUE_SUPPLY_NO_EXECUTION_AUDIT",
     "execution denial",
     "no executable payload"},
    {24,
     "VALUE_SUPPLY_PRECHECK_RELEASE_READINESS_AUDIT",
     "VALUE_SUPPLY_RELEASE_READINESS_AUDIT",
     "release readiness audit",
     "no approval grant"},
    {25,
     "VALUE_SUPPLY_PRECHECK_RELEASE_PACKAGE",
     "VALUE_SUPPLY_RELEASE_PACKAGE",
     "disabled precheck package",
     "no production activation"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_value_supply_precheck_slot_count() {
    return static_cast<int>(value_supply_precheck_slots.size());
}

std::string format_value_supply_precheck_slots_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_value_supply_precheck_slot_count());

    std::string response = "[";
    bool first = true;
    for (const auto& slot : value_supply_precheck_slots) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(slot.sequence) +
                    ",\"precheckCode\":" + json_string(slot.precheck_code) +
                    ",\"sourceEnvelopeCode\":" + json_string(slot.source_envelope_code) +
                    ",\"approvalInputRequirement\":" + json_string(slot.approval_input_requirement) +
                    ",\"rejectionRule\":" + json_string(slot.rejection_rule) +
                    ",\"publishedByCurrentStage\":" +
                    json_bool(slot.sequence <= clamped_published_count) +
                    ",\"approvalPacketRequired\":true"
                    ",\"approvalPacketPresent\":false"
                    ",\"operatorIdentityRequired\":true"
                    ",\"operatorIdentityPresent\":false"
                    ",\"approvalTimestampRequired\":true"
                    ",\"approvalTimestampPresent\":false"
                    ",\"perValueProvenanceRequired\":true"
                    ",\"perValueProvenanceSupplied\":false"
                    ",\"missingValueRejected\":true"
                    ",\"malformedValueRejected\":true"
                    ",\"automaticSiblingImportAllowed\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValuesPersisted\":false"
                    ",\"readOnly\":true"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_precheck_slots
