#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_operator_value_supply_profile_section_integrity_checks;

    const auto records = checks::operator_value_supply_profile_section_integrity_checks();
    assert(checks::planned_operator_value_supply_profile_section_integrity_check_count() == 10);
    assert(records.size() == 10);
    assert(records.front().check_code == std::string{"source-profile-section-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-value-chain-created"});

    const auto json = checks::format_operator_value_supply_profile_section_integrity_checks_json(10);
    assert_contains(json, "\"checkCode\":\"source-command-digest-pinned\"");
    assert_contains(json, "\"checkCode\":\"node-v1606-parallel-context\"");
    assert_contains(json, "\"integrityOnly\":true");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"freshSiblingEvidenceImported\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_operator_value_supply_profile_section_integrity_checks_json(11);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
