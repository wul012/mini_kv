#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<TypeBarrelSplitFollowUpFixtureAuditCheck, 12> check_catalog = {{
    {1, "source-follow-up-fixture-freeze", "mini-kv v1405", "v1405 remains the frozen source fixture for the fixture-audit chain"},
    {2, "current-fixture-parity-audit", "mini-kv v1407", "current shard-readiness fixture parity is checked after the audit section rolls forward"},
    {3, "versioned-fixture-continuity", "mini-kv v1408", "every audit release keeps a versioned fixture path and source pointer"},
    {4, "command-catalog-read-only-audit", "mini-kv v1409", "the audit command appears only as read metadata with no mutation or WAL touch"},
    {5, "cli-payload-boundary-audit", "mini-kv v1410", "large SHARDJSON payload review remains CLI and fixture based"},
    {6, "tcp-standalone-command-audit", "mini-kv v1411", "TCP smoke exercises only the standalone audit read command"},
    {7, "archive-screenshot-retention-audit", "mini-kv v1412", "command screenshots are retained without runtime archive scanning"},
    {8, "walkthrough-index-retention-audit", "mini-kv v1413", "walkthrough index evidence remains documentation-only"},
    {9, "node-test-only-shell-no-import", "Node v1847-v1866 context", "Node test-only shell split modules are neither imported nor executed"},
    {10, "no-router-write-wal-audit", "mini-kv v1415", "active router write routing load restore compact and WAL touch stay disabled"},
    {11, "whitespace-quality-gate-audit", "mini-kv v1416", "fixtures and walkthrough docs stay under git whitespace quality checks"},
    {12, "clean-workspace-ci-closeout-audit", "mini-kv v1417", "closeout keeps Node imports endpoints routers writes WAL and execution disabled"},
}};

std::string format_check_json(const TypeBarrelSplitFollowUpFixtureAuditCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"typeBarrelSplitFollowUpFixtureAuditOnly\":true"
           ",\"sourceFollowUpNonParticipationFixtureFrozen\":true"
           ",\"currentFixtureParityAudited\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeTestOnlyShellSplitObserved\":true"
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeTestOnlyShellModulesImportedByMiniKv\":false"
           ",\"nodeCredentialResolverShellExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvRuntimeReadsSourceFixturePayload\":false"
           ",\"miniKvImportsNodeModules\":false"
           ",\"miniKvExecutesNodeChecks\":false"
           ",\"miniKvStartsServices\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const TypeBarrelSplitFollowUpFixtureAuditCheck>
type_barrel_split_follow_up_fixture_audit_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_type_barrel_split_follow_up_fixture_audit_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_type_barrel_split_follow_up_fixture_audit_checks_json(int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("type barrel split follow-up fixture audit check count out of range");
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

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_checks
