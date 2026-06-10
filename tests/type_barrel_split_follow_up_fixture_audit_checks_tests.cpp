#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_checks;

    const auto records = checks::type_barrel_split_follow_up_fixture_audit_checks();
    assert(checks::planned_type_barrel_split_follow_up_fixture_audit_check_count() == 12);
    assert(records.size() == 12);
    assert(records.front().check_code == std::string{"source-follow-up-fixture-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout-audit"});

    const auto empty = checks::format_type_barrel_split_follow_up_fixture_audit_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_type_barrel_split_follow_up_fixture_audit_checks_json(12);
    assert_contains(json, "\"checkCode\":\"source-follow-up-fixture-freeze\"");
    assert_contains(json, "\"checkCode\":\"current-fixture-parity-audit\"");
    assert_contains(json, "\"checkCode\":\"node-test-only-shell-no-import\"");
    assert_contains(json, "\"checkCode\":\"no-router-write-wal-audit\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout-audit\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditOnly\":true");
    assert_contains(json, "\"sourceFollowUpNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCredentialResolverShellExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_type_barrel_split_follow_up_fixture_audit_checks_json(13);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
