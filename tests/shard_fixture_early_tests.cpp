#include "shard_test_suites.hpp"

namespace {

std::map<int, std::string> read_shard_readiness_fixtures(int first_release, int last_release) {
    assert(first_release <= last_release);

    std::map<int, std::string> fixtures;
    for (int release_version = first_release; release_version <= last_release; ++release_version) {
        fixtures.emplace(release_version, minikv::test_support::read_shard_readiness_fixture(release_version));
    }
    return fixtures;
}

void assert_fixture_differs_from_each(const std::string& fixture,
                                      const std::map<int, std::string>& historical_fixtures) {
    for (const auto& [release_version, historical_fixture] : historical_fixtures) {
        if (fixture == historical_fixture) {
            std::cerr << "current fixture unexpectedly matched v" << release_version << '\n';
        }
        assert(fixture != historical_fixture);
    }
}

} // namespace

namespace minikv::shard_test {

ShardFixture load_shard_fixture() {
    return {
        read_fixture_text(std::filesystem::path{"fixtures"} / "release" / "shard-readiness.json"),
        read_shard_readiness_fixtures(144, 200),
        shard_readiness::format_json(),
    };
}

void run_shard_fixture_early_tests(ShardFixture& fixture) {
    if (fixture.current != fixture.formatted) {
        std::cerr << "fixture length=" << fixture.current.size() << " formatted length=" << fixture.formatted.size()
                  << '\n';
        const auto limit = std::min(fixture.current.size(), fixture.formatted.size());
        for (std::size_t index = 0; index < limit; ++index) {
            if (fixture.current[index] != fixture.formatted[index]) {
                std::cerr << "first diff at " << index
                          << " fixture=" << static_cast<int>(static_cast<unsigned char>(fixture.current[index]))
                          << " formatted=" << static_cast<int>(static_cast<unsigned char>(fixture.formatted[index]))
                          << '\n';
                break;
            }
        }
    }
    assert(fixture.current == fixture.formatted);
    assert(minikv::shard_readiness::fixture_path() == "fixtures/release/shard-readiness.json");
    assert_shard_readiness_contract(fixture, fixture.current);
    assert_fixture_differs_from_each(fixture.current, fixture.historical);
    assert_contains(fixture.version(144), "\"releaseVersion\":\"v144\"");
    assert_contains(fixture.version(144), "\"status\":\"prototype-ready-read-only\"");
    assert_contains(fixture.version(144), "\"evidenceDigest\":\"fnv1a64:22d3c4815a440804\"");
    assert_contains(fixture.version(145), "\"releaseVersion\":\"v145\"");
    assert_contains(fixture.version(145), "\"status\":\"hardened-read-only\"");
    assert_contains(fixture.version(145), "\"evidenceDigest\":\"fnv1a64:ebe4c7e1a2704482\"");
    assert_contains(fixture.version(145), "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(fixture.version(146), "\"releaseVersion\":\"v146\"");
    assert_contains(fixture.version(146), "\"status\":\"historical-fallback-hardened-read-only\"");
    assert_contains(fixture.version(146),
                    "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v145.json\"");
    assert_contains(fixture.version(146), "\"evidenceDigest\":\"fnv1a64:6847d87decb76fcb\"");
    assert_contains(fixture.version(147), "\"releaseVersion\":\"v147\"");
    assert_contains(fixture.version(147), "\"status\":\"active-prototype-prerequisite-read-only\"");
    assert_contains(fixture.version(147), "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(fixture.version(147), "\"activeShardPrototypeAllowed\":false");
    assert_contains(fixture.version(147), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(147), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(147), "\"evidenceDigest\":\"fnv1a64:e4a386fc9add4eaf\"");
    assert_contains(fixture.version(148), "\"releaseVersion\":\"v148\"");
    assert_contains(fixture.version(148), "\"status\":\"active-prototype-plan-frozen-read-only\"");
    assert_contains(fixture.version(148), "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v147\"");
    assert_contains(fixture.version(148), "\"frozenRouterActivationAllowed\":false");
    assert_contains(fixture.version(148), "\"frozenWriteRoutingAllowed\":false");
    assert_contains(fixture.version(148), "\"evidenceDigest\":\"fnv1a64:8270eeebbf1a7f29\"");
    assert_contains(fixture.version(149), "\"releaseVersion\":\"v149\"");
    assert_contains(fixture.version(149), "\"status\":\"frozen-evidence-handoff-read-only\"");
    assert_contains(fixture.version(149), "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v148\"");
    assert_contains(fixture.version(149), "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(fixture.version(149), "\"startsServices\":false");
    assert_contains(fixture.version(149), "\"executionAllowed\":false");
    assert_contains(fixture.version(149), "\"evidenceDigest\":\"fnv1a64:521fe6dee47f7f2c\"");
    assert_contains(fixture.version(150), "\"releaseVersion\":\"v150\"");
    assert_contains(fixture.version(150), "\"status\":\"live-read-gate-prerequisite-read-only\"");
    assert_contains(fixture.version(150), "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(fixture.version(150), "\"liveReadGateAllowed\":false");
    assert_contains(fixture.version(150), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture.version(150), "\"startsServices\":false");
    assert_contains(fixture.version(150), "\"evidenceDigest\":\"fnv1a64:b8b134f6aa527ca4\"");
    assert_contains(fixture.version(151), "\"releaseVersion\":\"v151\"");
    assert_contains(fixture.version(151), "\"status\":\"operator-service-lifecycle-template-read-only\"");
    assert_contains(fixture.version(151),
                    "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(fixture.version(151), "\"serviceOwnerDeclared\":false");
    assert_contains(fixture.version(151), "\"startupCommandDeclared\":false");
    assert_contains(fixture.version(151), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture.version(151), "\"evidenceDigest\":\"fnv1a64:c9fb568ddff895e3\"");
    assert_contains(fixture.version(152), "\"releaseVersion\":\"v152\"");
    assert_contains(fixture.version(152), "\"status\":\"declared-operator-lifecycle-no-runtime-read-only\"");
    assert_contains(fixture.version(152),
                    "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(fixture.version(152), "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(fixture.version(152), "\"serviceOwnerDeclared\":true");
    assert_contains(fixture.version(152), "\"runtimeGateApproved\":false");
    assert_contains(fixture.version(152), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture.version(152), "\"executionAllowed\":false");
    assert_contains(fixture.version(152), "\"evidenceDigest\":\"fnv1a64:55cd5b8db109c64f\"");
    assert_contains(fixture.version(153), "\"releaseVersion\":\"v153\"");
    assert_contains(fixture.version(153),
                    "\"status\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(fixture.version(153), "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                                          "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(fixture.version(153), "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(fixture.version(153), "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(fixture.version(153), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(153), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(153), "\"executionAllowed\":false");
    assert_contains(fixture.version(153), "\"evidenceDigest\":\"fnv1a64:ca09095f3f3e4b2c\"");
    assert_contains(fixture.version(154), "\"releaseVersion\":\"v154\"");
    assert_contains(fixture.version(154),
                    "\"status\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(fixture.version(154), "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                                          "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(fixture.version(154), "\"candidateArtifactCount\":4");
    assert_contains(fixture.version(154), "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(fixture.version(154), "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(fixture.version(154), "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(fixture.version(154), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(154), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(154), "\"executionAllowed\":false");
    assert_contains(fixture.version(154), "\"evidenceDigest\":\"fnv1a64:edbe9c546c0d780a\"");
    assert_contains(fixture.version(155), "\"releaseVersion\":\"v155\"");
    assert_contains(fixture.version(155),
                    "\"status\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(fixture.version(155), "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                                          "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(fixture.version(155), "\"approvalGateInputCount\":0");
    assert_contains(fixture.version(155), "\"missingApprovalGateInputCount\":3");
    assert_contains(fixture.version(155), "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(fixture.version(155), "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(fixture.version(155), "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(fixture.version(155), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(155), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(155), "\"executionAllowed\":false");
    assert_contains(fixture.version(155), "\"evidenceDigest\":\"fnv1a64:edb1c4d8534461ab\"");
    assert_contains(fixture.version(156), "\"releaseVersion\":\"v156\"");
    assert_contains(fixture.version(156), "\"status\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(fixture.version(156),
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(fixture.version(156), "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(fixture.version(156), "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(fixture.version(156), "\"miniKvLoopbackPort\":6424");
    assert_contains(fixture.version(156), "\"cleanupProofPresent\":false");
    assert_contains(fixture.version(156), "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(fixture.version(156), "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(fixture.version(156), "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(fixture.version(156), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(156), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(156), "\"executionAllowed\":false");
    assert_contains(fixture.version(156), "\"evidenceDigest\":\"fnv1a64:f240d1fe1b25fab7\"");
    assert_contains(fixture.version(157), "\"releaseVersion\":\"v157\"");
    assert_contains(fixture.version(157),
                    "\"status\":\"runtime-execution-approval-input-template-validator-echo-read-only\"");
    assert_contains(fixture.version(157), "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                                          "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(fixture.version(157), "\"templateOnlyInputCount\":3");
    assert_contains(fixture.version(157), "\"canonicalRuntimeInputPresent\":false");
    assert_contains(fixture.version(157), "\"templateCopiedToCanonicalInput\":false");
    assert_contains(fixture.version(157), "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(fixture.version(157), "\"templatesAuthorizeRuntime\":false");
    assert_contains(fixture.version(157), "\"templateDigestAcceptedAsApproval\":false");
    assert_contains(fixture.version(157), "\"writesCanonicalApprovalInputFiles\":false");
    assert_contains(fixture.version(157), "\"changesNodeInputTemplateFiles\":false");
    assert_contains(fixture.version(157), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(157), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(157), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(157), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(157), "\"executionAllowed\":false");
    assert_contains(fixture.version(157), "\"evidenceDigest\":\"fnv1a64:1d8cdf7f597f837e\"");
    assert_contains(fixture.version(158), "\"releaseVersion\":\"v158\"");
    assert_contains(fixture.version(158),
                    "\"status\":\"runtime-execution-canonical-approval-input-precheck-read-only\"");
    assert_contains(fixture.version(158), "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                                          "\"blocked-missing-canonical-approval-inputs\"");
    assert_contains(fixture.version(158), "\"requiredCanonicalInputCount\":3");
    assert_contains(fixture.version(158), "\"presentCanonicalInputCount\":0");
    assert_contains(fixture.version(158), "\"missingCanonicalInputCount\":3");
    assert_contains(fixture.version(158), "\"canonicalApprovalInputsComplete\":false");
    assert_contains(fixture.version(158), "\"sharedApprovalCorrelationIdValidated\":false");
    assert_contains(fixture.version(158), "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_contains(fixture.version(158), "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_contains(fixture.version(158), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(158), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(158), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(158), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(158), "\"executionAllowed\":false");
    assert_contains(fixture.version(158), "\"evidenceDigest\":\"fnv1a64:bce4629123167bc6\"");
    assert_contains(fixture.version(159), "\"releaseVersion\":\"v159\"");
    assert_contains(fixture.version(159), "\"status\":\"node-route-group-split-compatibility-read-only\"");
    assert_contains(fixture.version(159), "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                                          "\"node-route-group-split-contract-stable\"");
    assert_contains(fixture.version(159),
                    "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_contains(fixture.version(159), "\"sourceFrozenReleaseVersion\":\"v158\"");
    assert_contains(fixture.version(159), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v158.json\"");
    assert_contains(fixture.version(159), "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_contains(fixture.version(159), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(159), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(159), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(159), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(159), "\"executionAllowed\":false");
    assert_contains(fixture.version(159), "\"evidenceDigest\":\"fnv1a64:0741c3475ee371e2\"");
    assert_contains(fixture.version(160), "\"releaseVersion\":\"v160\"");
    assert_contains(fixture.version(160), "\"status\":\"boundary-field-catalog-split-read-only\"");
    assert_contains(fixture.version(160), "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                                          "\"boundary-field-catalog-split-contract-preserving\"");
    assert_contains(fixture.version(160), "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_contains(fixture.version(160), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_contains(fixture.version(160), "\"boundaryFieldCatalogExtracted\":true");
    assert_contains(fixture.version(160), "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(fixture.version(160), "\"publicShardJsonContractChanged\":false");
    assert_contains(fixture.version(160), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(160), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(160), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(160), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(160), "\"executionAllowed\":false");
    assert_contains(fixture.version(160), "\"evidenceDigest\":\"fnv1a64:c96ae9497cfd6386\"");
    assert_contains(fixture.version(161), "\"releaseVersion\":\"v161\"");
    assert_contains(fixture.version(161), "\"status\":\"boundary-field-catalog-index-read-only\"");
    assert_contains(fixture.version(161),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v2\"");
    assert_contains(fixture.version(161), "\"sourceFrozenReleaseVersion\":\"v160\"");
    assert_contains(fixture.version(161), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v160.json\"");
    assert_contains(fixture.version(161), "\"fieldCount\":290");
    assert_contains(fixture.version(161), "\"groupCount\":15");
    assert_contains(fixture.version(161), "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(fixture.version(161), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(161), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(161), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(161), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(161), "\"executionAllowed\":false");
    assert_contains(fixture.version(161), "\"evidenceDigest\":\"fnv1a64:2193962195ba633a\"");
    assert_contains(fixture.version(162), "\"releaseVersion\":\"v162\"");
    assert_contains(fixture.version(162), "\"status\":\"slot-table-preview-read-only\"");
    assert_contains(fixture.version(162),
                    "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_contains(fixture.version(162), "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_contains(fixture.version(162), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_contains(fixture.version(162), "\"slotTablePreviewOnly\":true");
    assert_contains(fixture.version(162), "\"readOnlyShardMapPreview\":true");
    assert_contains(fixture.version(162), "\"activeRouterInstalled\":false");
    assert_contains(fixture.version(162), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(162), "\"executionAllowed\":false");
    assert_contains(fixture.version(162),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v3\"");
    assert_contains(fixture.version(162), "\"fieldCount\":303");
    assert_contains(fixture.version(162), "\"groupCount\":16");
    assert_contains(fixture.version(162), "\"evidenceDigest\":\"fnv1a64:8c01a3657f07cfd1\"");
    assert_contains(fixture.version(163), "\"releaseVersion\":\"v163\"");
    assert_contains(fixture.version(163), "\"status\":\"slot-table-preview-audit-read-only\"");
    assert_contains(fixture.version(163), "\"slotTablePreviewAudit\":{\"auditMode\":"
                                          "\"slot-table-preview-consistency-read-only\"");
    assert_contains(fixture.version(163), "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_contains(fixture.version(163), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_contains(fixture.version(163), "\"observedSlotCount\":16");
    assert_contains(fixture.version(163), "\"contiguousSlotRange\":true");
    assert_contains(fixture.version(163), "\"duplicateSlotsDetected\":false");
    assert_contains(fixture.version(163), "\"unassignedSlotsDetected\":false");
    assert_contains(fixture.version(163), "\"allAssignmentsReadOnly\":true");
    assert_contains(fixture.version(163), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(163), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(163), "\"executionAllowed\":false");
    assert_contains(fixture.version(163),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v4\"");
    assert_contains(fixture.version(163), "\"fieldCount\":319");
    assert_contains(fixture.version(163), "\"groupCount\":17");
    assert_contains(fixture.version(163), "\"evidenceDigest\":\"fnv1a64:f81c5b47cb396df6\"");
    assert_contains(fixture.version(164), "\"releaseVersion\":\"v164\"");
    assert_contains(fixture.version(164), "\"status\":\"slot-table-preview-audit-maintenance-read-only\"");
    assert_contains(fixture.version(164), "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                                          "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_contains(fixture.version(164), "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_contains(fixture.version(164), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_contains(fixture.version(164), "\"auditFormatterExtracted\":true");
    assert_contains(fixture.version(164), "\"slotPreviewModuleOwnsAssignments\":true");
    assert_contains(fixture.version(164), "\"slotPreviewAuditStillPublished\":true");
    assert_contains(fixture.version(164), "\"slotTablePreviewStillPublished\":true");
    assert_contains(fixture.version(164), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture.version(164), "\"startsJavaService\":false");
    assert_contains(fixture.version(164), "\"startsMiniKvService\":false");
    assert_contains(fixture.version(164), "\"routerActivationAllowed\":false");
    assert_contains(fixture.version(164), "\"writeRoutingAllowed\":false");
    assert_contains(fixture.version(164), "\"executionAllowed\":false");
    assert_contains(fixture.version(164),
                    "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_contains(fixture.version(164), "\"fieldCount\":337");
    assert_contains(fixture.version(164), "\"groupCount\":18");
    assert_contains(fixture.version(164), "\"evidenceDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
}

} // namespace minikv::shard_test
