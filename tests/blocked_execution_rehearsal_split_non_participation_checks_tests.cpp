#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_checks.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    if (text.find(needle) == std::string::npos) {
        std::cerr << "missing expected text: " << needle << '\n';
    }
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks =
        minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_checks;

    const auto records =
        checks::blocked_execution_rehearsal_split_non_participation_checks();
    assert(checks::planned_blocked_execution_rehearsal_split_non_participation_check_count() == 20);
    assert(records.size() == 20);
    assert(records.front().check_code == std::string{"public-entrypoint-preserved-no-import"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty =
        checks::format_blocked_execution_rehearsal_split_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json =
        checks::format_blocked_execution_rehearsal_split_non_participation_checks_json(20);
    assert_contains(json, "\"checkCode\":\"public-entrypoint-preserved-no-import\"");
    assert_contains(json, "\"checkCode\":\"types-module-no-import\"");
    assert_contains(json, "\"checkCode\":\"constants-module-no-import\"");
    assert_contains(json, "\"checkCode\":\"historical-reference-no-parse\"");
    assert_contains(json, "\"checkCode\":\"java-v90-reference-no-scan\"");
    assert_contains(json, "\"checkCode\":\"minikv-v99-wal-regression-no-replay\"");
    assert_contains(json, "\"checkCode\":\"renderer-no-execution\"");
    assert_contains(json, "\"checkCode\":\"digest-core-no-execution\"");
    assert_contains(json, "\"checkCode\":\"summary-core-no-execution\"");
    assert_contains(json, "\"checkCode\":\"attempt-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"message-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"focused-route-tests-observed-only\"");
    assert_contains(json, "\"checkCode\":\"downstream-intake-observed-only\"");
    assert_contains(json, "\"checkCode\":\"typecheck-build-observed-only\"");
    assert_contains(json, "\"checkCode\":\"node-closeout-observed-only\"");
    assert_contains(json, "\"checkCode\":\"minikv-v99-non-parse-lock\"");
    assert_contains(json, "\"checkCode\":\"java-v90-non-scan-lock\"");
    assert_contains(json, "\"checkCode\":\"downstream-no-execution-lock\"");
    assert_contains(json, "\"checkCode\":\"command-fixture-smoke-boundary\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationOnly\":true");
    assert_contains(json, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePreconditionIntakeExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvParsesHistoricalReceipts\":false");
    assert_contains(json, "\"miniKvScansJavaEvidence\":false");
    assert_contains(json, "\"miniKvReplaysWalRegression\":false");
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
        (void)checks::format_blocked_execution_rehearsal_split_non_participation_checks_json(21);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        (void)checks::format_blocked_execution_rehearsal_split_non_participation_checks_json(-1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
