#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<OperatorValueSupplyProfileSectionIntegrityCheck, 10> check_catalog = {{
    {1, "source-profile-section-fixture-frozen", "fixtures/release/shard-readiness-v1160.json",
     "v1160 remains the immutable source value supply profile section fixture"},
    {2, "source-command-digest-pinned", "SHARDROUTEVALUESUPPLYPROFILESECTIONJSON",
     "the source command and digest marker are pinned before integrity closeout"},
    {3, "profile-section-count-lock", "v1160 four-section group",
     "operator value supply profile section count remains four"},
    {4, "source-lineage-lock", "v560 v585 v610 Node v936 markers",
     "source import draft envelope and fresh sibling markers remain metadata only"},
    {5, "operator-value-import-lock", "operator value and sibling evidence flags",
     "operator value and fresh sibling evidence import remain disabled"},
    {6, "runtime-secret-endpoint-lock", "runtime payload secret and raw endpoint fields",
     "runtime payload synthetic evidence secret value and raw endpoint parsing remain disabled"},
    {7, "node-v1606-parallel-context", "Node v1606 readiness packet module split",
     "Node parallel maintenance does not require fresh mini-kv evidence"},
    {8, "command-shardjson-exposure", "COMMANDS COMMANDSJSON HELP SHARDJSON",
     "integrity evidence is parser visible and embedded as read-only metadata"},
    {9, "validation-split", "operator value supply profile section integrity validation module",
     "validation stays separate from the evidence formatter"},
    {10, "no-value-chain-created", "profile section integrity closeout",
     "closeout does not open value import router write WAL or execution"},
}};

std::string format_check_json(const OperatorValueSupplyProfileSectionIntegrityCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"integrityOnly\":true"
           ",\"sourceFixtureFrozen\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorValueAccepted\":false"
           ",\"freshSiblingEvidenceImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"credentialValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"materialIntakeOpened\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const OperatorValueSupplyProfileSectionIntegrityCheck>
operator_value_supply_profile_section_integrity_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_operator_value_supply_profile_section_integrity_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_operator_value_supply_profile_section_integrity_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published operator value supply profile section integrity check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_checks
