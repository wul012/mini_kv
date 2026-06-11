#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks {
namespace {

constexpr std::array<DisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheck, 15> checks{{
    {1, "public-entrypoint-preserved-no-import", "Node v1953",
     "public loader path remains Node-owned and is not imported by mini-kv"},
    {2, "constants-module-no-import", "Node v1954",
     "route plan evidence path and contract shape constants stay outside C++"},
    {3, "node-v288-projection-no-execution", "Node v1955",
     "Node v288 disabled fake harness projection is not executed by mini-kv"},
    {4, "java-v122-v126-reference-only", "Node v1956",
     "Java v122-v126 evidence construction is historical metadata only"},
    {5, "minikv-v127-receipt-reference-only", "Node v1957",
     "mini-kv v127 non-participation receipt is referenced without parsing runtime files"},
    {6, "expected-snippet-builders-no-execution", "Node v1958",
     "Java and mini-kv expected snippet builders stay Node-only"},
    {7, "reference-helper-no-execution", "Node v1959",
     "evidence parsing helpers stay in Node references module"},
    {8, "core-assembly-no-execution", "Node v1960",
     "echo verification assembly is not executed by mini-kv"},
    {9, "summary-aggregation-no-execution", "Node v1961",
     "summary aggregation remains Node core behavior"},
    {10, "readiness-policy-no-execution", "Node v1962",
     "readiness checks do not open managed audit connection from mini-kv"},
    {11, "blocker-policy-no-execution", "Node v1963",
     "blocker message collection remains Node policy only"},
    {12, "warning-recommendation-policy-no-execution", "Node v1964",
     "warnings and recommendations remain Node policy only"},
    {13, "focused-tests-observed-only", "Node v1965",
     "focused upstream echo and execution-denied preflight tests are observed but not run by mini-kv"},
    {14, "typecheck-build-observed-only", "Node v1966",
     "TypeScript typecheck and build are observed sibling checks only"},
    {15, "archive-cleanup-ci-closeout", "Node v1967",
     "archives cleanup commit tag push and CI stay Node-owned"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheck>
disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks() {
    return checks;
}

int planned_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds disabled fake harness split checks"};
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
                  ",\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationOnly\":true"
                  ",\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false"
                  ",\"nodeDisabledFakeHarnessExecutionDeniedRoutePreflightExecutedByMiniKv\":false"
                  ",\"nodeTypecheckExecutedByMiniKv\":false"
                  ",\"nodeVitestExecutedByMiniKv\":false"
                  ",\"nodeBuildExecutedByMiniKv\":false"
                  ",\"miniKvImportsNodeModules\":false"
                  ",\"miniKvExecutesNodeChecks\":false"
                  ",\"miniKvStartsServices\":false"
                  ",\"miniKvReadsEndpoints\":false"
                  ",\"miniKvReadsCredentials\":false"
                  ",\"miniKvParsesHistoricalReceipts\":false"
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

} // namespace minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks
