#include "minikv/shard_route_preview_operator_value_supply_approval_template_fields.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_approval_template_fields {
namespace {

struct ApprovalTemplateField {
    int sequence;
    std::string_view field_code;
    std::string_view template_field;
    std::string_view requirement;
    std::string_view withheld_value;
};

constexpr std::array<ApprovalTemplateField, 25> approval_template_fields = {{
    {1,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_FIELD_CATALOG_SPLIT",
     "approval_packet_contract_catalog",
     "keep packet draft fields separate from the precheck requirement catalog",
     "operator_value"},
    {2,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SOURCE_PRECHECK_CONTRACT",
     "source_precheck_release_id",
     "require the v635 disabled precheck package as source evidence",
     "runtime_precheck_payload"},
    {3,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SOURCE_RELEASE_FREEZE",
     "source_precheck_fixture_path",
     "pin v635 as a versioned fixture before rolling current approval-template evidence",
     "rolling_current_reference"},
    {4,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_STAGE_CHAIN_AUDIT",
     "approval_template_stage_chain",
     "audit stage continuity before any future approval packet can be accepted",
     "approval_grant"},
    {5,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_FIXTURE_CHAIN_AUDIT",
     "approval_template_fixture_chain",
     "audit frozen fixture continuity without runtime filesystem reads",
     "runtime_file_read"},
    {6,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_COMMAND_CATALOG_CONTRACT",
     "command_catalog_metadata",
     "publish the approval-template command as metadata read only",
     "write_command"},
    {7,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_EXPLAIN_CONTRACT",
     "explain_metadata",
     "classify approval-template inspection as metadata_read only",
     "execution_authority"},
    {8,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_CHECKJSON_CONTRACT",
     "checkjson_metadata",
     "prove CHECKJSON denies execution for the template command",
     "payload_acceptance"},
    {9,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SHARDJSON_EMBEDDING",
     "single_shardjson_section",
     "embed exactly one approval-template section in SHARDJSON",
     "duplicate_template_section"},
    {10,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SHARDJSON_EMBEDDING_AUDIT",
     "single_shardjson_section_audit",
     "audit aggregate embedding without mutating shard state",
     "aggregate_mutation"},
    {11,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SLOT_FIELD_CONTRACT",
     "slot_field_contract",
     "map each value-supply slot to approval-template fields",
     "slot_payload"},
    {12,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_SLOT_COUNT_AUDIT",
     "slot_count_audit",
     "keep approval-template slots aligned with precheck slots",
     "partial_slot_map"},
    {13,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_IDENTITY_FIELD_POLICY",
     "operator_identity",
     "require a future human operator identity while keeping it absent",
     "operator_identity"},
    {14,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_TIMESTAMP_FIELD_POLICY",
     "approval_timestamp",
     "require a future approval timestamp while keeping it absent",
     "approval_timestamp"},
    {15,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_CORRELATION_ID_POLICY",
     "approval_correlation_id",
     "require a future correlation id before approval capture",
     "approval_correlation_id"},
    {16,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_REDACTION_POLICY",
     "redacted_value_digest",
     "require redacted digests before any value could be persisted elsewhere",
     "raw_value"},
    {17,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_VALUE_INGEST_AUDIT",
     "typed_value_envelope",
     "audit that no operator value is ingested by the template",
     "typed_value_payload"},
    {18,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_AUTOMATIC_IMPORT_AUDIT",
     "sibling_import_denial",
     "audit that sibling evidence is not imported automatically",
     "sibling_import_payload"},
    {19,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_RUNTIME_PAYLOAD_AUDIT",
     "runtime_payload_denial",
     "audit that runtime payloads are not accepted",
     "runtime_payload"},
    {20,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_SECRET_STORAGE_AUDIT",
     "secret_storage_denial",
     "audit that secrets, credentials, and raw endpoints are never stored",
     "credential_value"},
    {21,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_ROUTER_AUDIT",
     "router_denial",
     "audit that approval-template evidence installs no active shard router",
     "router_handle"},
    {22,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_WRITE_AUDIT",
     "write_denial",
     "audit that approval-template evidence routes no writes and touches no WAL",
     "wal_record"},
    {23,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_NO_EXECUTION_AUDIT",
     "execution_denial",
     "audit that restore, load, compact, routing, and services cannot execute",
     "executable_payload"},
    {24,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_RELEASE_READINESS_AUDIT",
     "release_readiness",
     "audit release readiness while approval inputs remain absent",
     "approval_grant"},
    {25,
     "VALUE_SUPPLY_APPROVAL_TEMPLATE_RELEASE_PACKAGE",
     "release_package",
     "package final approval-template evidence for downstream review",
     "production_activation"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

int planned_approval_template_field_count() {
    return static_cast<int>(approval_template_fields.size());
}

std::string format_approval_template_fields_json(int published_stage_count) {
    const int clamped_published_count =
        std::clamp(published_stage_count, 0, planned_approval_template_field_count());

    std::string response = "[";
    bool first = true;
    for (const auto& field : approval_template_fields) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"sequence\":" + std::to_string(field.sequence) +
                    ",\"fieldCode\":" + json_string(field.field_code) +
                    ",\"templateField\":" + json_string(field.template_field) +
                    ",\"requirement\":" + json_string(field.requirement) +
                    ",\"withheldValue\":" + json_string(field.withheld_value) +
                    ",\"publishedByCurrentStage\":" +
                    json_bool(field.sequence <= clamped_published_count) +
                    ",\"templateOnly\":true"
                    ",\"requiredBeforeApproval\":true"
                    ",\"presentInMiniKv\":false"
                    ",\"approvalPacketPresent\":false"
                    ",\"approvalPacketAccepted\":false"
                    ",\"operatorValueAccepted\":false"
                    ",\"operatorValuesPersisted\":false"
                    ",\"automaticSiblingImportAllowed\":false"
                    ",\"runtimePayloadAccepted\":false"
                    ",\"readOnly\":true"
                    ",\"executionAllowed\":false}";
    }
    response += "]";
    return response;
}

} // namespace minikv::shard_route_preview_operator_value_supply_approval_template_fields
