#include "minikv/shard_route_preview_operator_value_supply_precheck_requirements.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_precheck_requirements {
namespace {

struct PrecheckRequirement {
    int sequence;
    std::string_view requirement_code;
    std::string_view policy;
    std::string_view failure_mode;
    std::string_view withheld_field;
};

constexpr std::array<PrecheckRequirement, 25> precheck_requirements = {{
    {1,
     "VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT",
     "split approval packet requirements away from the value-supply envelope",
     "approval packet absent",
     "operator_supplied_value"},
    {2,
     "VALUE_SUPPLY_PRECHECK_SOURCE_ENVELOPE_CONTRACT",
     "require v610 value-supply envelope evidence before approval review",
     "source envelope not accepted as runtime input",
     "operator_runtime_payload"},
    {3,
     "VALUE_SUPPLY_PRECHECK_SOURCE_RELEASE_FREEZE",
     "pin v610 as versioned source fixture before rolling current evidence",
     "current fixture is not treated as source history",
     "rolling_current_reference"},
    {4,
     "VALUE_SUPPLY_PRECHECK_STAGE_CHAIN_AUDIT",
     "audit precheck stage continuity through shared stage-chain helpers",
     "stage chain incomplete",
     "activation_decision"},
    {5,
     "VALUE_SUPPLY_PRECHECK_FIXTURE_CHAIN_AUDIT",
     "audit fixture continuity without reading the filesystem at runtime",
     "fixture chain incomplete",
     "runtime_file_read"},
    {6,
     "VALUE_SUPPLY_PRECHECK_COMMAND_CATALOG_CONTRACT",
     "publish command catalog metadata for the disabled precheck command",
     "command remains metadata read only",
     "write_command"},
    {7,
     "VALUE_SUPPLY_PRECHECK_EXPLAIN_CONTRACT",
     "explain the precheck command as metadata_read only",
     "risk profile does not authorize execution",
     "execution_authority"},
    {8,
     "VALUE_SUPPLY_PRECHECK_CHECKJSON_CONTRACT",
     "check the precheck command without importing values",
     "check result is evidence only",
     "payload_acceptance"},
    {9,
     "VALUE_SUPPLY_PRECHECK_SHARDJSON_EMBEDDING",
     "embed one precheck section in SHARDJSON",
     "duplicate section rejected by tests",
     "duplicate_precheck_section"},
    {10,
     "VALUE_SUPPLY_PRECHECK_SHARDJSON_EMBEDDING_AUDIT",
     "audit single-section embedding in the aggregate evidence",
     "section count mismatch",
     "aggregate_mutation"},
    {11,
     "VALUE_SUPPLY_PRECHECK_SLOT_REQUIREMENT_CONTRACT",
     "map each precheck slot to a withheld approval requirement",
     "slot requirement absent",
     "slot_payload"},
    {12,
     "VALUE_SUPPLY_PRECHECK_SLOT_COUNT_AUDIT",
     "audit precheck slot count against the source envelope slot count",
     "slot count mismatch",
     "partial_slot_map"},
    {13,
     "VALUE_SUPPLY_PRECHECK_PROVENANCE_POLICY",
     "require per-value provenance before any operator value can be considered",
     "provenance missing",
     "source_provenance"},
    {14,
     "VALUE_SUPPLY_PRECHECK_REDACTION_POLICY",
     "require redaction before any value could be persisted elsewhere",
     "redaction absent",
     "raw_value"},
    {15,
     "VALUE_SUPPLY_PRECHECK_MISSING_MALFORMED_POLICY",
     "reject missing or malformed supplied values",
     "missing or malformed value rejected",
     "invalid_value"},
    {16,
     "VALUE_SUPPLY_PRECHECK_APPROVAL_IDENTITY_FIELDS",
     "require operator approval identity fields while keeping them absent",
     "operator identity absent",
     "operator_identity"},
    {17,
     "VALUE_SUPPLY_PRECHECK_APPROVAL_TIMESTAMP_FIELDS",
     "require approval timestamp fields while keeping them absent",
     "approval timestamp absent",
     "approval_timestamp"},
    {18,
     "VALUE_SUPPLY_PRECHECK_NO_AUTOMATIC_IMPORT_AUDIT",
     "audit that no sibling evidence is imported automatically",
     "automatic import blocked",
     "sibling_import_payload"},
    {19,
     "VALUE_SUPPLY_PRECHECK_NO_RUNTIME_PAYLOAD_AUDIT",
     "audit that runtime payloads are not accepted by the precheck",
     "runtime payload blocked",
     "runtime_payload"},
    {20,
     "VALUE_SUPPLY_PRECHECK_NO_SECRET_STORAGE_AUDIT",
     "audit that credentials, secrets, and raw endpoints are never stored",
     "secret storage blocked",
     "credential_value"},
    {21,
     "VALUE_SUPPLY_PRECHECK_NO_ROUTER_AUDIT",
     "audit that the precheck installs no active shard router",
     "router activation blocked",
     "router_handle"},
    {22,
     "VALUE_SUPPLY_PRECHECK_NO_WRITE_AUDIT",
     "audit that the precheck routes no writes and touches no WAL",
     "write routing blocked",
     "wal_record"},
    {23,
     "VALUE_SUPPLY_PRECHECK_NO_EXECUTION_AUDIT",
     "audit that the precheck cannot trigger restore, load, compact, routing, or service execution",
     "execution blocked",
     "executable_payload"},
    {24,
     "VALUE_SUPPLY_PRECHECK_RELEASE_READINESS_AUDIT",
     "audit release readiness while approval inputs remain absent",
     "release is evidence only",
     "approval_grant"},
    {25,
     "VALUE_SUPPLY_PRECHECK_RELEASE_PACKAGE",
     "package final disabled precheck evidence for downstream review",
     "package cannot approve execution",
     "production_activation"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_precheck_requirement_count() {
    return static_cast<int>(precheck_requirements.size());
}

std::string format_precheck_requirements_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_precheck_requirement_count());

    std::string response = "[";
    bool first = true;
    for (const auto& requirement : precheck_requirements) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(requirement.sequence) +
                    ",\"requirementCode\":" + json_string(requirement.requirement_code) +
                    ",\"policy\":" + json_string(requirement.policy) +
                    ",\"failureMode\":" + json_string(requirement.failure_mode) +
                    ",\"withheldField\":" + json_string(requirement.withheld_field) +
                    ",\"publishedByCurrentStage\":" +
                    json_bool(requirement.sequence <= clamped_published_count) +
                    ",\"requiredBeforeApproval\":true"
                    ",\"presentInMiniKv\":false"
                    ",\"approvalPacketPresent\":false"
                    ",\"precheckOnly\":true"
                    ",\"readOnly\":true"
                    ",\"automaticSiblingImportAllowed\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_precheck_requirements
