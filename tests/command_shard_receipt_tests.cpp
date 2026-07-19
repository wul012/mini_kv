#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_receipt_tests(CommandFixture& fixture) {
    auto& processor = fixture.processor;
    auto& result = fixture.result;

    result = processor.execute("SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\"");
    assert_response_contains(result, "\"sourceNodeTestOnlyShellSplitCloseoutReleaseVersion\":\"Node v1866\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result,
                             "\"sourceFollowUpCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceFollowUpReleaseVersion\":\"v1405\"");
    assert_response_contains(result, "\"sourceFollowUpPublishedStageCount\":20");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v1417\"");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpFixtureAuditReleaseRangeStart\":\"v1406\"");
    assert_response_contains(result, "\"plannedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_response_contains(result, "\"completedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpFixtureAuditOnly\":true");
    assert_response_contains(result, "\"sourceFollowUpNonParticipationFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeCredentialResolverShellExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"profileBoundaryEndpointRead\":false");
    assert_response_contains(result, "\"evidenceEndpointRead\":false");
    assert_response_contains(result, "\"typeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON extra");
    assert(result.response == "ERR usage: SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON");

    result = processor.execute("SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeDisabledPrecheckUpstreamEchoCloseoutReleaseVersion\":\"Node v1878\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result,
                             "\"sourceFixtureAuditCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\"");
    assert_response_contains(result, "\"sourceFixtureAuditReleaseVersion\":\"v1417\"");
    assert_response_contains(result, "\"sourceFixtureAuditPublishedStageCount\":12");
    assert_response_contains(result, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":\"v1442\"");
    assert_response_contains(result, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseRangeStart\":\"v1418\"");
    assert_response_contains(result, "\"plannedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_response_contains(result, "\"completedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_response_contains(result, "\"disabledPrecheckUpstreamEchoNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceFixtureAuditFrozen\":true");
    assert_response_contains(result, "\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_response_contains(
        result, "\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoCloseoutReleaseVersion\":\"Node v1903\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v114\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v105\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(
        result, "\"sourceDisabledPrecheckCommand\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceDisabledPrecheckReleaseVersion\":\"v1442\"");
    assert_response_contains(result, "\"sourceDisabledPrecheckPublishedStageCount\":25");
    assert_response_contains(
        result, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseVersion\":\"v1472\"");
    assert_response_contains(
        result, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeStart\":\"v1443\"");
    assert_response_contains(result,
                             "\"plannedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_response_contains(result,
                             "\"completedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_response_contains(result, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpointHandles\":false");
    assert_response_contains(result, "\"miniKvParsesRawEndpoint\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"endpointHandleRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\"");
    assert_response_contains(result,
                             "\"command\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeImplementationPlanUpstreamEchoCloseoutReleaseVersion\":\"Node v1934\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v126\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v121\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeImplementationPlanUpstreamEchoCloseoutDocumentationOnly\":true");
    assert_response_contains(result, "\"sourceSandboxEndpointCommand\":"
                                     "\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceSandboxEndpointReleaseVersion\":\"v1472\"");
    assert_response_contains(result, "\"sourceSandboxEndpointPublishedStageCount\":30");
    assert_response_contains(result,
                             "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":\"v1475\"");
    assert_response_contains(result,
                             "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeStart\":\"v1473\"");
    assert_response_contains(result, "\"plannedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_response_contains(result, "\"completedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_response_contains(result, "\"implementationPlanUpstreamEchoCloseoutNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"endpointHandleRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-release-window-readiness-packet-split-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion\":\"Node v1937\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v70\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v61\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(
        result,
        "\"sourceImplementationPlanCommand\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceImplementationPlanReleaseVersion\":\"v1475\"");
    assert_response_contains(result, "\"sourceImplementationPlanPublishedStageCount\":3");
    assert_response_contains(result, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v1490\"");
    assert_response_contains(result,
                             "\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeStart\":\"v1476\"");
    assert_response_contains(result, "\"plannedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_response_contains(result, "\"completedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_response_contains(result, "\"releaseWindowReadinessPacketSplitNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeStatusRoutesExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"miniKvExecutesReleaseWindowPacket\":false");
    assert_response_contains(result, "\"miniKvReplaysRestoreDrillEvidence\":false");
    assert_response_contains(result, "\"endpointHandleRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON");

    result =
        processor.execute("SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-disabled-fake-harness-contract-upstream-echo-"
                                     "verification-split-non-participation.v1\"");
    assert_response_contains(
        result,
        "\"command\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(
        result,
        "\"sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitCloseoutReleaseVersion\":\"Node v1967\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v127\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersionRange\":\"Java v122-v126\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceReleaseWindowReadinessPacketSplitCommand\":"
                                     "\"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceReleaseWindowReadinessPacketSplitReleaseVersion\":\"v1490\"");
    assert_response_contains(result, "\"sourceReleaseWindowReadinessPacketSplitPublishedStageCount\":15");
    assert_response_contains(result, "\"sourceReleaseWindowReadinessPacketSplitComplete\":true");
    assert_response_contains(
        result, "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_response_contains(
        result,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseRangeStart\":\"v1491\"");
    assert_response_contains(
        result,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseRangeEnd\":\"v1505\"");
    assert_response_contains(
        result, "\"plannedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_response_contains(
        result, "\"completedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_response_contains(result,
                             "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
    assert_response_contains(result,
                             "\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false");
    assert_response_contains(result,
                             "\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeDisabledFakeHarnessExecutionDeniedRoutePreflightExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"miniKvParsesHistoricalReceipts\":false");
    assert_response_contains(result, "\"miniKvReadsHistoricalMiniKvV127ReceiptFile\":false");
    assert_response_contains(result, "\"miniKvScansJavaEvidence\":false");
    assert_response_contains(result, "\"endpointHandleRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(
        result, "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":true");

    result = processor.execute(
        "SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON extra");
    assert(result.response ==
           "ERR usage: SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-blocked-execution-rehearsal-split-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion\":\"Node v1982\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v99\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"wal-regression-evidence\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v90\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"context-normalization-evidence\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(
        result, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCommand\":"
                "\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(
        result,
        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_response_contains(
        result, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":15");
    assert_response_contains(result, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":true");
    assert_response_contains(result, "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_response_contains(result, "\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeStart\":\"v1506\"");
    assert_response_contains(result, "\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeEnd\":\"v1525\"");
    assert_response_contains(result, "\"plannedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_response_contains(result, "\"completedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_response_contains(result, "\"blockedExecutionRehearsalSplitNonParticipationOnly\":true");
    assert_response_contains(result,
                             "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodePreconditionIntakeExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeBuildExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeChecks\":false");
    assert_response_contains(result, "\"miniKvStartsServices\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"miniKvParsesHistoricalReceipts\":false");
    assert_response_contains(result, "\"miniKvReadsHistoricalMiniKvV99WalRegressionFile\":false");
    assert_response_contains(result, "\"miniKvScansJavaV90ContextNormalizationEvidence\":false");
    assert_response_contains(result, "\"miniKvScansJavaEvidence\":false");
    assert_response_contains(result, "\"miniKvReplaysWalRegression\":false");
    assert_response_contains(result, "\"endpointHandleRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":true");

    result = processor.execute("SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-precheck-upstream-receipt-verification-split-non-participation.v1\"");
    assert_response_contains(result,
                             "\"command\":\"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(
        result, "\"sourceNodePrecheckUpstreamReceiptVerificationSplitCloseoutReleaseVersion\":\"Node v2002\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v108\"");
    assert_response_contains(result, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v99\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result,
                             "\"sourceBlockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_response_contains(result,
                             "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");
    assert_response_contains(result,
                             "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeStart\":\"v1526\"");
    assert_response_contains(result,
                             "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeEnd\":\"v1545\"");
    assert_response_contains(result,
                             "\"plannedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_response_contains(result,
                             "\"completedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_response_contains(result, "\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeCodeHealthScanExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvImportsNodeModules\":false");
    assert_response_contains(result, "\"miniKvReadsEndpoints\":false");
    assert_response_contains(result, "\"miniKvReadsCredentials\":false");
    assert_response_contains(result, "\"miniKvParsesHistoricalReceipts\":false");
    assert_response_contains(result, "\"miniKvReadsHistoricalMiniKvV108NonParticipationFile\":false");
    assert_response_contains(result, "\"miniKvScansJavaV99Evidence\":false");
    assert_response_contains(result, "\"miniKvExecutesDownstreamChecks\":false");
    assert_response_contains(result, "\"miniKvMutatesLargeFileInventory\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result,
                             "\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":true");

    result = processor.execute("SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-runtime-execution-packet-approval-gate-archive-"
                                     "verification-non-participation.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeArchiveVerificationReleaseVersion\":\"Node v399\"");
    assert_response_contains(result, "\"sourceNodeVerifiedArchiveReleaseVersion\":\"Node v398\"");
    assert_response_contains(result, "\"sourceNodeNextConsumer\":\"Node v400\"");
    assert_response_contains(
        result, "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":\"v1585\"");
    assert_response_contains(result, "\"approvalInputCount\":0");
    assert_response_contains(result, "\"requiredApprovalInputCount\":3");
    assert_response_contains(result, "\"runtimeArtifactCount\":0");
    assert_response_contains(result, "\"requiredRuntimeArtifactCount\":6");
    assert_response_contains(result, "\"miniKvReadsNodeArchiveFiles\":false");
    assert_response_contains(result, "\"miniKvStartsJavaService\":false");
    assert_response_contains(result, "\"miniKvStartsMiniKvService\":false");
    assert_response_contains(result, "\"managedAuditConnectionOpened\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(
        result, "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":true");

    result = processor.execute("SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON extra");
    assert(result.response ==
           "ERR usage: SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON");

    result = processor.execute("SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-code-walkthrough-quality-gate-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeCodeWalkthroughQualityGateReleaseVersion\":\"Node v2077\"");
    assert_response_contains(result, "\"sourceNodeQualityRunRange\":\"Node v2068-v2077\"");
    assert_response_contains(result,
                             "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationReleaseVersion\":\"v1585\"");
    assert_response_contains(result, "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v1590\"");
    assert_response_contains(result, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeStart\":\"v1586\"");
    assert_response_contains(result, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeEnd\":\"v1591\"");
    assert_response_contains(result, "\"walkthroughQualityGateOnly\":true");
    assert_response_contains(result, "\"codeWalkthroughMayBeOmittedForTinyMaintenance\":true");
    assert_response_contains(result, "\"placeholderWalkthroughAllowed\":false");
    assert_response_contains(result, "\"unsafeProductionClaimAllowed\":false");
    assert_response_contains(result, "\"bulkHistoricalRelocationAllowed\":false");
    assert_response_contains(result, "\"nodeWalkthroughFilesReadByMiniKv\":false");
    assert_response_contains(result, "\"miniKvScansNodeRepository\":false");
    assert_response_contains(result, "\"miniKvExecutesNodeQualityRoute\":false");
    assert_response_contains(result, "\"miniKvStartsNodeServices\":false");
    assert_response_contains(result, "\"miniKvStartsJavaService\":false");
    assert_response_contains(result, "\"miniKvStartsMiniKvService\":false");
    assert_response_contains(result, "\"miniKvMutatesNodeDocs\":false");
    assert_response_contains(result, "\"miniKvMutatesJavaDocs\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":true");

    result = processor.execute("SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-production-shard-execution-owner-receipt-request-packet.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\"");
    assert_response_contains(result, "\"sourceNodeProductionShardExecutionReleaseVersion\":\"Node v2087\"");
    assert_response_contains(result, "\"sourceNodeAllowsJavaMiniKvParallel\":true");
    assert_response_contains(result, "\"sourceNodeRequiresExistingSignedReceipts\":false");
    assert_response_contains(result, "\"sourceCodeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_response_contains(result, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v1595\"");
    assert_response_contains(result, "\"requestedReceiptSlotCount\":9");
    assert_response_contains(result, "\"signedMiniKvReceiptPresent\":false");
    assert_response_contains(result, "\"signedJavaReceiptPresent\":false");
    assert_response_contains(result, "\"signedCrossProjectCleanupReceiptPresent\":false");
    assert_response_contains(result, "\"perVersionArchiveRequired\":true");
    assert_response_contains(result, "\"combinedArchiveFolderAllowed\":false");
    assert_response_contains(result, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");
    assert_response_contains(result, "\"productionShardExecutionEnabled\":false");
    assert_response_contains(result, "\"managedAuditStoreBound\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"mutatesStore\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON extra");
    assert(result.response == "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON\"");
    assert_response_contains(result, "\"sourceOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_response_contains(result, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v1600\"");
    assert_response_contains(result, "\"miniKvReceiptSlotCount\":4");
    assert_response_contains(result, "\"miniKvReceiptsComplete\":true");
    assert_response_contains(result, "\"javaOwnerReceiptsPresent\":false");
    assert_response_contains(result, "\"crossProjectCleanupReconciliationReceiptPresent\":false");
    assert_response_contains(result, "\"archiveRoot\":\"f\"");
    assert_response_contains(result, "\"archiveRootIsF\":true");
    assert_response_contains(result, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");
    assert_response_contains(result, "\"abortExecutionPerformed\":false");
    assert_response_contains(result, "\"rollbackExecutionPerformed\":false");
    assert_response_contains(result, "\"lifecycleServiceStarted\":false");
    assert_response_contains(result, "\"cleanupExecutionPerformed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"mutatesStore\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON extra");
    assert(result.response == "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-production-shard-execution-external-artifact-dry-run-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON\"");
    assert_response_contains(result, "\"sourceMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_response_contains(result,
                             "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v1606\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v1605\"");
    assert_response_contains(result, "\"dryRunBatchClosed\":true");
    assert_response_contains(result, "\"nodeOnlyGrowthStopped\":true");
    assert_response_contains(result, "\"realExternalArtifactPresent\":false");
    assert_response_contains(result, "\"externalArtifactParsed\":false");
    assert_response_contains(result, "\"externalArtifactAccepted\":false");
    assert_response_contains(result, "\"externalArtifactStored\":false");
    assert_response_contains(result, "\"dryRunArtifactExecuted\":false");
    assert_response_contains(result, "\"realSignedApprovalPresent\":false");
    assert_response_contains(result, "\"managedAuditOwnerBindingPresent\":false");
    assert_response_contains(result, "\"realJavaReceiptPresent\":false");
    assert_response_contains(result, "\"realMiniKvSignedReceiptPresent\":false");
    assert_response_contains(result, "\"realCleanupReceiptPresent\":false");
    assert_response_contains(result, "\"archiveRoot\":\"f\"");
    assert_response_contains(result, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"mutatesStore\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON extra");
    assert(result.response == "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON");

    result = processor.execute("SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");
    assert_response_contains(result, "\"contract\":\"shard-f-folder-explanation-quality-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/v2103-f-folder-explanation-expansion-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceExternalArtifactCloseoutReleaseVersion\":\"v1606\"");
    assert_response_contains(result, "\"fFolderExplanationQualityCloseoutReleaseVersion\":\"v1607\"");
    assert_response_contains(result, "\"chineseWritingRequired\":true");
    assert_response_contains(result, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_response_contains(result, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_response_contains(result, "\"fillerPaddingAllowed\":false");
    assert_response_contains(result, "\"singleVersionLargeScope\":true");
    assert_response_contains(result, "\"multiVersionFragmentationAllowed\":false");
    assert_response_contains(result, "\"plannedFFolderExplanationQualityRuleCount\":13");
    assert_response_contains(result, "\"completedFFolderExplanationQualityCloseoutCheckCount\":19");
    assert_response_contains(result, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
    assert_response_contains(result, "\"scansNodeFolders\":false");
    assert_response_contains(result, "\"importsNodeQualityRoute\":false");
    assert_response_contains(result, "\"createsPictureDirsWithoutEvidence\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"mutatesStore\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON extra");
    assert(result.response == "ERR usage: SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");
}

} // namespace minikv::command_test
