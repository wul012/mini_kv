#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_checks.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_stages.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_validation.hpp"
#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_blocked_execution_rehearsal_split_non_participation_checks;
namespace source_disabled_fake_harness =
    shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation;
namespace stages = shard_preview_blocked_execution_rehearsal_split_non_participation_stages;
namespace validation = shard_preview_blocked_execution_rehearsal_split_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1968-v1982-blocked-execution-rehearsal-split-roadmap.md";
constexpr int current_stage_count = 20;
constexpr int source_disabled_fake_harness_planned_stage_count = 15;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;
constexpr bool mini_kv_reads_endpoints = false;
constexpr bool mini_kv_reads_credentials = false;
constexpr bool mini_kv_parses_historical_receipts = false;
constexpr bool mini_kv_scans_java_evidence = false;
constexpr bool mini_kv_replays_wal_regression = false;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return stages::blocked_execution_rehearsal_split_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::blocked_execution_rehearsal_split_non_participation_stages(),
        current_stage_count,
        stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count(),
        stages::first_blocked_execution_rehearsal_split_non_participation_release_number());
}

bool source_disabled_fake_harness_complete() {
    return source_disabled_fake_harness::published_stage_count() ==
           source_disabled_fake_harness_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_blocked_execution_rehearsal_split_non_participation_check_count();
}

} // namespace

std::string format_blocked_execution_rehearsal_split_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::blocked_execution_rehearsal_split_non_participation_stages(),
        current_stage_count);
}

std::string blocked_execution_rehearsal_split_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_blocked_execution_rehearsal_split_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_disabled_fake_harness::published_stage_count();
    const bool source_complete = source_disabled_fake_harness_complete();
    const int planned_check_count = checks::planned_blocked_execution_rehearsal_split_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-blocked-execution-rehearsal-split-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"blockedExecutionRehearsalSplitNonParticipationMode\":\"controlled-read-only-blocked-execution-rehearsal-split-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeBlockedExecutionRehearsalSplitReleaseRange\":\"Node v1968-v1982\""
           ",\"sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion\":\"Node v1982\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v99\""
           ",\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"wal-regression-evidence\""
           ",\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v90\""
           ",\"sourceNodeConsumesHistoricalJavaEvidence\":\"context-normalization-evidence\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeServiceStartupRequired\":false"
           ",\"sourceNodeBlockedExecutionRehearsalSplitOnly\":true"
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitCommand\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCommand\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitContract\":\"shard-route-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation.v1\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationContract\":\"shard-route-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation.v1\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitReleaseVersion\":\"v1505\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixturePath\":\"fixtures/release/shard-readiness-v1505.json\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationFixturePath\":\"fixtures/release/shard-readiness-v1505.json\""
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":" +
           std::to_string(source_stage_count) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":" + json_bool(source_complete) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitStatus\":" +
           json_string(source_disabled_fake_harness::
                           disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_status()) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitDigestMarker\":" +
           json_string(source_disabled_fake_harness::
                           disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1505\""
           ",\"blockedExecutionRehearsalSplitNonParticipationStage\":" + json_string(stage.stage) +
           ",\"blockedExecutionRehearsalSplitNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeStart\":\"v1506\""
           ",\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"blockedExecutionRehearsalSplitNonParticipationDeclared\":true"
           ",\"blockedExecutionRehearsalSplitNonParticipationOnly\":true"
           ",\"blockedExecutionRehearsalSplitNonParticipationChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true"
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeServiceSplitObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"blockedExecutionRehearsalSplitFocus\":" +
           json_string_array({"public-entrypoint-preserved-no-import",
                              "types-module-no-import",
                              "constants-module-no-import",
                              "historical-reference-no-parse",
                              "java-v90-reference-no-scan",
                              "minikv-v99-wal-regression-no-replay",
                              "renderer-no-execution",
                              "digest-core-no-execution",
                              "summary-core-no-execution",
                              "attempt-policy-no-execution",
                              "message-policy-no-execution",
                              "focused-route-tests-observed-only",
                              "downstream-intake-observed-only",
                              "typecheck-build-observed-only",
                              "node-closeout-observed-only",
                              "minikv-v99-non-parse-lock",
                              "java-v90-non-scan-lock",
                              "downstream-no-execution-lock",
                              "command-fixture-smoke-boundary",
                              "archive-cleanup-ci-closeout"}) +
           ",\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false"
           ",\"nodePreconditionIntakeExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvParsesHistoricalReceipts\":" + json_bool(mini_kv_parses_historical_receipts) +
           ",\"miniKvScansJavaEvidence\":" + json_bool(mini_kv_scans_java_evidence) +
           ",\"miniKvReplaysWalRegression\":" + json_bool(mini_kv_replays_wal_regression) +
           ",\"miniKvReadsHistoricalMiniKvV99WalRegressionFile\":false"
           ",\"miniKvScansJavaV90ContextNormalizationEvidence\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"moduleSplit\":[\"blocked_execution_rehearsal_split_non_participation_core\",\"blocked_execution_rehearsal_split_non_participation_stages\",\"blocked_execution_rehearsal_split_non_participation_checks\",\"blocked_execution_rehearsal_split_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_blocked_execution_rehearsal_split_non_participation_stage_catalog_json() +
           ",\"blockedExecutionRehearsalSplitNonParticipationChecks\":" +
           checks::format_blocked_execution_rehearsal_split_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               mini_kv_reads_endpoints,
               mini_kv_reads_credentials,
               mini_kv_parses_historical_receipts,
               mini_kv_scans_java_evidence,
               mini_kv_replays_wal_regression,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1505 remains the frozen disabled fake harness contract upstream echo verification split non-participation source",
                              "Node v1968-v1982 splits blocked execution rehearsal internals into loader types constants references core policy and renderer modules",
                              "Node consumes historical Java v90 context-normalization evidence and mini-kv v99 WAL regression evidence only; no fresh sibling work or service startup is required",
                              "mini-kv records the split without importing Node modules parsing historical receipt files scanning Java evidence replaying WAL regression reading endpoints credentials managed connections routers writes WAL or execution"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string blocked_execution_rehearsal_split_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_blocked_execution_rehearsal_split_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation
