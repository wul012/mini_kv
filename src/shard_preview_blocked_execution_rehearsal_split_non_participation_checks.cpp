#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_checks {
namespace {

constexpr std::array<BlockedExecutionRehearsalSplitNonParticipationCheck, 20> checks{{
    {1, "public-entrypoint-preserved-no-import", "Node v1968",
     "blocked execution rehearsal public loader remains Node-owned and is not imported by mini-kv"},
    {2, "types-module-no-import", "Node v1969",
     "profile reference evidence attempt check message and receipt types stay outside C++"},
    {3, "constants-module-no-import", "Node v1970",
     "path endpoint digest and accepted mini-kv reference constants are not imported by mini-kv"},
    {4, "historical-reference-no-parse", "Node v1971",
     "historical evidence file and snippet collection remain Node-owned"},
    {5, "java-v90-reference-no-scan", "Node v1972",
     "Java v90 context-normalization evidence is referenced without scanning sibling files"},
    {6, "minikv-v99-wal-regression-no-replay", "Node v1973",
     "mini-kv v99 WAL regression evidence is referenced without replay or file parsing"},
    {7, "renderer-no-execution", "Node v1974",
     "Markdown rendering remains Node-owned and is not executed by mini-kv"},
    {8, "digest-core-no-execution", "Node v1975",
     "digest assembly core remains Node-owned"},
    {9, "summary-core-no-execution", "Node v1976",
     "summary aggregation core remains Node-owned"},
    {10, "attempt-policy-no-execution", "Node v1977",
     "simulated blocked attempts and readiness checks stay outside mini-kv"},
    {11, "message-policy-no-execution", "Node v1978",
     "blocker warning and recommendation policy stays outside mini-kv"},
    {12, "focused-route-tests-observed-only", "Node v1979",
     "focused blocked rehearsal and route output tests are observed but not run by mini-kv"},
    {13, "downstream-intake-observed-only", "Node v1980",
     "downstream precondition intake consumption is observed but not executed by mini-kv"},
    {14, "typecheck-build-observed-only", "Node v1981",
     "TypeScript typecheck and build are observed sibling checks only"},
    {15, "node-closeout-observed-only", "Node v1982",
     "Node archive cleanup commit tag push and CI remain Node-owned"},
    {16, "minikv-v99-non-parse-lock", "mini-kv v1521",
     "mini-kv does not parse historical v99 WAL regression fixture or runtime output"},
    {17, "java-v90-non-scan-lock", "mini-kv v1522",
     "mini-kv does not scan Java v90 context-normalization evidence files"},
    {18, "downstream-no-execution-lock", "mini-kv v1523",
     "mini-kv does not execute downstream precondition intake or blocked rehearsal behavior"},
    {19, "command-fixture-smoke-boundary", "mini-kv v1524",
     "command and fixture smoke prove read-only exposure without service startup side effects"},
    {20, "clean-workspace-ci-closeout", "mini-kv v1525",
     "archive cleanup commit tag push and CI close the evidence without opening execution authority"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const BlockedExecutionRehearsalSplitNonParticipationCheck>
blocked_execution_rehearsal_split_non_participation_checks() {
    return checks;
}

int planned_blocked_execution_rehearsal_split_non_participation_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_blocked_execution_rehearsal_split_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds blocked execution rehearsal split checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"source\":" + json_string(check.source) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"blockedExecutionRehearsalSplitNonParticipationOnly\":true"
                  ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true"
                  ",\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false"
                  ",\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false"
                  ",\"nodePreconditionIntakeExecutedByMiniKv\":false"
                  ",\"nodeTypecheckExecutedByMiniKv\":false"
                  ",\"nodeVitestExecutedByMiniKv\":false"
                  ",\"nodeBuildExecutedByMiniKv\":false"
                  ",\"miniKvImportsNodeModules\":false"
                  ",\"miniKvExecutesNodeChecks\":false"
                  ",\"miniKvStartsServices\":false"
                  ",\"miniKvReadsEndpoints\":false"
                  ",\"miniKvReadsCredentials\":false"
                  ",\"miniKvParsesHistoricalReceipts\":false"
                  ",\"miniKvScansJavaEvidence\":false"
                  ",\"miniKvReplaysWalRegression\":false"
                  ",\"endpointHandleRead\":false"
                  ",\"rawEndpointParsed\":false"
                  ",\"credentialValueRead\":false"
                  ",\"managedAuditConnectionOpened\":false"
                  ",\"activeRouterInstalled\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"loadRestoreCompactAllowed\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_checks
