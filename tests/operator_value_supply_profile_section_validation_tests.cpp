#include "minikv/shard_preview_operator_value_supply_profile_section_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_operator_value_supply_profile_section_validation;

    const auto passed =
        validation::format_operator_value_supply_profile_section_validation_json(25, true, 25, true, 25, true, 4, 4,
                                                                                 false, false, false, 25, 25, 25, 25);
    assert_contains(passed, "\"operatorValueSupplyProfileSectionValidationPassed\":true");
    assert_contains(passed, "\"sourcesFrozen\":true");
    assert_contains(passed, "\"rendererSplitScopeAligned\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"operatorValueImported\":false");
    assert_contains(passed, "\"syntheticEvidenceAccepted\":false");
    assert_contains(passed, "\"secretValueRead\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_section_count =
        validation::format_operator_value_supply_profile_section_validation_json(25, true, 25, true, 25, true, 4, 3,
                                                                                 false, false, false, 25, 25, 25, 25);
    assert_contains(failed_section_count, "\"operatorValueSupplyProfileSectionValidationPassed\":false");
    assert_contains(failed_section_count, "\"rendererSplitScopeAligned\":false");

    const auto failed_source =
        validation::format_operator_value_supply_profile_section_validation_json(24, true, 25, true, 25, true, 4, 4,
                                                                                 false, false, false, 25, 25, 25, 25);
    assert_contains(failed_source, "\"operatorValueSupplyProfileSectionValidationPassed\":false");
    assert_contains(failed_source, "\"sourcesFrozen\":false");

    const auto failed_count =
        validation::format_operator_value_supply_profile_section_validation_json(25, true, 25, true, 25, true, 4, 4,
                                                                                 false, false, false, 25, 24, 25, 25);
    assert_contains(failed_count, "\"operatorValueSupplyProfileSectionValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
