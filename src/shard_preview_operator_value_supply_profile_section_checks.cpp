#include "minikv/shard_preview_operator_value_supply_profile_section_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_operator_value_supply_profile_section_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<OperatorValueSupplyProfileSectionCheck, 25> check_catalog = {{
    {1, "source-current-fixture-frozen", "fixtures/release/shard-readiness-v1135.json",
     "v1135 remains the immutable source current fixture"},
    {2, "node-no-new-evidence-chain-decision", "Node v1581 renderer split roadmap",
     "mini-kv treats the split as read-only ownership context"},
    {3, "value-supply-section-group-boundary", "Node v1581 four-section group",
     "only import preflight value draft fresh sibling intake and value supply envelope sections are represented"},
    {4, "main-renderer-delegation-boundary", "live read-only window profile sections renderer",
     "main renderer delegation is recorded without executing Node code"},
    {5, "value-supply-aggregator-boundary", "operator evidence value supply profile section renderer",
     "aggregator ownership is metadata only"},
    {6, "import-draft-renderer-boundary", "import preflight and value draft renderer split",
     "import preflight and value draft rendering ownership is metadata only"},
    {7, "sibling-envelope-renderer-boundary", "fresh sibling and value supply envelope renderer split",
     "fresh sibling intake and envelope rendering ownership is metadata only"},
    {8, "import-preflight-section-carried", "Node v886 import preflight marker",
     "operator evidence import preflight identity remains metadata only"},
    {9, "value-draft-section-carried", "Node v911 value draft marker",
     "operator evidence value draft identity remains metadata only"},
    {10, "fresh-sibling-intake-section-carried", "Node v936 fresh sibling intake marker",
     "fresh sibling intake identity remains plan metadata only"},
    {11, "value-supply-envelope-section-carried", "Node v961 value supply envelope marker",
     "value supply envelope identity remains metadata only"},
    {12, "runtime-payload-false-lock", "runtime payload false state",
     "runtime payload import and acceptance remain disabled"},
    {13, "synthetic-evidence-false-lock", "synthetic evidence false state",
     "synthetic evidence cannot become accepted operator value material"},
    {14, "secret-raw-endpoint-false-lock", "credential secret and raw endpoint fields",
     "credential secret and raw endpoint values remain unread and unparsed"},
    {15, "type-catalog-aggregator-order-lock", "Node v1581 catalog order 217",
     "value supply aggregator catalog order is recorded without import"},
    {16, "type-catalog-import-renderer-order-lock", "Node v1581 catalog order 218",
     "import renderer catalog order is recorded without import"},
    {17, "type-catalog-envelope-renderer-order-lock", "Node v1581 catalog order 219",
     "envelope renderer catalog order is recorded without import"},
    {18, "profile-entry-order-lock", "Node v1581 catalog order 220",
     "profile-entry-types catalog order is recorded without import"},
    {19, "catalog-validation-version-lock", "Node v1581 validation version",
     "catalog and validation context is metadata only"},
    {20, "route-markdown-parity", "Node v1581 route Markdown closeout",
     "mini-kv records parity without rendering Node Markdown"},
    {21, "main-renderer-size-control", "Node v1581 main renderer size reduction",
     "line-count reduction is maintenance context not a mini-kv execution dependency"},
    {22, "subrenderer-size-control", "Node v1581 subrenderer size control",
     "subrenderer sizing is metadata only"},
    {23, "parallel-status-recorded", "Node v1581 cross-project parallel plan",
     "Node does not require fresh mini-kv evidence in this range"},
    {24, "no-live-integration-startup", "live integration startup boundary",
     "Java mini-kv and sibling service startup remain out of scope"},
    {25, "no-value-supply-profile-chain-created", "value supply profile section closeout",
     "closeout does not open value import router write WAL or execution"},
}};

std::string format_check_json(const OperatorValueSupplyProfileSectionCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"valueSupplyProfileSectionOnly\":true"
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
           ",\"rawEndpointParsed\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const OperatorValueSupplyProfileSectionCheck> operator_value_supply_profile_section_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_operator_value_supply_profile_section_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_operator_value_supply_profile_section_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{"published operator value supply profile section check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_operator_value_supply_profile_section_checks
