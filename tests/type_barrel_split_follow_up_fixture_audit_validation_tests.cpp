#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_validation;

    const auto passed = validation::format_type_barrel_split_follow_up_fixture_audit_validation_json(
        20, true, false, false, false, false, false, 12, 12, 12, 12);
    assert_contains(passed, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"auditClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
    assert_contains(passed, "\"miniKvImportsNodeModules\":false");
    assert_contains(passed, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
    assert_contains(passed, "\"profileBoundaryEndpointRead\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source = validation::format_type_barrel_split_follow_up_fixture_audit_validation_json(
        19, true, false, false, false, false, false, 12, 12, 12, 12);
    assert_contains(failed_source, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_closure = validation::format_type_barrel_split_follow_up_fixture_audit_validation_json(
        20, true, false, true, false, false, false, 12, 12, 12, 12);
    assert_contains(failed_closure, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":false");
    assert_contains(failed_closure, "\"auditClosed\":false");

    const auto failed_count = validation::format_type_barrel_split_follow_up_fixture_audit_validation_json(
        20, true, false, false, false, false, false, 12, 11, 12, 12);
    assert_contains(failed_count, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
