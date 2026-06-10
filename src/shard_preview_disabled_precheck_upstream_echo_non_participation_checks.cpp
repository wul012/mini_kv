#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<DisabledPrecheckUpstreamEchoNonParticipationCheck, 25> check_catalog = {{
    {1, "source-fixture-audit-freeze", "mini-kv v1417", "v1417 fixture-audit evidence is the frozen source for this chain"},
    {2, "node-plan-intake", "Node v1867-v1878", "Node disabled-precheck upstream echo work is treated as module split maintenance only"},
    {3, "command-catalog-read-only", "mini-kv COMMANDS and COMMANDSJSON", "the new command is visible only as read-only metadata"},
    {4, "current-shardjson-rollover", "mini-kv SHARDJSON", "current shard readiness status rolls forward without enabling execution"},
    {5, "versioned-fixture-chain-continuity", "mini-kv fixtures", "each release keeps contiguous versioned fixture pointers"},
    {6, "stable-barrel-no-import", "Node service entrypoint", "mini-kv does not import the stable TypeScript service barrel"},
    {7, "constants-reference-no-import", "Node constants and reference builders", "mini-kv does not import Node constants reference builders or sibling fixture parsers"},
    {8, "checks-messages-no-execution", "Node readiness checks", "mini-kv does not execute Node readiness checks blockers warnings or recommendations"},
    {9, "core-loader-no-execution", "Node core loader", "mini-kv does not execute the disabled-precheck upstream echo core loader"},
    {10, "renderer-export-stability", "Node route renderer export", "renderer export stability remains a Node-owned concern"},
    {11, "node-test-boundary", "Node typecheck Vitest build", "mini-kv does not run Node typecheck Vitest npm test or build"},
    {12, "no-sibling-service-startup", "cross-project startup boundary", "Node plan requires no Java or mini-kv sibling service startup"},
    {13, "endpoint-handle-no-read", "endpoint handle boundary", "mini-kv does not read endpoint handles"},
    {14, "raw-endpoint-no-parse", "raw endpoint boundary", "mini-kv does not parse raw endpoint URLs"},
    {15, "credential-secret-no-read", "credential boundary", "mini-kv does not read credential or secret values"},
    {16, "active-router-disabled", "router boundary", "active shard router installation remains disabled"},
    {17, "write-routing-disabled", "write boundary", "write routing and store mutation remain disabled"},
    {18, "wal-load-restore-compact-disabled", "storage execution boundary", "WAL touch load restore and compact execution remain disabled"},
    {19, "cli-standalone-read-smoke", "mini-kv CLI smoke", "CLI smoke exercises only the standalone read command"},
    {20, "tcp-standalone-read-smoke", "mini-kv TCP smoke", "TCP smoke exercises only the standalone read command"},
    {21, "fixture-parity-current", "fixtures/release/shard-readiness.json", "current fixture stays aligned with the v1442 generated evidence"},
    {22, "archive-screenshot-retention", "e/1442 archive", "command screenshots are retained without runtime archive walks"},
    {23, "walkthrough-index-retention", "code walkthrough index", "walkthrough evidence remains documentation only"},
    {24, "whitespace-quality-gate", "git diff --check", "docs fixtures and source stay free of trailing whitespace"},
    {25, "clean-workspace-ci-closeout", "GitHub Actions closeout", "closeout keeps Node imports endpoints routers writes WAL and execution disabled"},
}};

std::string format_check_json(const DisabledPrecheckUpstreamEchoNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationOnly\":true"
           ",\"sourceFixtureAuditFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckConstantsImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckReferenceBuildersImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckChecksExecutedByMiniKv\":false"
           ",\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false"
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
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const DisabledPrecheckUpstreamEchoNonParticipationCheck>
disabled_precheck_upstream_echo_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_disabled_precheck_upstream_echo_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_disabled_precheck_upstream_echo_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("disabled precheck upstream echo non-participation check count out of range");
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

} // namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_checks
