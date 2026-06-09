#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_operator_value_supply_profile_section_integrity_validation;

    const auto passed =
        validation::format_operator_value_supply_profile_section_integrity_validation_json(25, true, 4, 4, true, true,
                                                                                           false, false, 10, 10, 10,
                                                                                           10);
    assert_contains(passed, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"sectionCountsAligned\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"operatorValueImported\":false");
    assert_contains(passed, "\"freshSiblingEvidenceImported\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_operator_value_supply_profile_section_integrity_validation_json(24, true, 4, 4, true, true,
                                                                                           false, false, 10, 10, 10,
                                                                                           10);
    assert_contains(failed_source, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_section_count =
        validation::format_operator_value_supply_profile_section_integrity_validation_json(25, true, 4, 3, true, true,
                                                                                           false, false, 10, 10, 10,
                                                                                           10);
    assert_contains(failed_section_count, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":false");
    assert_contains(failed_section_count, "\"sectionCountsAligned\":false");

    const auto failed_count =
        validation::format_operator_value_supply_profile_section_integrity_validation_json(25, true, 4, 4, true, true,
                                                                                           false, false, 10, 9, 10,
                                                                                           10);
    assert_contains(failed_count, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
