#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<SandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheck, 30> check_catalog = {{
    {1, "source-disabled-precheck-freeze", "mini-kv v1442", "v1442 disabled-precheck upstream echo non-participation remains the frozen source"},
    {2, "node-plan-intake", "Node v1879-v1903", "Node sandbox endpoint credential resolver upstream echo work is treated as module split maintenance only"},
    {3, "command-catalog-read-only", "mini-kv COMMANDS and COMMANDSJSON", "the new command appears only as read-only metadata"},
    {4, "current-shardjson-rollover", "mini-kv SHARDJSON", "current shard readiness status rolls forward without enabling execution"},
    {5, "versioned-fixture-chain-continuity", "mini-kv fixtures", "every release keeps contiguous versioned fixture pointers"},
    {6, "stable-barrel-no-import", "Node service entrypoint", "mini-kv does not import the stable service barrel"},
    {7, "route-constant-no-import", "Node constants module", "mini-kv does not import Node route paths evidence paths or comparison catalogs"},
    {8, "node-reference-no-import", "Node v260 reference builder", "mini-kv does not import Node source reference builders"},
    {9, "java-reference-no-import", "Java v105 reference builder", "mini-kv does not import Java reference builder logic"},
    {10, "historical-minikv-reference-frozen", "Node mini-kv v114 reference", "Node consumes historical mini-kv v114 while current mini-kv remains independently versioned"},
    {11, "historical-helper-no-execution", "Node historical fallback helpers", "mini-kv does not execute ordered comparisons or fallback helpers"},
    {12, "readiness-check-no-execution", "Node readiness checks", "mini-kv does not execute upstream echo readiness checks"},
    {13, "blocker-message-no-execution", "Node blockers warnings recommendations", "mini-kv does not execute blocker warning or recommendation builders"},
    {14, "core-loader-no-execution", "Node core loader", "mini-kv does not execute digest or profile assembly loaders"},
    {15, "renderer-export-stability", "Node renderer export", "renderer export stability remains a Node-owned contract"},
    {16, "typecheck-boundary", "Node typecheck", "mini-kv does not run Node typecheck"},
    {17, "build-vitest-boundary", "Node build and Vitest", "mini-kv does not run Node build or Vitest"},
    {18, "no-service-startup", "cross-project startup boundary", "Node plan requires no Java or mini-kv sibling service startup"},
    {19, "endpoint-handle-no-read", "endpoint handle boundary", "mini-kv does not read endpoint handles"},
    {20, "raw-endpoint-no-parse", "raw endpoint boundary", "mini-kv does not parse raw endpoint URLs"},
    {21, "credential-secret-no-read", "credential boundary", "mini-kv does not read credential or secret values"},
    {22, "managed-connection-closed", "managed audit boundary", "managed audit connection external requests schema migration and ledger writes remain closed"},
    {23, "active-router-disabled", "router boundary", "active shard router installation remains disabled"},
    {24, "write-routing-disabled", "write boundary", "write routing and store mutation remain disabled"},
    {25, "wal-load-restore-compact-disabled", "storage execution boundary", "WAL touch load restore and compact execution remain disabled"},
    {26, "cli-standalone-read-smoke", "mini-kv CLI smoke", "CLI smoke exercises only the standalone read command"},
    {27, "tcp-standalone-read-smoke", "mini-kv TCP smoke", "TCP smoke exercises only the standalone read command"},
    {28, "fixture-parity-current", "fixtures/release/shard-readiness.json", "current fixture stays aligned with the v1472 generated evidence"},
    {29, "archive-walkthrough-retention", "e/1472 archive and walkthrough index", "archive screenshots and walkthrough evidence remain documentation only"},
    {30, "clean-workspace-ci-closeout", "GitHub Actions closeout", "closeout keeps Node imports endpoints credentials routers writes WAL and execution disabled"},
}};

std::string format_check_json(const SandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationOnly\":true"
           ",\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeRendererExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":false"
           ",\"miniKvExecutesNodeChecks\":false"
           ",\"miniKvStartsServices\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const SandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheck>
sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("sandbox endpoint credential resolver upstream echo non-participation check count out of range");
    }

    std::string json = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            json += ",";
        }
        json += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks
