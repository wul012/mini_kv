#include "minikv/shard_readiness_history.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::history {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_diagnostics_json() {
    return "{\"writeCommandsExecuted\":false,\"adminCommandsExecuted\":false,"
           "\"loadRestoreCompactExecuted\":false,"
           "\"nodeConsumer\":\"Node v471+ may consume v189 as a versioned route catalog evidence chain only\","
           "\"javaEchoExpected\":\"Java shard-readiness echo may consume the same shard-readiness.v1 fields\","
           "\"nodeArchivedEvidencePreserved\":true}";
}

std::string format_fixture_parity_json() {
    return "{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\","
           "\"historicalFixturePaths\":" +
           json_string_array({
               "fixtures/release/shard-readiness-v144.json",
               "fixtures/release/shard-readiness-v145.json",
               "fixtures/release/shard-readiness-v146.json",
               "fixtures/release/shard-readiness-v147.json",
               "fixtures/release/shard-readiness-v148.json",
               "fixtures/release/shard-readiness-v149.json",
               "fixtures/release/shard-readiness-v150.json",
               "fixtures/release/shard-readiness-v151.json",
               "fixtures/release/shard-readiness-v152.json",
               "fixtures/release/shard-readiness-v153.json",
               "fixtures/release/shard-readiness-v154.json",
               "fixtures/release/shard-readiness-v155.json",
               "fixtures/release/shard-readiness-v156.json",
               "fixtures/release/shard-readiness-v157.json",
               "fixtures/release/shard-readiness-v158.json",
               "fixtures/release/shard-readiness-v159.json",
               "fixtures/release/shard-readiness-v160.json",
               "fixtures/release/shard-readiness-v161.json",
               "fixtures/release/shard-readiness-v162.json",
               "fixtures/release/shard-readiness-v163.json",
               "fixtures/release/shard-readiness-v164.json",
               "fixtures/release/shard-readiness-v165.json",
               "fixtures/release/shard-readiness-v166.json",
               "fixtures/release/shard-readiness-v167.json",
               "fixtures/release/shard-readiness-v168.json",
               "fixtures/release/shard-readiness-v169.json",
               "fixtures/release/shard-readiness-v170.json",
               "fixtures/release/shard-readiness-v171.json",
               "fixtures/release/shard-readiness-v172.json",
               "fixtures/release/shard-readiness-v173.json",
               "fixtures/release/shard-readiness-v174.json",
               "fixtures/release/shard-readiness-v175.json",
               "fixtures/release/shard-readiness-v176.json",
               "fixtures/release/shard-readiness-v177.json",
               "fixtures/release/shard-readiness-v178.json",
               "fixtures/release/shard-readiness-v179.json",
               "fixtures/release/shard-readiness-v180.json",
               "fixtures/release/shard-readiness-v181.json",
               "fixtures/release/shard-readiness-v182.json",
               "fixtures/release/shard-readiness-v183.json",
                "fixtures/release/shard-readiness-v184.json",
                "fixtures/release/shard-readiness-v185.json",
                "fixtures/release/shard-readiness-v186.json",
                "fixtures/release/shard-readiness-v187.json",
                "fixtures/release/shard-readiness-v188.json",
            }) +
           ",\"runtimeMatchesCurrentFixture\":true,\"historicalFixturesPreserved\":true}";
}

std::string format_archive_compatibility_json() {
    return "{\"preservesNodeArchivedEvidence\":true,"
           "\"archivedNodeVersions\":" +
           json_string_array({
               "Node v370",
               "Node v371",
               "Node v372",
               "Node v373",
               "Node v374",
               "Node v375",
               "Node v376",
               "Node v377",
               "Node v378",
               "Node v379",
               "Node v380",
               "Node v381",
               "Node v382",
               "Node v383",
               "Node v384",
               "Node v385",
               "Node v386",
               "Node v387",
               "Node v388",
               "Node v389",
               "Node v390",
               "Node v391",
               "Node v392",
               "Node v393",
               "Node v394",
               "Node v395",
               "Node v396",
               "Node v397",
               "Node v398",
               "Node v399",
               "Node v400",
               "Node v401",
               "Node v402",
               "Node v403",
               "Node v418",
               "Node v419",
               "Node v420",
               "Node v421",
               "Node v422",
               "Node v423",
               "Node v424",
               "Node v425",
               "Node v426",
               "Node v427",
               "Node v428",
               "Node v429",
               "Node v430",
               "Node v431",
               "Node v432",
               "Node v433",
               "Node v434",
               "Node v435",
               "Node v436",
               "Node v437",
               "Node v438",
               "Node v439",
               "Node v440",
               "Node v441",
               "Node v442",
               "Node v443",
               "Node v444",
               "Node v445",
               "Node v446",
               "Node v447",
               "Node v448",
               "Node v449",
               "Node v450",
               "Node v451",
               "Node v452",
               "Node v453",
               "Node v454",
               "Node v455",
               "Node v456",
               "Node v457",
               "Node v458",
               "Node v459",
               "Node v460",
               "Node v461",
               "Node v462",
               "Node v463",
               "Node v464",
               "Node v465",
               "Node v466",
               "Node v467",
                "Node v468",
                "Node v469",
                "Node v470",
                "Node v471",
            }) +
            ",\"changesArchivedNodeEvidence\":false,"
            "\"futureNodeConsumer\":\"Node v471 or later may consume this versioned route catalog evidence chain without changing mini-kv runtime boundaries\"}";
}

std::string format_historical_fallback_json() {
    return "{\"previousConsumedReleaseVersion\":\"v188\","
           "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v188.json\","
           "\"previousConsumptionNodeVersion\":\"Node v471 route catalog evidence chain may consume v188 frozen audit evidence\","
           "\"olderPrototypeFixturePath\":\"fixtures/release/shard-readiness-v144.json\","
           "\"rollingCurrentUsedForHistoricalBaseline\":false,"
           "\"nodeV396ProgressIntakePreserved\":true,"
           "\"nodeV397ContributionReviewPreserved\":true,"
           "\"nodeV398ApprovalGateReviewPreserved\":true,"
           "\"nodeV399ArchiveVerificationPreserved\":true,"
           "\"nodeV400InputContractPreserved\":true,"
           "\"nodeV401CompletionIntakePreserved\":true,"
           "\"nodeV402TemplateValidatorPreserved\":true,"
           "\"nodeV403TemplateCompatibilityIntakePreserved\":true,"
           "\"nodeV404RequiresRealCanonicalInputs\":true,"
           "\"nodeV418RouteGroupSplitCompatibilityPreserved\":true,"
           "\"nodeV419BoundaryCatalogMaintenanceReady\":true,"
           "\"nodeV420PreImplementationReadinessRouteSplitPreserved\":true,"
           "\"nodeV421ApprovalRequiredReadinessRouteSplitPreserved\":true,"
           "\"nodeV422ImplementationPlanRouteSplitPreserved\":true,"
           "\"nodeV423FakeHarnessContractRouteSplitPreserved\":true,"
           "\"nodeV424FakeHarnessExecutionReadinessRouteSplitPreserved\":true,"
           "\"nodeV425DisabledRuntimeShellReadinessRouteSplitPreserved\":true,"
           "\"nodeV426RuntimeShellDecisionRouteSplitPreserved\":true,"
           "\"nodeV427RuntimeShellPostDecisionRouteSplitPreserved\":true,"
           "\"nodeV428RuntimeShellPrerequisiteRouteSplitPreserved\":true,"
           "\"nodeV429HumanApprovalArtifactReviewRouteSplitPreserved\":true,"
           "\"nodeV430SignedHumanApprovalArtifactRouteSplitPreserved\":true,"
           "\"nodeV431CredentialHandleApprovalRouteSplitPreserved\":true,"
           "\"nodeV432EndpointHandleAllowlistApprovalRouteSplitPreserved\":true,"
           "\"nodeV433NoNetworkSafetyFixtureRouteSplitPreserved\":true,"
           "\"nodeV434FinalPrerequisiteRouteSplitPreserved\":true,"
           "\"nodeV435ImplementationCandidateGateRouteSplitPreserved\":true,"
           "\"nodeV436DesignDraftCandidateRouteSplitPreserved\":true,"
           "\"nodeV437DesignDraftOutlineRouteSplitPreserved\":true,"
           "\"nodeV438DesignDraftBodyIntakeRouteSplitPreserved\":true,"
           "\"nodeV439DesignDraftBodyCandidateRouteSplitPreserved\":true,"
           "\"nodeV440DesignDraftBodyPreDraftDecisionRouteSplitPreserved\":true,"
           "\"nodeV441DesignDraftBodyPreparationPlanRouteSplitPreserved\":true,"
           "\"nodeV442DesignDraftBodyDraftCandidateRouteSplitPreserved\":true,"
           "\"nodeV443ManagedAuditAdapterRouteSplitPreserved\":true,"
           "\"nodeV444ManagedAuditPersistenceRouteSplitPreserved\":true,"
           "\"nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved\":true,"
           "\"nodeV446ManagedAuditRestoreDrillRouteSplitPreserved\":true,"
           "\"nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved\":true,"
           "\"nodeV448ManagedAuditRouteQualityRouteSplitPreserved\":true,"
           "\"nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved\":true,"
           "\"nodeV450ManagedAuditLocalAdapterRouteSplitPreserved\":true,"
           "\"nodeV451ManagedAuditSandboxAdapterRouteSplitPreserved\":true,"
           "\"nodeV452ManualSandboxConnectionPacketRouteSplitPreserved\":true,"
           "\"nodeV453ManualSandboxConnectionReadinessRouteSplitPreserved\":true,"
           "\"nodeV454ManualSandboxConnectionCommandRouteSplitPreserved\":true,"
           "\"nodeV455ManualSandboxConnectionPrecheckRouteSplitPreserved\":true,"
           "\"nodeV456ManualSandboxConnectionAdapterClientRouteSplitPreserved\":true,"
           "\"nodeV457ManualSandboxConnectionFakeTransportRouteSplitPreserved\":true,"
           "\"nodeV458FoundationalAuditRouteSplitPreserved\":true,"
           "\"nodeV459AuditRouteGroupCatalogPreserved\":true,"
           "\"nodeV460AuditRouteCatalogIntegrityPreserved\":true,"
           "\"nodeV461AuditRouteCatalogTestSupportPreserved\":true,"
           "\"nodeV462AuditRouteCatalogTestMigrationPreserved\":true,"
           "\"nodeV463AuditRouteQualityCatalogIntegrityPreserved\":true,"
           "\"nodeV464AuditRouteCatalogFinalCloseoutPreserved\":true,"
           "\"nodeV464AuditRouteCatalogFinalCloseoutSnapshotPreserved\":true,"
           "\"nodeV465CodeHealthRouteGroupEvidencePreserved\":true,"
           "\"nodeV466CentralRouteAnchorExtractionPreserved\":true,"
           "\"nodeV467RouteCatalogAnchorRemovalPreserved\":true,"
           "\"nodeV468RouteCatalogIntegrityAnchorFieldRemovalPreserved\":true,"
            "\"nodeV469RouteCatalogSummaryBuilderPreserved\":true,"
            "\"nodeV470RouteCatalogExpectedSummaryPreserved\":true,"
            "\"nodeV471RouteCatalogExpectedIntegritySnapshotPreserved\":true}";
}

std::string format_history_formatter_maintenance_json() {
    return "{\"maintenanceMode\":\"history-fixture-archive-formatter-split-read-only\","
           "\"sourceFrozenReleaseVersion\":\"v179\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\","
           "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\","
           "\"splitFrom\":\"src/shard_readiness.cpp\","
           "\"splitInto\":\"src/shard_readiness_history.cpp\","
           "\"preservesFixtureParity\":true,"
           "\"preservesArchiveCompatibility\":true,"
           "\"preservesHistoricalFallback\":true,"
           "\"changesShardJsonCommand\":false,"
           "\"changesFixturePath\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::history
