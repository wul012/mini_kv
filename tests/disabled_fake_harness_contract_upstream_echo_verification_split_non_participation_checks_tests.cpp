#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks.hpp"

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
        minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks;

    const auto records =
        checks::disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks();
    assert(checks::
               planned_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_check_count() ==
           15);
    assert(records.size() == 15);
    assert(records.front().check_code == std::string{"public-entrypoint-preserved-no-import"});
    assert(records.back().check_code == std::string{"archive-cleanup-ci-closeout"});

    const auto empty =
        checks::format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json =
        checks::format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks_json(15);
    assert_contains(json, "\"checkCode\":\"public-entrypoint-preserved-no-import\"");
    assert_contains(json, "\"checkCode\":\"constants-module-no-import\"");
    assert_contains(json, "\"checkCode\":\"node-v288-projection-no-execution\"");
    assert_contains(json, "\"checkCode\":\"java-v122-v126-reference-only\"");
    assert_contains(json, "\"checkCode\":\"minikv-v127-receipt-reference-only\"");
    assert_contains(json, "\"checkCode\":\"expected-snippet-builders-no-execution\"");
    assert_contains(json, "\"checkCode\":\"reference-helper-no-execution\"");
    assert_contains(json, "\"checkCode\":\"core-assembly-no-execution\"");
    assert_contains(json, "\"checkCode\":\"summary-aggregation-no-execution\"");
    assert_contains(json, "\"checkCode\":\"readiness-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"blocker-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"warning-recommendation-policy-no-execution\"");
    assert_contains(json, "\"checkCode\":\"focused-tests-observed-only\"");
    assert_contains(json, "\"checkCode\":\"typecheck-build-observed-only\"");
    assert_contains(json, "\"checkCode\":\"archive-cleanup-ci-closeout\"");
    assert_contains(json,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationOnly\":true");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessExecutionDeniedRoutePreflightExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvParsesHistoricalReceipts\":false");
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
        (void)checks::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks_json(16);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
