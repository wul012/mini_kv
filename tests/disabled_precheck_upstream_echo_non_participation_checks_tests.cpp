#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_checks;

    const auto records = checks::disabled_precheck_upstream_echo_non_participation_checks();
    assert(checks::planned_disabled_precheck_upstream_echo_non_participation_check_count() == 25);
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"source-fixture-audit-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty = checks::format_disabled_precheck_upstream_echo_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_disabled_precheck_upstream_echo_non_participation_checks_json(25);
    assert_contains(json, "\"checkCode\":\"source-fixture-audit-freeze\"");
    assert_contains(json, "\"checkCode\":\"stable-barrel-no-import\"");
    assert_contains(json, "\"checkCode\":\"checks-messages-no-execution\"");
    assert_contains(json, "\"checkCode\":\"credential-secret-no-read\"");
    assert_contains(json, "\"checkCode\":\"wal-load-restore-compact-disabled\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationOnly\":true");
    assert_contains(json, "\"sourceFixtureAuditFrozen\":true");
    assert_contains(json, "\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckReferenceBuildersImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckChecksExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_disabled_precheck_upstream_echo_non_participation_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
