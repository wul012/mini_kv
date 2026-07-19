#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void run_shard_fixture_late_tests(ShardFixture& fixture) {
    assert_contains(fixture.version(165), "\"releaseVersion\":\"v165\"");
    assert_contains(fixture.version(165), "\"status\":\"shard-readiness-release-catalog-read-only\"");
    assert_contains(fixture.version(165), "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                                          "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_contains(fixture.version(165), "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_contains(fixture.version(165), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_contains(fixture.version(165), "\"catalogedReleaseCount\":4");
    assert_contains(fixture.version(165), "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_contains(fixture.version(165), "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(fixture.version(165), "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_contains(fixture.version(165), "\"versionedFixtureCatalogOnly\":true");
    assert_contains(fixture.version(165), "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(fixture.version(165), "\"activeRouterInstalled\":false");
    assert_contains(fixture.version(165), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(165), "\"executionAllowed\":false");
    assert_contains(fixture.version(165),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v6\"");
    assert_contains(fixture.version(165), "\"fieldCount\":357");
    assert_contains(fixture.version(165), "\"groupCount\":19");
    assert_contains(fixture.version(165), "\"evidenceDigest\":\"fnv1a64:5e94e2a9b438d2ad\"");
    assert_contains(fixture.version(166), "\"releaseVersion\":\"v166\"");
    assert_contains(fixture.version(166), "\"status\":\"shard-readiness-release-catalog-audit-read-only\"");
    assert_contains(fixture.version(166), "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                                          "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_contains(fixture.version(166), "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_contains(fixture.version(166), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_contains(fixture.version(166), "\"expectedReleaseCount\":4");
    assert_contains(fixture.version(166), "\"observedReleaseCount\":4");
    assert_contains(fixture.version(166), "\"contiguousReleaseRange\":true");
    assert_contains(fixture.version(166), "\"duplicateReleasesDetected\":false");
    assert_contains(fixture.version(166), "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(fixture.version(166), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(166), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(166), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(166), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(166), "\"executionAllowed\":false");
    assert_contains(fixture.version(166),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v7\"");
    assert_contains(fixture.version(166), "\"fieldCount\":381");
    assert_contains(fixture.version(166), "\"groupCount\":20");
    assert_contains(fixture.version(166), "\"evidenceDigest\":\"fnv1a64:4bd18a01790dc5f1\"");
    assert_contains(fixture.version(167), "\"releaseVersion\":\"v167\"");
    assert_contains(fixture.version(167), "\"status\":\"node-route-split-window-compatibility-read-only\"");
    assert_contains(fixture.version(167), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v440-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(167), "\"sourceFrozenReleaseVersion\":\"v166\"");
    assert_contains(fixture.version(167), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v166.json\"");
    assert_contains(fixture.version(167), "\"windowEndNodeVersion\":\"Node v440\"");
    assert_contains(fixture.version(167), "\"splitRouteGroupCount\":8");
    assert_contains(fixture.version(167), "\"nodeAddsEvidenceGate\":false");
    assert_contains(fixture.version(167), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(167), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(167), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(167), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(167), "\"executionAllowed\":false");
    assert_contains(fixture.version(167),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(fixture.version(167), "\"fieldCount\":401");
    assert_contains(fixture.version(167), "\"groupCount\":21");
    assert_contains(fixture.version(167), "\"evidenceDigest\":\"fnv1a64:caf300915d4e988c\"");
    assert_contains(fixture.version(168), "\"releaseVersion\":\"v168\"");
    assert_contains(fixture.version(168), "\"status\":\"node-route-split-window-preparation-plan-read-only\"");
    assert_contains(fixture.version(168), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v441-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(168), "\"sourceFrozenReleaseVersion\":\"v167\"");
    assert_contains(fixture.version(168), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v167.json\"");
    assert_contains(fixture.version(168), "\"windowEndNodeVersion\":\"Node v441\"");
    assert_contains(fixture.version(168), "\"splitRouteGroupCount\":9");
    assert_contains(fixture.version(168), "\"nodeV441DesignDraftBodyPreparationPlanRouteSplitPreserved\":true");
    assert_contains(fixture.version(168), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(168), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(168), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(168), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(168), "\"executionAllowed\":false");
    assert_contains(fixture.version(168),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(fixture.version(168), "\"fieldCount\":401");
    assert_contains(fixture.version(168), "\"groupCount\":21");
    assert_contains(fixture.version(168), "\"evidenceDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(fixture.version(169), "\"releaseVersion\":\"v169\"");
    assert_contains(fixture.version(169), "\"status\":\"node-route-split-window-audit-read-only\"");
    assert_contains(fixture.version(169), "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                                          "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_contains(fixture.version(169), "\"sourceFrozenReleaseVersion\":\"v168\"");
    assert_contains(fixture.version(169), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v168.json\"");
    assert_contains(fixture.version(169), "\"expectedWindowVersionCount\":9");
    assert_contains(fixture.version(169), "\"observedWindowVersionCount\":9");
    assert_contains(fixture.version(169), "\"windowRangeEnd\":\"Node v441\"");
    assert_contains(fixture.version(169), "\"sourceFrozenWindowDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(fixture.version(169), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(169), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(169), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(169), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(169), "\"executionAllowed\":false");
    assert_contains(fixture.version(169),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(169), "\"fieldCount\":420");
    assert_contains(fixture.version(169), "\"groupCount\":22");
    assert_contains(fixture.version(169), "\"evidenceDigest\":\"fnv1a64:eaa587d1a50d8200\"");
    assert_contains(fixture.version(170), "\"releaseVersion\":\"v170\"");
    assert_contains(fixture.version(170), "\"status\":\"node-route-split-window-draft-candidate-read-only\"");
    assert_contains(fixture.version(170), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v442-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(170), "\"sourceFrozenReleaseVersion\":\"v169\"");
    assert_contains(fixture.version(170), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v169.json\"");
    assert_contains(fixture.version(170), "\"windowEndNodeVersion\":\"Node v442\"");
    assert_contains(fixture.version(170), "\"splitRouteGroupCount\":10");
    assert_contains(fixture.version(170), "\"observedWindowVersionCount\":10");
    assert_contains(fixture.version(170), "\"nodeV442DesignDraftBodyDraftCandidateRouteSplitPreserved\":true");
    assert_contains(fixture.version(170), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(170), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(170), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(170), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(170), "\"executionAllowed\":false");
    assert_contains(fixture.version(170),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(170), "\"fieldCount\":420");
    assert_contains(fixture.version(170), "\"groupCount\":22");
    assert_contains(fixture.version(170), "\"evidenceDigest\":\"fnv1a64:d06aaa033f5c1d86\"");
    assert_contains(fixture.version(171), "\"releaseVersion\":\"v171\"");
    assert_contains(fixture.version(171), "\"status\":\"node-route-split-window-managed-audit-read-only\"");
    assert_contains(fixture.version(171), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v444-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(171), "\"sourceFrozenReleaseVersion\":\"v170\"");
    assert_contains(fixture.version(171), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v170.json\"");
    assert_contains(fixture.version(171), "\"windowEndNodeVersion\":\"Node v444\"");
    assert_contains(fixture.version(171), "\"splitRouteGroupCount\":12");
    assert_contains(fixture.version(171), "\"observedWindowVersionCount\":12");
    assert_contains(fixture.version(171), "\"nodeV443ManagedAuditAdapterRouteSplitPreserved\":true");
    assert_contains(fixture.version(171), "\"nodeV444ManagedAuditPersistenceRouteSplitPreserved\":true");
    assert_contains(fixture.version(171), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(171), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(171), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(171), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(171), "\"executionAllowed\":false");
    assert_contains(fixture.version(171),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(171), "\"fieldCount\":420");
    assert_contains(fixture.version(171), "\"groupCount\":22");
    assert_contains(fixture.version(171), "\"evidenceDigest\":\"fnv1a64:9f6aa583ef521d1a\"");
    assert_contains(fixture.version(172), "\"releaseVersion\":\"v172\"");
    assert_contains(fixture.version(172), "\"status\":\"node-route-split-window-restore-drill-read-only\"");
    assert_contains(fixture.version(172), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v446-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(172), "\"sourceFrozenReleaseVersion\":\"v171\"");
    assert_contains(fixture.version(172), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v171.json\"");
    assert_contains(fixture.version(172), "\"windowEndNodeVersion\":\"Node v446\"");
    assert_contains(fixture.version(172), "\"splitRouteGroupCount\":14");
    assert_contains(fixture.version(172), "\"observedWindowVersionCount\":14");
    assert_contains(fixture.version(172), "\"nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved\":true");
    assert_contains(fixture.version(172), "\"nodeV446ManagedAuditRestoreDrillRouteSplitPreserved\":true");
    assert_contains(fixture.version(172), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(172), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(172), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(172), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(172), "\"executionAllowed\":false");
    assert_contains(fixture.version(172),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(172), "\"fieldCount\":420");
    assert_contains(fixture.version(172), "\"groupCount\":22");
    assert_contains(fixture.version(172), "\"evidenceDigest\":\"fnv1a64:85f0acb5a011256f\"");
    assert_contains(fixture.version(173), "\"releaseVersion\":\"v173\"");
    assert_contains(fixture.version(173), "\"status\":\"node-route-split-window-dry-run-adapter-read-only\"");
    assert_contains(fixture.version(173), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v447-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(173), "\"sourceFrozenReleaseVersion\":\"v172\"");
    assert_contains(fixture.version(173), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v172.json\"");
    assert_contains(fixture.version(173), "\"windowEndNodeVersion\":\"Node v447\"");
    assert_contains(fixture.version(173), "\"splitRouteGroupCount\":15");
    assert_contains(fixture.version(173), "\"observedWindowVersionCount\":15");
    assert_contains(fixture.version(173), "\"nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved\":true");
    assert_contains(fixture.version(173), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(173), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(173), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(173), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(173), "\"executionAllowed\":false");
    assert_contains(fixture.version(173),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(173), "\"fieldCount\":420");
    assert_contains(fixture.version(173), "\"groupCount\":22");
    assert_contains(fixture.version(173), "\"evidenceDigest\":\"fnv1a64:33a60bc1d66fd286\"");
    assert_contains(fixture.version(174), "\"releaseVersion\":\"v174\"");
    assert_contains(fixture.version(174), "\"status\":\"node-route-split-window-route-quality-read-only\"");
    assert_contains(fixture.version(174), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v448-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(174), "\"sourceFrozenReleaseVersion\":\"v173\"");
    assert_contains(fixture.version(174), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v173.json\"");
    assert_contains(fixture.version(174), "\"windowEndNodeVersion\":\"Node v448\"");
    assert_contains(fixture.version(174), "\"splitRouteGroupCount\":16");
    assert_contains(fixture.version(174), "\"observedWindowVersionCount\":16");
    assert_contains(fixture.version(174), "\"nodeV448ManagedAuditRouteQualityRouteSplitPreserved\":true");
    assert_contains(fixture.version(174), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(174), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(174), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(174), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(174), "\"executionAllowed\":false");
    assert_contains(fixture.version(174),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(174), "\"fieldCount\":420");
    assert_contains(fixture.version(174), "\"groupCount\":22");
    assert_contains(fixture.version(174), "\"evidenceDigest\":\"fnv1a64:d582c3d04aed99e1\"");
    assert_contains(fixture.version(175), "\"releaseVersion\":\"v175\"");
    assert_contains(fixture.version(175), "\"status\":\"node-route-split-window-adapter-implementation-read-only\"");
    assert_contains(fixture.version(175), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v449-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(175), "\"sourceFrozenReleaseVersion\":\"v174\"");
    assert_contains(fixture.version(175), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v174.json\"");
    assert_contains(fixture.version(175), "\"windowEndNodeVersion\":\"Node v449\"");
    assert_contains(fixture.version(175), "\"splitRouteGroupCount\":17");
    assert_contains(fixture.version(175), "\"observedWindowVersionCount\":17");
    assert_contains(fixture.version(175), "\"nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved\":true");
    assert_contains(fixture.version(175), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(175), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(175), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(175), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(175), "\"executionAllowed\":false");
    assert_contains(fixture.version(175),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(175), "\"fieldCount\":420");
    assert_contains(fixture.version(175), "\"groupCount\":22");
    assert_contains(fixture.version(175), "\"evidenceDigest\":\"fnv1a64:61e561e1c8ba7d50\"");
    assert_contains(fixture.version(176), "\"releaseVersion\":\"v176\"");
    assert_contains(fixture.version(176),
                    "\"status\":\"node-route-split-window-manual-sandbox-fake-transport-read-only\"");
    assert_contains(fixture.version(176), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v457-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(176), "\"sourceFrozenReleaseVersion\":\"v175\"");
    assert_contains(fixture.version(176), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v175.json\"");
    assert_contains(fixture.version(176), "\"windowEndNodeVersion\":\"Node v457\"");
    assert_contains(fixture.version(176), "\"splitRouteGroupCount\":25");
    assert_contains(fixture.version(176), "\"observedWindowVersionCount\":25");
    assert_contains(fixture.version(176), "\"nodeV457ManualSandboxConnectionFakeTransportRouteSplitPreserved\":true");
    assert_contains(fixture.version(176), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(176), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(176), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(176), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(176), "\"executionAllowed\":false");
    assert_contains(fixture.version(176),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(176), "\"fieldCount\":420");
    assert_contains(fixture.version(176), "\"groupCount\":22");
    assert_contains(fixture.version(176), "\"evidenceDigest\":\"fnv1a64:528073c4315ef89b\"");
    assert_contains(fixture.version(177), "\"releaseVersion\":\"v177\"");
    assert_contains(fixture.version(177), "\"status\":\"node-route-split-window-foundational-audit-read-only\"");
    assert_contains(fixture.version(177), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v458-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture.version(177), "\"sourceFrozenReleaseVersion\":\"v176\"");
    assert_contains(fixture.version(177), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v176.json\"");
    assert_contains(fixture.version(177), "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(fixture.version(177), "\"splitRouteGroupCount\":26");
    assert_contains(fixture.version(177), "\"observedWindowVersionCount\":26");
    assert_contains(fixture.version(177), "\"nodeV458FoundationalAuditRouteSplitPreserved\":true");
    assert_contains(fixture.version(177), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(177), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(177), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(177), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(177), "\"executionAllowed\":false");
    assert_contains(fixture.version(177),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(177), "\"fieldCount\":420");
    assert_contains(fixture.version(177), "\"groupCount\":22");
    assert_contains(fixture.version(177), "\"evidenceDigest\":\"fnv1a64:e773708a4decc60e\"");
    assert_contains(fixture.version(178), "\"releaseVersion\":\"v178\"");
    assert_contains(fixture.version(178), "\"status\":\"node-route-split-window-computed-audit-read-only\"");
    assert_contains(fixture.version(178), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v458-route-split-window-computed-audit-read-only\"");
    assert_contains(fixture.version(178), "\"sourceFrozenReleaseVersion\":\"v177\"");
    assert_contains(fixture.version(178), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v177.json\"");
    assert_contains(fixture.version(178), "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(fixture.version(178), "\"splitRouteGroupCount\":26");
    assert_contains(fixture.version(178), "\"observedWindowVersionCount\":26");
    assert_contains(fixture.version(178), "\"contiguousNodeVersionWindow\":true");
    assert_contains(fixture.version(178), "\"duplicateWindowVersionsDetected\":false");
    assert_contains(fixture.version(178), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(178), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(178), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(178), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(178), "\"executionAllowed\":false");
    assert_contains(fixture.version(178),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture.version(178), "\"fieldCount\":420");
    assert_contains(fixture.version(178), "\"groupCount\":22");
    assert_contains(fixture.version(178), "\"evidenceDigest\":\"fnv1a64:dd89f7c3bd63d7c3\"");
    assert_contains(fixture.version(179), "\"releaseVersion\":\"v179\"");
    assert_contains(fixture.version(179), "\"status\":\"node-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(fixture.version(179), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v458-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(fixture.version(179), "\"sourceFrozenReleaseVersion\":\"v178\"");
    assert_contains(fixture.version(179), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v178.json\"");
    assert_contains(fixture.version(179), "\"windowRangeStartNumber\":433");
    assert_contains(fixture.version(179), "\"windowRangeEndNumber\":458");
    assert_contains(fixture.version(179), "\"computedWindowVersionSpan\":26");
    assert_contains(fixture.version(179), "\"windowRangeNumbersParseable\":true");
    assert_contains(fixture.version(179), "\"windowCountMatchesRange\":true");
    assert_contains(fixture.version(179), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(179), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(179), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(179), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(179), "\"executionAllowed\":false");
    assert_contains(fixture.version(179),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v10\"");
    assert_contains(fixture.version(179), "\"fieldCount\":422");
    assert_contains(fixture.version(179), "\"groupCount\":22");
    assert_contains(fixture.version(179), "\"evidenceDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(fixture.version(180), "\"releaseVersion\":\"v180\"");
    assert_contains(fixture.version(180), "\"status\":\"shard-readiness-history-formatter-maintenance-read-only\"");
    assert_contains(fixture.version(180), "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                                          "\"history-fixture-archive-formatter-split-read-only\"");
    assert_contains(fixture.version(180), "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_contains(fixture.version(180), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_contains(fixture.version(180), "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(fixture.version(180), "\"preservesFixtureParity\":true");
    assert_contains(fixture.version(180), "\"preservesArchiveCompatibility\":true");
    assert_contains(fixture.version(180), "\"preservesHistoricalFallback\":true");
    assert_contains(fixture.version(180), "\"changesShardJsonCommand\":false");
    assert_contains(fixture.version(180), "\"changesFixturePath\":false");
    assert_contains(fixture.version(180), "\"startsServices\":false");
    assert_contains(fixture.version(180), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture.version(180), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(180), "\"executionAllowed\":false");
    assert_contains(fixture.version(180),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(fixture.version(180), "\"fieldCount\":431");
    assert_contains(fixture.version(180), "\"groupCount\":23");
    assert_contains(fixture.version(180), "\"evidenceDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(fixture.version(181), "\"releaseVersion\":\"v181\"");
    assert_contains(fixture.version(181), "\"status\":\"node-route-catalog-integrity-window-read-only\"");
    assert_contains(fixture.version(181), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v460-route-catalog-integrity-window-read-only\"");
    assert_contains(fixture.version(181), "\"sourceFrozenReleaseVersion\":\"v180\"");
    assert_contains(fixture.version(181), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v180.json\"");
    assert_contains(fixture.version(181), "\"windowEndNodeVersion\":\"Node v460\"");
    assert_contains(fixture.version(181), "\"splitRouteGroupCount\":28");
    assert_contains(fixture.version(181), "\"computedWindowVersionSpan\":28");
    assert_contains(fixture.version(181), "\"sourceFrozenWindowDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(fixture.version(181), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(181), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(181), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(181), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(181), "\"executionAllowed\":false");
    assert_contains(fixture.version(181),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(fixture.version(181), "\"fieldCount\":431");
    assert_contains(fixture.version(181), "\"groupCount\":23");
    assert_contains(fixture.version(181), "\"evidenceDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(fixture.version(182), "\"releaseVersion\":\"v182\"");
    assert_contains(fixture.version(182), "\"status\":\"node-route-catalog-test-migration-window-read-only\"");
    assert_contains(fixture.version(182), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v462-route-catalog-test-migration-window-read-only\"");
    assert_contains(fixture.version(182), "\"sourceFrozenReleaseVersion\":\"v181\"");
    assert_contains(fixture.version(182), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v181.json\"");
    assert_contains(fixture.version(182), "\"windowEndNodeVersion\":\"Node v462\"");
    assert_contains(fixture.version(182), "\"splitRouteGroupCount\":30");
    assert_contains(fixture.version(182), "\"computedWindowVersionSpan\":30");
    assert_contains(fixture.version(182), "\"sourceFrozenWindowDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(fixture.version(182), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(182), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(182), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(182), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(182), "\"executionAllowed\":false");
    assert_contains(fixture.version(182), "\"evidenceDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(fixture.version(183), "\"releaseVersion\":\"v183\"");
    assert_contains(fixture.version(183), "\"status\":\"node-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(fixture.version(183), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v463-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(fixture.version(183), "\"sourceFrozenReleaseVersion\":\"v182\"");
    assert_contains(fixture.version(183), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v182.json\"");
    assert_contains(fixture.version(183), "\"windowEndNodeVersion\":\"Node v463\"");
    assert_contains(fixture.version(183), "\"splitRouteGroupCount\":31");
    assert_contains(fixture.version(183), "\"computedWindowVersionSpan\":31");
    assert_contains(fixture.version(183), "\"sourceFrozenWindowDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(fixture.version(183), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(183), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(183), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(183), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(183), "\"executionAllowed\":false");
    assert_contains(fixture.version(183), "\"evidenceDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(fixture.version(184), "\"releaseVersion\":\"v184\"");
    assert_contains(fixture.version(184), "\"status\":\"node-route-catalog-final-closeout-window-read-only\"");
    assert_contains(fixture.version(184), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                          "\"node-v433-v464-route-catalog-final-closeout-window-read-only\"");
    assert_contains(fixture.version(184), "\"sourceFrozenReleaseVersion\":\"v183\"");
    assert_contains(fixture.version(184), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v183.json\"");
    assert_contains(fixture.version(184), "\"windowEndNodeVersion\":\"Node v464\"");
    assert_contains(fixture.version(184), "\"splitRouteGroupCount\":32");
    assert_contains(fixture.version(184), "\"computedWindowVersionSpan\":32");
    assert_contains(fixture.version(184), "\"sourceFrozenWindowDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(fixture.version(184), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(184), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(184), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(184), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(184), "\"executionAllowed\":false");
    assert_contains(fixture.version(184), "\"evidenceDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(fixture.version(185), "\"releaseVersion\":\"v185\"");
    assert_contains(fixture.version(185), "\"status\":\"node-route-catalog-closeout-snapshot-read-only\"");
    assert_contains(fixture.version(185), "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                                          "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_contains(fixture.version(185), "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_contains(fixture.version(185), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_contains(fixture.version(185), "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(fixture.version(185), "\"focusedCloseoutTestFileCount\":7");
    assert_contains(fixture.version(185), "\"focusedCloseoutTestCount\":17");
    assert_contains(fixture.version(185), "\"nodeFullVitestFileCount\":393");
    assert_contains(fixture.version(185), "\"nodeFullVitestTestCount\":1221");
    assert_contains(fixture.version(185), "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(fixture.version(185), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(185), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(185), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(185), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(185), "\"executionAllowed\":false");
    assert_contains(fixture.version(185), "\"evidenceDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(fixture.version(186), "\"releaseVersion\":\"v186\"");
    assert_contains(fixture.version(186), "\"status\":\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(fixture.version(186), "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                                          "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(fixture.version(186), "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_contains(fixture.version(186), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_contains(fixture.version(186), "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(fixture.version(186), "\"windowEndNodeVersion\":\"Node v467\"");
    assert_contains(fixture.version(186), "\"splitRouteGroupCount\":35");
    assert_contains(fixture.version(186), "\"computedWindowVersionSpan\":35");
    assert_contains(fixture.version(186), "\"sourceAnchorCompatibilityRemoved\":true");
    assert_contains(fixture.version(186), "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_contains(fixture.version(186), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(186), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(186), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(186), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(186), "\"executionAllowed\":false");
    assert_contains(fixture.version(186), "\"evidenceDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(fixture.version(187), "\"releaseVersion\":\"v187\"");
    assert_contains(fixture.version(187),
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(fixture.version(187), "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                                          "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(fixture.version(187), "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_contains(fixture.version(187), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_contains(fixture.version(187), "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(fixture.version(187), "\"windowEndNodeVersion\":\"Node v471\"");
    assert_contains(fixture.version(187), "\"splitRouteGroupCount\":39");
    assert_contains(fixture.version(187), "\"computedWindowVersionSpan\":39");
    assert_contains(fixture.version(187), "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_contains(fixture.version(187), "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(fixture.version(187), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(187), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(187), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(187), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(187), "\"executionAllowed\":false");
    assert_contains(fixture.version(187), "\"evidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture.version(188), "\"releaseVersion\":\"v188\"");
    assert_contains(fixture.version(188),
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(fixture.version(188), "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                                          "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(fixture.version(188), "\"frozenReleaseVersion\":\"v187\"");
    assert_contains(fixture.version(188), "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_contains(fixture.version(188), "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture.version(188), "\"sourceFrozenWindowDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture.version(188), "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(fixture.version(188), "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_contains(fixture.version(188), "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(fixture.version(188), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(188), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(188), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(188), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(188), "\"executionAllowed\":false");
    assert_contains(fixture.version(188), "\"evidenceDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture.version(189), "\"releaseVersion\":\"v189\"");
    assert_contains(fixture.version(189), "\"status\":\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(fixture.version(189), "\"nodeRouteCatalogEvidenceChain\":{\"catalogMode\":"
                                          "\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(fixture.version(189), "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(fixture.version(189), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(fixture.version(189), "\"sourceFrozenDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture.version(189), "\"catalogedReleaseCount\":4");
    assert_contains(fixture.version(189), "\"releaseRangeEnd\":\"v188\"");
    assert_contains(fixture.version(189), "\"latestCatalogedDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture.version(189),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v17\"");
    assert_contains(fixture.version(189), "\"fieldCount\":547");
    assert_contains(fixture.version(189), "\"groupCount\":28");
    assert_contains(fixture.version(189), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(189), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(189), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(189), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(189), "\"executionAllowed\":false");
    assert_contains(fixture.version(189), "\"evidenceDigest\":\"fnv1a64:a595f2d9cdcbac8d\"");
    assert_contains(fixture.version(190), "\"releaseVersion\":\"v190\"");
    assert_contains(fixture.version(190), "\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(fixture.version(190), "\"nodeRouteCatalogEvidenceChainAudit\":{\"auditMode\":"
                                          "\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(fixture.version(190), "\"sourceFrozenReleaseVersion\":\"v189\"");
    assert_contains(fixture.version(190), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v189.json\"");
    assert_contains(fixture.version(190), "\"nodeCleanupCloseoutVersion\":\"Node v472\"");
    assert_contains(fixture.version(190), "\"nodeFullVitestTestCount\":1222");
    assert_contains(fixture.version(190),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_contains(fixture.version(190), "\"fieldCount\":573");
    assert_contains(fixture.version(190), "\"groupCount\":29");
    assert_contains(fixture.version(190), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(190), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(190), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(190), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(190), "\"executionAllowed\":false");
    assert_contains(fixture.version(190), "\"evidenceDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_contains(fixture.version(191), "\"releaseVersion\":\"v191\"");
    assert_contains(fixture.version(191), "\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(fixture.version(191), "\"nodeRouteCatalogCleanupCloseoutHandoff\":{\"handoffMode\":"
                                          "\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(fixture.version(191), "\"sourceFrozenReleaseVersion\":\"v190\"");
    assert_contains(fixture.version(191), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v190.json\"");
    assert_contains(fixture.version(191), "\"readyForDownstreamConsumption\":true");
    assert_contains(fixture.version(191), "\"handoffBoundaryCatalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_contains(fixture.version(191),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v19\"");
    assert_contains(fixture.version(191), "\"fieldCount\":597");
    assert_contains(fixture.version(191), "\"groupCount\":30");
    assert_contains(fixture.version(191), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(191), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(191), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(191), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(191), "\"executionAllowed\":false");
    assert_contains(fixture.version(191), "\"evidenceDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_contains(fixture.version(192), "\"releaseVersion\":\"v192\"");
    assert_contains(fixture.version(192), "\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(fixture.version(192), "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":{\"auditMode\":"
                                          "\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(fixture.version(192), "\"sourceFrozenReleaseVersion\":\"v191\"");
    assert_contains(fixture.version(192), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v191.json\"");
    assert_contains(fixture.version(192), "\"handoffIncludedSectionsComplete\":true");
    assert_contains(fixture.version(192), "\"rollingCurrentRejected\":true");
    assert_contains(fixture.version(192),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v20\"");
    assert_contains(fixture.version(192), "\"fieldCount\":625");
    assert_contains(fixture.version(192), "\"groupCount\":31");
    assert_contains(fixture.version(192), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(192), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(192), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(192), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(192), "\"executionAllowed\":false");
    assert_contains(fixture.version(192), "\"evidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_contains(fixture.version(193), "\"releaseVersion\":\"v193\"");
    assert_contains(fixture.version(193),
                    "\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_contains(fixture.version(193), "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":{\"freezeMode\":"
                                          "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_contains(fixture.version(193), "\"frozenReleaseVersion\":\"v192\"");
    assert_contains(fixture.version(193), "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v192.json\"");
    assert_contains(fixture.version(193), "\"frozenEvidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_contains(fixture.version(193),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v21\"");
    assert_contains(fixture.version(193), "\"fieldCount\":648");
    assert_contains(fixture.version(193), "\"groupCount\":32");
    assert_contains(fixture.version(193), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(193), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(193), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(193), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(193), "\"executionAllowed\":false");
    assert_contains(fixture.version(193), "\"evidenceDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(fixture.version(194), "\"releaseVersion\":\"v194\"");
    assert_contains(fixture.version(194),
                    "\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(fixture.version(194), "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":{\"catalogMode\":"
                                          "\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(fixture.version(194), "\"sourceFrozenReleaseVersion\":\"v193\"");
    assert_contains(fixture.version(194), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v193.json\"");
    assert_contains(fixture.version(194), "\"catalogedReleaseCount\":4");
    assert_contains(fixture.version(194), "\"latestCatalogedReleaseVersion\":\"v193\"");
    assert_contains(fixture.version(194), "\"latestCatalogedDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(fixture.version(194), "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(fixture.version(194),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v22\"");
    assert_contains(fixture.version(194), "\"fieldCount\":669");
    assert_contains(fixture.version(194), "\"groupCount\":33");
    assert_contains(fixture.version(194), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(194), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(194), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(194), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(194), "\"executionAllowed\":false");
    assert_contains(fixture.version(194), "\"evidenceDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_contains(fixture.version(195), "\"releaseVersion\":\"v195\"");
    assert_contains(fixture.version(195),
                    "\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-audit-read-only\"");
    assert_contains(fixture.version(195),
                    "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":{\"auditMode\":"
                    "\"node-route-catalog-cleanup-closeout-release-catalog-consistency-read-only\"");
    assert_contains(fixture.version(195), "\"sourceFrozenReleaseVersion\":\"v194\"");
    assert_contains(fixture.version(195), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v194.json\"");
    assert_contains(fixture.version(195), "\"sourceFrozenDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_contains(fixture.version(195), "\"observedCatalogedReleaseCount\":4");
    assert_contains(fixture.version(195), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture.version(195), "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(fixture.version(195),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v23\"");
    assert_contains(fixture.version(195), "\"fieldCount\":693");
    assert_contains(fixture.version(195), "\"groupCount\":34");
    assert_contains(fixture.version(195), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(195), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(195), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(195), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(195), "\"executionAllowed\":false");
    assert_contains(fixture.version(195), "\"evidenceDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_contains(fixture.version(196), "\"releaseVersion\":\"v196\"");
    assert_contains(fixture.version(196),
                    "\"status\":\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_contains(fixture.version(196), "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":{\"maintenanceMode\":"
                                          "\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_contains(fixture.version(196), "\"sourceFrozenReleaseVersion\":\"v195\"");
    assert_contains(fixture.version(196), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v195.json\"");
    assert_contains(fixture.version(196), "\"sourceFrozenDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_contains(fixture.version(196), "\"extractedCatalogFormatter\":true");
    assert_contains(fixture.version(196), "\"preservesReleaseCatalogAudit\":true");
    assert_contains(fixture.version(196),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v24\"");
    assert_contains(fixture.version(196), "\"fieldCount\":714");
    assert_contains(fixture.version(196), "\"groupCount\":35");
    assert_contains(fixture.version(196), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(196), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(196), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(196), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(196), "\"executionAllowed\":false");
    assert_contains(fixture.version(196), "\"evidenceDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture.version(197), "\"releaseVersion\":\"v197\"");
    assert_contains(fixture.version(197),
                    "\"status\":\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_contains(fixture.version(197), "\"nodeRouteCatalogCleanupLatestEvidencePackage\":{\"packageMode\":"
                                          "\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_contains(fixture.version(197), "\"sourceFrozenReleaseVersion\":\"v196\"");
    assert_contains(fixture.version(197), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v196.json\"");
    assert_contains(fixture.version(197), "\"sourceFrozenDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture.version(197), "\"packageReleaseCount\":3");
    assert_contains(fixture.version(197), "\"latestPackagedReleaseVersion\":\"v196\"");
    assert_contains(fixture.version(197), "\"latestPackagedDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture.version(197), "\"packageReadyForArchiveVerification\":true");
    assert_contains(fixture.version(197),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v25\"");
    assert_contains(fixture.version(197), "\"fieldCount\":735");
    assert_contains(fixture.version(197), "\"groupCount\":36");
    assert_contains(fixture.version(197), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(197), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(197), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(197), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(197), "\"executionAllowed\":false");
    assert_contains(fixture.version(197), "\"evidenceDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_contains(fixture.version(198), "\"releaseVersion\":\"v198\"");
    assert_contains(fixture.version(198),
                    "\"status\":\"node-route-catalog-cleanup-latest-evidence-package-audit-read-only\"");
    assert_contains(fixture.version(198),
                    "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":{\"auditMode\":"
                    "\"node-route-catalog-cleanup-latest-evidence-package-consistency-read-only\"");
    assert_contains(fixture.version(198), "\"sourceFrozenReleaseVersion\":\"v197\"");
    assert_contains(fixture.version(198), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v197.json\"");
    assert_contains(fixture.version(198), "\"sourceFrozenDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_contains(fixture.version(198), "\"observedPackageReleaseCount\":3");
    assert_contains(fixture.version(198), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture.version(198), "\"latestPackagedDigestMatchesPackageSource\":true");
    assert_contains(fixture.version(198), "\"packageReadyForArchiveVerification\":true");
    assert_contains(fixture.version(198),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v26\"");
    assert_contains(fixture.version(198), "\"fieldCount\":756");
    assert_contains(fixture.version(198), "\"groupCount\":37");
    assert_contains(fixture.version(198), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(198), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(198), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(198), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(198), "\"executionAllowed\":false");
    assert_contains(fixture.version(198), "\"evidenceDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture.version(199), "\"releaseVersion\":\"v199\"");
    assert_contains(fixture.version(199),
                    "\"status\":\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_contains(fixture.version(199), "\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":{\"closeoutMode\":"
                                          "\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_contains(fixture.version(199), "\"sourceFrozenReleaseVersion\":\"v198\"");
    assert_contains(fixture.version(199), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_contains(fixture.version(199), "\"sourceFrozenDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture.version(199), "\"miniKvCloseoutReleaseCount\":5");
    assert_contains(fixture.version(199), "\"latestCloseoutReleaseVersion\":\"v198\"");
    assert_contains(fixture.version(199), "\"latestCloseoutDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture.version(199), "\"closeoutReadyForNextBatch\":true");
    assert_contains(fixture.version(199),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v27\"");
    assert_contains(fixture.version(199), "\"fieldCount\":778");
    assert_contains(fixture.version(199), "\"groupCount\":38");
    assert_contains(fixture.version(199), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(199), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(199), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(199), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(199), "\"executionAllowed\":false");
    assert_contains(fixture.version(199), "\"evidenceDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_contains(fixture.version(200), "\"releaseVersion\":\"v200\"");
    assert_contains(fixture.version(200),
                    "\"status\":\"node-route-catalog-cleanup-evidence-batch-closeout-audit-read-only\"");
    assert_contains(fixture.version(200),
                    "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":{\"auditMode\":"
                    "\"node-route-catalog-cleanup-evidence-batch-closeout-consistency-read-only\"");
    assert_contains(fixture.version(200), "\"sourceFrozenReleaseVersion\":\"v199\"");
    assert_contains(fixture.version(200), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v199.json\"");
    assert_contains(fixture.version(200), "\"sourceFrozenDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_contains(fixture.version(200), "\"observedCloseoutReleaseCount\":5");
    assert_contains(fixture.version(200), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture.version(200), "\"latestCloseoutDigestMatchesCloseoutSource\":true");
    assert_contains(fixture.version(200), "\"closeoutReadyForNextBatch\":true");
    assert_contains(fixture.version(200),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v28\"");
    assert_contains(fixture.version(200), "\"fieldCount\":800");
    assert_contains(fixture.version(200), "\"groupCount\":39");
    assert_contains(fixture.version(200), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(200), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(200), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(200), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(200), "\"executionAllowed\":false");
    assert_contains(fixture.version(200), "\"evidenceDigest\":\"fnv1a64:d1e889711b5d8574\"");
}

} // namespace minikv::shard_test
