#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks =
        minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks;

    const auto records = checks::implementation_plan_upstream_echo_closeout_non_participation_checks();
    assert(checks::planned_implementation_plan_upstream_echo_closeout_non_participation_check_count() == 8);
    assert(records.size() == 8);
    assert(records.front().check_code == std::string{"source-sandbox-endpoint-fixture-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty = checks::format_implementation_plan_upstream_echo_closeout_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_implementation_plan_upstream_echo_closeout_non_participation_checks_json(8);
    assert_contains(json, "\"checkCode\":\"source-sandbox-endpoint-fixture-freeze\"");
    assert_contains(json, "\"checkCode\":\"node-closeout-plan-intake\"");
    assert_contains(json, "\"checkCode\":\"ownership-map-metadata-only\"");
    assert_contains(json, "\"checkCode\":\"historical-sibling-reference-only\"");
    assert_contains(json, "\"checkCode\":\"endpoint-credential-boundary-closed\"");
    assert_contains(json, "\"checkCode\":\"router-write-wal-execution-disabled\"");
    assert_contains(json, "\"checkCode\":\"command-shardjson-fixture-parity\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationOnly\":true");
    assert_contains(json, "\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanChecksExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_implementation_plan_upstream_echo_closeout_non_participation_checks_json(9);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
