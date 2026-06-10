#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation;

    const auto passed =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            25, true, false, false, false, false, false, false, false, 30, 30, 30, 30);
    assert_contains(passed, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true");
    assert_contains(passed, "\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false");
    assert_contains(passed, "\"miniKvImportsNodeModules\":false");
    assert_contains(passed, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(passed, "\"miniKvStartsServices\":false");
    assert_contains(passed, "\"miniKvReadsEndpointHandles\":false");
    assert_contains(passed, "\"miniKvParsesRawEndpoint\":false");
    assert_contains(passed, "\"miniKvReadsCredentials\":false");
    assert_contains(passed, "\"endpointHandleRead\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"credentialValueRead\":false");
    assert_contains(passed, "\"managedAuditConnectionOpened\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            24, true, false, false, false, false, false, false, false, 30, 30, 30, 30);
    assert_contains(failed_source,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_node_plan =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            25, true, true, false, false, false, false, false, false, 30, 30, 30, 30);
    assert_contains(failed_node_plan,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_node_plan, "\"nodePlanObservedOnly\":false");

    const auto failed_import =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            25, true, false, true, false, false, false, false, false, 30, 30, 30, 30);
    assert_contains(failed_import,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_import, "\"miniKvBoundaryClosed\":false");

    const auto failed_raw_endpoint =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            25, true, false, false, false, false, false, true, false, 30, 30, 30, 30);
    assert_contains(failed_raw_endpoint,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_raw_endpoint, "\"miniKvBoundaryClosed\":false");

    const auto failed_count =
        validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
            25, true, false, false, false, false, false, false, false, 30, 29, 30, 30);
    assert_contains(failed_count,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
