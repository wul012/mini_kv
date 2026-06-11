#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks;
namespace source_packet = shard_preview_production_shard_execution_owner_receipt_request_packet;
namespace stages = shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages;
namespace validation = shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation;

constexpr std::string_view command_name = "SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v2087-production-shard-execution-owner-receipt-request-packet-roadmap.md";
constexpr int current_stage_count = 5;
constexpr int mini_kv_receipt_slot_count = 4;
constexpr int external_receipt_slot_count = 5;
constexpr bool mini_kv_abort_owner_receipt_present = true;
constexpr bool mini_kv_rollback_owner_receipt_present = true;
constexpr bool mini_kv_lifecycle_owner_receipt_present = true;
constexpr bool mini_kv_cleanup_no_start_proof_present = true;
constexpr bool mini_kv_owner_receipt_bundle_only = true;
constexpr bool java_owner_receipts_present = false;
constexpr bool cross_project_cleanup_reconciliation_receipt_present = false;
constexpr bool per_version_archive_required = true;
constexpr bool archive_root_is_f = true;
constexpr bool combined_archive_folder_allowed = false;
constexpr bool abort_execution_performed = false;
constexpr bool rollback_execution_performed = false;
constexpr bool lifecycle_service_started = false;
constexpr bool cleanup_execution_performed = false;
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
    return stages::production_shard_execution_mini_kv_owner_receipt_bundle_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

bool source_request_packet_complete() {
    return source_packet::published_stage_count() == 5;
}

bool source_request_packet_validation_passed() {
    return source_request_packet_complete();
}

int completed_check_count() {
    return checks::planned_production_shard_execution_mini_kv_owner_receipt_bundle_check_count();
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_shard_execution_mini_kv_owner_receipt_bundle_stages(),
        current_stage_count,
        stages::planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count(),
        stages::first_production_shard_execution_mini_kv_owner_receipt_bundle_release_number());
}

std::string format_archive_plan_json() {
    return "{\"archiveRoot\":\"f\""
           ",\"archiveRootIsF\":" + json_bool(archive_root_is_f) +
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
           ",\"combinedArchiveFolderAllowed\":" + json_bool(combined_archive_folder_allowed) +
           ",\"versionedArchiveDirectories\":[\"f/1597/\",\"f/1598/\",\"f/1599/\",\"f/1600/\",\"f/1601/\"]"
           ",\"screenshotDirectoryName\":\"project-standard-picture-subdir\""
           ",\"explanationDirectoryName\":\"project-standard-explanation-subdir\""
           ",\"batchArchiveFolderUsed\":false}";
}

std::string format_mini_kv_owner_receipts_json() {
    return "{\"miniKvOwnerReceiptSlots\":" +
           json_string_array({"mini-kv.abort-owner-receipt",
                              "mini-kv.rollback-owner-receipt",
                              "mini-kv.lifecycle-owner-receipt",
                              "mini-kv.cleanup-no-start-proof"}) +
           ",\"miniKvReceiptSlotCount\":" + std::to_string(mini_kv_receipt_slot_count) +
           ",\"miniKvAbortOwnerReceiptPresent\":" + json_bool(mini_kv_abort_owner_receipt_present) +
           ",\"miniKvRollbackOwnerReceiptPresent\":" + json_bool(mini_kv_rollback_owner_receipt_present) +
           ",\"miniKvLifecycleOwnerReceiptPresent\":" + json_bool(mini_kv_lifecycle_owner_receipt_present) +
           ",\"miniKvCleanupNoStartProofPresent\":" +
           json_bool(mini_kv_cleanup_no_start_proof_present) +
           ",\"miniKvReceiptsComplete\":true"
           ",\"miniKvOwnerReceiptBundleOnly\":" + json_bool(mini_kv_owner_receipt_bundle_only) +
           ",\"abortExecutionPerformed\":" + json_bool(abort_execution_performed) +
           ",\"rollbackExecutionPerformed\":" + json_bool(rollback_execution_performed) +
           ",\"lifecycleServiceStarted\":" + json_bool(lifecycle_service_started) +
           ",\"cleanupExecutionPerformed\":" + json_bool(cleanup_execution_performed) +
           "}";
}

std::string format_external_receipts_json() {
    return "{\"javaOwnerReceiptsPresent\":" + json_bool(java_owner_receipts_present) +
           ",\"crossProjectCleanupReconciliationReceiptPresent\":" +
           json_bool(cross_project_cleanup_reconciliation_receipt_present) +
           ",\"externalReceiptSlotCount\":" + std::to_string(external_receipt_slot_count) +
           ",\"externalReceiptsAbsent\":true"
           ",\"productionShardExecutionStillBlocked\":true}";
}

} // namespace

std::string format_production_shard_execution_mini_kv_owner_receipt_bundle_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_shard_execution_mini_kv_owner_receipt_bundle_stages(),
        current_stage_count);
}

std::string production_shard_execution_mini_kv_owner_receipt_bundle_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_shard_execution_mini_kv_owner_receipt_bundle_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_packet::published_stage_count();
    const bool source_complete = source_request_packet_complete();
    const bool source_validation_passed = source_request_packet_validation_passed();
    const int planned_check_count =
        checks::planned_production_shard_execution_mini_kv_owner_receipt_bundle_check_count();
    const int check_count = completed_check_count();
    return "{\"contract\":\"shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleMode\":\"mini-kv-owner-receipts-only\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceOwnerReceiptRequestPacketCommand\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\""
           ",\"sourceOwnerReceiptRequestPacketContract\":\"shard-production-shard-execution-owner-receipt-request-packet.v1\""
           ",\"sourceOwnerReceiptRequestPacketReleaseVersion\":\"v1596\""
           ",\"sourceOwnerReceiptRequestPacketFixturePath\":\"fixtures/release/shard-readiness-v1596.json\""
           ",\"sourceOwnerReceiptRequestPacketPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceOwnerReceiptRequestPacketComplete\":" + json_bool(source_complete) +
           ",\"sourceOwnerReceiptRequestPacketValidationPassed\":" + json_bool(source_validation_passed) +
           ",\"sourceOwnerReceiptRequestPacketStatus\":" +
           json_string(source_packet::production_shard_execution_owner_receipt_request_packet_status()) +
           ",\"sourceOwnerReceiptRequestPacketDigestMarker\":" +
           json_string(source_packet::production_shard_execution_owner_receipt_request_packet_digest_marker()) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleStage\":" + json_string(stage.stage) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseRangeStart\":\"v1597\""
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionShardExecutionMiniKvOwnerReceiptBundleCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionMiniKvOwnerReceiptBundleCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleDeclared\":true"
           ",\"miniKvOwnerReceipts\":" + format_mini_kv_owner_receipts_json() +
           ",\"externalOwnerReceipts\":" + format_external_receipts_json() +
           ",\"remainingReceiptBlockers\":" +
           json_string_array({"Java owner receipts remain absent",
                              "cross-project cleanup reconciliation receipt remains absent",
                              "production shard execution remains blocked until external receipts are reviewed"}) +
           ",\"receiptBoundaryWarnings\":" +
           json_string_array({"mini-kv owner receipts prove ownership only",
                              "abort and rollback paths are not executed",
                              "lifecycle ownership does not start services",
                              "cleanup no-start proof does not execute cleanup",
                              "archive evidence is split under f/<version> directories"}) +
           ",\"archivePlan\":" + format_archive_plan_json() +
           ",\"moduleSplit\":[\"production_shard_execution_mini_kv_owner_receipt_bundle_core\",\"production_shard_execution_mini_kv_owner_receipt_bundle_stages\",\"production_shard_execution_mini_kv_owner_receipt_bundle_checks\",\"production_shard_execution_mini_kv_owner_receipt_bundle_validation\"]"
           ",\"stageCatalog\":" +
           format_production_shard_execution_mini_kv_owner_receipt_bundle_stage_catalog_json() +
           ",\"productionShardExecutionMiniKvOwnerReceiptBundleChecks\":" +
           checks::format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_shard_execution_mini_kv_owner_receipt_bundle_validation_json(
               source_stage_count,
               source_complete,
               source_validation_passed,
               mini_kv_abort_owner_receipt_present,
               mini_kv_rollback_owner_receipt_present,
               mini_kv_lifecycle_owner_receipt_present,
               mini_kv_cleanup_no_start_proof_present,
               mini_kv_owner_receipt_bundle_only,
               mini_kv_receipt_slot_count,
               java_owner_receipts_present,
               cross_project_cleanup_reconciliation_receipt_present,
               per_version_archive_required,
               archive_root_is_f,
               combined_archive_folder_allowed,
               abort_execution_performed,
               rollback_execution_performed,
               lifecycle_service_started,
               cleanup_execution_performed,
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
               stages::planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1596 remains the frozen owner receipt request packet source",
                              "mini-kv now satisfies its four requested receipt slots as local evidence",
                              "Java and cross-project cleanup receipts are still absent",
                              "f-root archive rotation is active for v1597-v1601"}) +
           ",\"abortExecutionPerformed\":" + json_bool(abort_execution_performed) +
           ",\"rollbackExecutionPerformed\":" + json_bool(rollback_execution_performed) +
           ",\"lifecycleServiceStarted\":" + json_bool(lifecycle_service_started) +
           ",\"cleanupExecutionPerformed\":" + json_bool(cleanup_execution_performed) +
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

std::string production_shard_execution_mini_kv_owner_receipt_bundle_digest_marker() {
    const auto& stage = current_stage();
    return shard_route_preview_stage_catalog::format_digest_marker(
        stage,
        current_stage_count,
        stages::planned_production_shard_execution_mini_kv_owner_receipt_bundle_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle
