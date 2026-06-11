#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ImplementationPlanUpstreamEchoCloseoutNonParticipationCheck, 8> check_catalog = {{
    {1, "source-sandbox-endpoint-fixture-freeze", "mini-kv v1472", "v1472 sandbox endpoint credential resolver upstream echo non-participation remains the frozen source"},
    {2, "node-closeout-plan-intake", "Node v1934", "Node implementation-plan upstream echo closeout is documentation-only maintenance context"},
    {3, "ownership-map-metadata-only", "Node module ownership map", "barrel constants references checks messages and core ownership are recorded without importing Node code"},
    {4, "historical-sibling-reference-only", "Node v1934 cross-project plan", "Node consumes frozen mini-kv v126 and Java v121 references without fresh sibling startup"},
    {5, "endpoint-credential-boundary-closed", "endpoint and credential boundary", "mini-kv does not read endpoint handles raw endpoints credentials secrets or managed audit connections"},
    {6, "router-write-wal-execution-disabled", "storage execution boundary", "router write WAL load restore compact and execution remain disabled"},
    {7, "command-shardjson-fixture-parity", "mini-kv command and fixtures", "standalone command current SHARDJSON and v1473-v1475 fixtures stay aligned"},
    {8, "clean-workspace-ci-closeout", "GitHub Actions closeout", "closeout keeps Node imports checks endpoints credentials routers writes WAL and execution disabled"},
}};

std::string format_check_json(const ImplementationPlanUpstreamEchoCloseoutNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationOnly\":true"
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true"
           ",\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false"
           ",\"nodeImplementationPlanConstantsImportedByMiniKv\":false"
           ",\"nodeImplementationPlanReferencesImportedByMiniKv\":false"
           ",\"nodeImplementationPlanChecksExecutedByMiniKv\":false"
           ",\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":false"
           ",\"miniKvExecutesNodeChecks\":false"
           ",\"miniKvStartsServices\":false"
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

} // namespace

std::span<const ImplementationPlanUpstreamEchoCloseoutNonParticipationCheck>
implementation_plan_upstream_echo_closeout_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_implementation_plan_upstream_echo_closeout_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_implementation_plan_upstream_echo_closeout_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("implementation plan upstream echo closeout non-participation check count out of range");
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

} // namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks
