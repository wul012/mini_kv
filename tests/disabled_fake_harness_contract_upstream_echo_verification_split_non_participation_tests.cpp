#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence =
        minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation;

    const auto json =
        evidence::format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation.v1\"");
    assert_contains(
        json,
        "\"command\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1953-v1967-disabled-fake-harness-contract-upstream-echo-verification-split-roadmap.md\"");
    assert_contains(json,
                    "\"sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitReleaseRange\":\"Node v1953-v1967\"");
    assert_contains(json,
                    "\"sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitCloseoutReleaseVersion\":\"Node v1967\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v127\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersionRange\":\"Java v122-v126\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitReleaseVersion\":\"v1490\"");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitPublishedStageCount\":15");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1490\"");
    assert_contains(json,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1504\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1504.json\"");
    assert_contains(json,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseRangeStart\":\"v1491\"");
    assert_contains(json,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseRangeEnd\":\"v1505\"");
    assert_contains(json, "\"publishedStageCount\":15");
    assert_contains(json, "\"plannedStageCount\":15");
    assert_contains(json,
                    "\"plannedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_contains(json,
                    "\"completedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_contains(json,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationChainComplete\":true");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
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
    assert_contains(json, "\"miniKvReadsHistoricalMiniKvV127ReceiptFile\":false");
    assert_contains(json, "\"miniKvScansJavaEvidence\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(
        json,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":true");

    const auto digest =
        evidence::disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_digest_marker();
    assert(digest.find(
               "v1505-route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 15);
    assert(evidence::disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_status() ==
           "route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
