#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks {
namespace {

constexpr std::array<PrecheckUpstreamReceiptVerificationSplitNonParticipationCheck, 20> checks{{
    {1, "public-loader-render-exports-preserved-no-import", "Node v1983",
     "public loader and render exports remain Node-owned and are not imported by mini-kv"},
    {2, "types-module-no-import", "Node v1984",
     "profile receipt evidence snippet message and check types stay outside C++"},
    {3, "constants-module-no-import", "Node v1985",
     "fixed evidence paths endpoint paths and precheck shape constants are not imported by mini-kv"},
    {4, "node-v245-source-adapter-no-parse", "Node v1986",
     "Node v245 source adapter remains a Node reference and is not parsed by mini-kv"},
    {5, "java-v99-receipt-reference-no-scan", "Node v1987",
     "Java v99 receipt reference is not scanned from Java files by mini-kv"},
    {6, "minikv-v108-non-participation-reference-no-parse", "Node v1988",
     "mini-kv v108 non-participation reference is not reparsed from historical files"},
    {7, "reference-helpers-private-no-execution", "Node v1989",
     "evidence-file snippet JSON and field helpers remain private Node references"},
    {8, "check-policy-no-execution", "Node v1990",
     "check construction policy stays outside mini-kv execution"},
    {9, "message-policy-no-execution", "Node v1991",
     "blocker warning and recommendation policy stays outside mini-kv execution"},
    {10, "core-assembly-no-execution", "Node v1992",
     "digest receipt verification summary endpoint and next-action assembly stay outside mini-kv"},
    {11, "renderer-no-execution", "Node v1993",
     "Markdown rendering remains Node-owned and is not executed by mini-kv"},
    {12, "entrypoint-orchestration-no-execution", "Node v1994",
     "the split entrypoint orchestrates Node modules without becoming a C++ entrypoint"},
    {13, "code-health-scan-family-observed-only", "Node v1995",
     "code-health scan family updates are observed but not executed by mini-kv"},
    {14, "large-file-inventory-observed-only", "Node v1996",
     "large-file inventory cleanup is observed but not mutated by mini-kv"},
    {15, "direct-tests-fallback-observed-only", "Node v1997",
     "direct service tests and forced historical fallback are observed only"},
    {16, "downstream-code-health-observed-only", "Node v1998",
     "downstream code-health behavior remains a Node-owned check"},
    {17, "downstream-rehearsal-guard-observed-only", "Node v1999",
     "downstream rehearsal guard ready-state behavior remains a Node-owned check"},
    {18, "typecheck-build-observed-only", "Node v2000",
     "TypeScript typecheck and production build are observed sibling checks only"},
    {19, "node-closeout-observed-only", "Node v2001",
     "Node commit tag push and CI remain Node-owned"},
    {20, "clean-workspace-ci-closeout", "Node v2002 / mini-kv v1545",
     "cleanup and no local long-running process proof close the read-only mini-kv evidence"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const PrecheckUpstreamReceiptVerificationSplitNonParticipationCheck>
precheck_upstream_receipt_verification_split_non_participation_checks() {
    return checks;
}

int planned_precheck_upstream_receipt_verification_split_non_participation_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_precheck_upstream_receipt_verification_split_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds precheck upstream receipt verification split checks"};
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
                  ",\"precheckUpstreamReceiptVerificationSplitNonParticipationOnly\":true"
                  ",\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true"
                  ",\"nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv\":false"
                  ",\"nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv\":false"
                  ",\"nodeCodeHealthScanExecutedByMiniKv\":false"
                  ",\"nodeLargeFileInventoryMutatedByMiniKv\":false"
                  ",\"nodeDirectTestsExecutedByMiniKv\":false"
                  ",\"nodeDownstreamCodeHealthExecutedByMiniKv\":false"
                  ",\"nodeDownstreamRehearsalGuardExecutedByMiniKv\":false"
                  ",\"nodeTypecheckExecutedByMiniKv\":false"
                  ",\"nodeVitestExecutedByMiniKv\":false"
                  ",\"nodeBuildExecutedByMiniKv\":false"
                  ",\"miniKvImportsNodeModules\":false"
                  ",\"miniKvExecutesNodeChecks\":false"
                  ",\"miniKvStartsServices\":false"
                  ",\"miniKvReadsEndpoints\":false"
                  ",\"miniKvReadsCredentials\":false"
                  ",\"miniKvParsesHistoricalReceipts\":false"
                  ",\"miniKvReadsHistoricalMiniKvV108NonParticipationFile\":false"
                  ",\"miniKvScansJavaV99Evidence\":false"
                  ",\"miniKvScansJavaEvidence\":false"
                  ",\"miniKvExecutesDownstreamChecks\":false"
                  ",\"miniKvMutatesLargeFileInventory\":false"
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

} // namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks
