#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks;
namespace source_bundle = shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle;
namespace stages = shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages;
namespace validation = shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation;

constexpr std::string_view command_name =
    "SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v2093-production-shard-execution-external-artifact-dry-run-closeout-roadmap.md";
constexpr int current_stage_count = 5;
constexpr bool dry_run_batch_closed = true;
constexpr bool node_only_growth_stopped = true;
constexpr bool real_external_artifact_present = false;
constexpr bool external_artifact_parsed = false;
constexpr bool external_artifact_accepted = false;
constexpr bool external_artifact_stored = false;
constexpr bool dry_run_artifact_executed = false;
constexpr bool real_signed_approval_present = false;
constexpr bool managed_audit_owner_binding_present = false;
constexpr bool real_java_receipt_present = false;
constexpr bool real_mini_kv_signed_receipt_present = false;
constexpr bool real_cleanup_receipt_present = false;
constexpr bool missing_real_artifact_blocker_listed = true;
constexpr bool per_version_archive_required = true;
constexpr bool archive_root_is_f = true;
constexpr bool combined_archive_folder_allowed = false;
constexpr bool production_shard_execution_enabled = false;
constexpr bool managed_audit_store_bound = false;
constexpr bool raw_endpoint_parsed = false;
constexpr bool credential_value_read = false;
constexpr bool starts_node_services = false;
constexpr bool starts_java_service = false;
constexpr bool starts_mini_kv_service = false;
constexpr bool active_router_installed = false;
constexpr bool write_routing_allowed = false;
constexpr bool mutates_store = false;
constexpr bool touches_wal = false;
constexpr bool execution_allowed = false;

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
    return stages::production_shard_execution_external_artifact_dry_run_closeout_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

bool source_mini_kv_owner_receipt_bundle_complete() {
    return source_bundle::published_stage_count() == 5;
}

bool source_mini_kv_owner_receipt_bundle_validation_passed() {
    return source_mini_kv_owner_receipt_bundle_complete();
}

int completed_check_count() {
    return checks::planned_production_shard_execution_external_artifact_dry_run_closeout_check_count();
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_shard_execution_external_artifact_dry_run_closeout_stages(),
        current_stage_count,
        stages::planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count(),
        stages::first_production_shard_execution_external_artifact_dry_run_closeout_release_number());
}

std::string format_archive_plan_json() {
    return "{\"archiveRoot\":\"f\""
           ",\"archiveRootIsF\":" + json_bool(archive_root_is_f) +
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
           ",\"combinedArchiveFolderAllowed\":" + json_bool(combined_archive_folder_allowed) +
           ",\"versionedArchiveDirectories\":[\"f/1602/\",\"f/1603/\",\"f/1604/\",\"f/1605/\",\"f/1606/\"]"
           ",\"screenshotDirectoryName\":\"project-standard-picture-subdir\""
           ",\"explanationDirectoryName\":\"project-standard-explanation-subdir\""
           ",\"batchArchiveFolderUsed\":false}";
}

std::string format_missing_real_artifacts_json() {
    return "{\"requiredExternalArtifactSlots\":" +
           json_string_array({"real-signed-approval",
                              "managed-audit-owner-binding",
                              "real-java-owner-receipt",
                              "real-mini-kv-signed-owner-receipt",
                              "real-cross-project-cleanup-receipt"}) +
           ",\"realExternalArtifactPresent\":" + json_bool(real_external_artifact_present) +
           ",\"realSignedApprovalPresent\":" + json_bool(real_signed_approval_present) +
           ",\"managedAuditOwnerBindingPresent\":" + json_bool(managed_audit_owner_binding_present) +
           ",\"realJavaReceiptPresent\":" + json_bool(real_java_receipt_present) +
           ",\"realMiniKvSignedReceiptPresent\":" + json_bool(real_mini_kv_signed_receipt_present) +
           ",\"realCleanupReceiptPresent\":" + json_bool(real_cleanup_receipt_present) +
           ",\"productionShardExecutionStillBlocked\":true}";
}

std::string format_artifact_handling_boundary_json() {
    return "{\"externalArtifactParsed\":" + json_bool(external_artifact_parsed) +
           ",\"externalArtifactAccepted\":" + json_bool(external_artifact_accepted) +
           ",\"externalArtifactStored\":" + json_bool(external_artifact_stored) +
           ",\"dryRunArtifactExecuted\":" + json_bool(dry_run_artifact_executed) +
           ",\"miniKvConsumesExternalArtifactBytes\":false"
           ",\"miniKvIssuesSignedReceipt\":false"
           ",\"dryRunOnly\":true"
           ",\"nodeOnlyGrowthStopped\":" + json_bool(node_only_growth_stopped) +
           "}";
}

} // namespace

std::string format_production_shard_execution_external_artifact_dry_run_closeout_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_shard_execution_external_artifact_dry_run_closeout_stages(),
        current_stage_count);
}

std::string production_shard_execution_external_artifact_dry_run_closeout_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_shard_execution_external_artifact_dry_run_closeout_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_bundle::published_stage_count();
    const bool source_complete = source_mini_kv_owner_receipt_bundle_complete();
    const bool source_validation_passed = source_mini_kv_owner_receipt_bundle_validation_passed();
    const int planned_check_count =
        checks::planned_production_shard_execution_external_artifact_dry_run_closeout_check_count();
    const int check_count = completed_check_count();

    return "{\"contract\":\"shard-production-shard-execution-external-artifact-dry-run-closeout.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutMode\":\"dry-run-closeout-intake-boundary\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceMiniKvOwnerReceiptBundleCommand\":\"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON\""
           ",\"sourceMiniKvOwnerReceiptBundleContract\":\"shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1\""
           ",\"sourceMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\""
           ",\"sourceMiniKvOwnerReceiptBundleFixturePath\":\"fixtures/release/shard-readiness-v1601.json\""
           ",\"sourceMiniKvOwnerReceiptBundlePublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceMiniKvOwnerReceiptBundleComplete\":" + json_bool(source_complete) +
           ",\"sourceMiniKvOwnerReceiptBundleValidationPassed\":" + json_bool(source_validation_passed) +
           ",\"sourceMiniKvOwnerReceiptBundleStatus\":" +
           json_string(source_bundle::production_shard_execution_mini_kv_owner_receipt_bundle_status()) +
           ",\"sourceMiniKvOwnerReceiptBundleDigestMarker\":" +
           json_string(source_bundle::production_shard_execution_mini_kv_owner_receipt_bundle_digest_marker()) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutStage\":" + json_string(stage.stage) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseRangeStart\":\"v1602\""
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionShardExecutionExternalArtifactDryRunCloseoutCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionExternalArtifactDryRunCloseoutCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutDeclared\":true"
           ",\"dryRunCloseout\":{\"dryRunBatchClosed\":" + json_bool(dry_run_batch_closed) +
           ",\"nodeOnlyGrowthStopped\":" + json_bool(node_only_growth_stopped) +
           ",\"missingRealArtifactBlockerListed\":" + json_bool(missing_real_artifact_blocker_listed) +
           ",\"productionShardExecutionStillBlocked\":true}"
           ",\"missingRealExternalArtifacts\":" + format_missing_real_artifacts_json() +
           ",\"artifactHandlingBoundary\":" + format_artifact_handling_boundary_json() +
           ",\"remainingExternalArtifactBlockers\":" +
           json_string_array({"real external approval artifact is missing",
                              "managed audit owner binding is missing",
                              "real Java owner receipt is missing",
                              "real mini-kv signed owner receipt is missing",
                              "cross-project cleanup receipt is missing"}) +
           ",\"closeoutBoundaryWarnings\":" +
           json_string_array({"Node v2093 dry-run closeout is recorded as context only",
                              "mini-kv does not accept or parse external artifacts",
                              "mini-kv v1601 owner receipt evidence is local and not a real signed external artifact",
                              "production shard execution stays blocked until real artifacts arrive",
                              "archive evidence is split under f/<version> directories"}) +
           ",\"archivePlan\":" + format_archive_plan_json() +
           ",\"moduleSplit\":[\"production_shard_execution_external_artifact_dry_run_closeout_core\",\"production_shard_execution_external_artifact_dry_run_closeout_stages\",\"production_shard_execution_external_artifact_dry_run_closeout_checks\",\"production_shard_execution_external_artifact_dry_run_closeout_validation\"]"
           ",\"stageCatalog\":" +
           format_production_shard_execution_external_artifact_dry_run_closeout_stage_catalog_json() +
           ",\"productionShardExecutionExternalArtifactDryRunCloseoutChecks\":" +
           checks::format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_shard_execution_external_artifact_dry_run_closeout_validation_json(
               source_stage_count,
               source_complete,
               source_validation_passed,
               dry_run_batch_closed,
               node_only_growth_stopped,
               real_external_artifact_present,
               external_artifact_parsed,
               external_artifact_accepted,
               external_artifact_stored,
               dry_run_artifact_executed,
               real_signed_approval_present,
               managed_audit_owner_binding_present,
               real_java_receipt_present,
               real_mini_kv_signed_receipt_present,
               real_cleanup_receipt_present,
               missing_real_artifact_blocker_listed,
               per_version_archive_required,
               archive_root_is_f,
               combined_archive_folder_allowed,
               production_shard_execution_enabled,
               managed_audit_store_bound,
               raw_endpoint_parsed,
               credential_value_read,
               starts_node_services,
               starts_java_service,
               starts_mini_kv_service,
               active_router_installed,
               write_routing_allowed,
               mutates_store,
               touches_wal,
               execution_allowed,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1601 remains the frozen mini-kv owner receipt bundle source",
                              "Node v2093 dry-run batch closeout is recorded without consuming external artifact bytes",
                              "real signed approval, managed audit binding, Java, mini-kv signed, and cleanup receipts remain missing",
                              "f-root archive rotation is active for v1602-v1606"}) +
           ",\"realExternalArtifactPresent\":" + json_bool(real_external_artifact_present) +
           ",\"externalArtifactParsed\":" + json_bool(external_artifact_parsed) +
           ",\"externalArtifactAccepted\":" + json_bool(external_artifact_accepted) +
           ",\"externalArtifactStored\":" + json_bool(external_artifact_stored) +
           ",\"dryRunArtifactExecuted\":" + json_bool(dry_run_artifact_executed) +
           ",\"realSignedApprovalPresent\":" + json_bool(real_signed_approval_present) +
           ",\"managedAuditOwnerBindingPresent\":" + json_bool(managed_audit_owner_binding_present) +
           ",\"realJavaReceiptPresent\":" + json_bool(real_java_receipt_present) +
           ",\"realMiniKvSignedReceiptPresent\":" + json_bool(real_mini_kv_signed_receipt_present) +
           ",\"realCleanupReceiptPresent\":" + json_bool(real_cleanup_receipt_present) +
           ",\"productionShardExecutionEnabled\":" + json_bool(production_shard_execution_enabled) +
           ",\"managedAuditStoreBound\":" + json_bool(managed_audit_store_bound) +
           ",\"rawEndpointParsed\":" + json_bool(raw_endpoint_parsed) +
           ",\"credentialValueRead\":" + json_bool(credential_value_read) +
           ",\"startsNodeServices\":" + json_bool(starts_node_services) +
           ",\"startsJavaService\":" + json_bool(starts_java_service) +
           ",\"startsMiniKvService\":" + json_bool(starts_mini_kv_service) +
           ",\"activeRouterInstalled\":" + json_bool(active_router_installed) +
           ",\"writeRoutingAllowed\":" + json_bool(write_routing_allowed) +
           ",\"mutatesStore\":" + json_bool(mutates_store) +
           ",\"touchesWal\":" + json_bool(touches_wal) +
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           ",\"readOnly\":true}";
}

std::string production_shard_execution_external_artifact_dry_run_closeout_digest_marker() {
    const auto& stage = current_stage();
    return shard_route_preview_stage_catalog::format_digest_marker(
        stage,
        current_stage_count,
        stages::planned_production_shard_execution_external_artifact_dry_run_closeout_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout
