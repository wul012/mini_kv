#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks.hpp"

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
        minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks;

    const auto records = checks::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks();
    assert(checks::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_check_count() ==
           30);
    assert(records.size() == 30);
    assert(records.front().check_code == std::string{"source-disabled-precheck-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty =
        checks::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json =
        checks::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(30);
    assert_contains(json, "\"checkCode\":\"source-disabled-precheck-freeze\"");
    assert_contains(json, "\"checkCode\":\"stable-barrel-no-import\"");
    assert_contains(json, "\"checkCode\":\"readiness-check-no-execution\"");
    assert_contains(json, "\"checkCode\":\"endpoint-handle-no-read\"");
    assert_contains(json, "\"checkCode\":\"raw-endpoint-no-parse\"");
    assert_contains(json, "\"checkCode\":\"credential-secret-no-read\"");
    assert_contains(json, "\"checkCode\":\"managed-connection-closed\"");
    assert_contains(json, "\"checkCode\":\"wal-load-restore-compact-disabled\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationOnly\":true");
    assert_contains(json, "\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"versionedFixtureChainOnly\":true");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"schemaMigrationExecuted\":false");
    assert_contains(json, "\"ledgerWriteExecuted\":false");
    assert_contains(json, "\"externalRequestSent\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(31);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
