#include "minikv/shard_preview_operator_value_supply_profile_section_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_operator_value_supply_profile_section_checks;

    const auto records = checks::operator_value_supply_profile_section_checks();
    assert(checks::planned_operator_value_supply_profile_section_check_count() == 25);
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"source-current-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-value-supply-profile-chain-created"});

    const auto json = checks::format_operator_value_supply_profile_section_checks_json(25);
    assert_contains(json, "\"checkCode\":\"value-supply-aggregator-boundary\"");
    assert_contains(json, "\"checkCode\":\"fresh-sibling-intake-section-carried\"");
    assert_contains(json, "\"valueSupplyProfileSectionOnly\":true");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"freshSiblingEvidenceImported\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_operator_value_supply_profile_section_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
