#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_checks.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_stages.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_shard_execution_owner_receipt_request_packet_checks;
namespace source_quality_gate = shard_preview_code_walkthrough_quality_gate_non_participation;
namespace stages = shard_preview_production_shard_execution_owner_receipt_request_packet_stages;
namespace validation = shard_preview_production_shard_execution_owner_receipt_request_packet_validation;

constexpr std::string_view command_name = "SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v2087-production-shard-execution-owner-receipt-request-packet-roadmap.md";
constexpr int current_stage_count = 5;
constexpr int requested_receipt_slot_count = 9;
constexpr bool node_waits_for_mini_kv = false;
constexpr bool node_requires_existing_signed_receipts = false;
constexpr bool signed_mini_kv_receipt_present = false;
constexpr bool signed_java_receipt_present = false;
constexpr bool signed_cross_project_cleanup_receipt_present = false;
constexpr bool owner_receipt_request_packet_only = true;
constexpr bool per_version_archive_required = true;
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
    return stages::production_shard_execution_owner_receipt_request_packet_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_shard_execution_owner_receipt_request_packet_stages(),
        current_stage_count,
        stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count(),
        stages::first_production_shard_execution_owner_receipt_request_packet_release_number());
}

bool source_quality_gate_complete() {
    return source_quality_gate::published_stage_count() == 6;
}

int completed_check_count() {
    return checks::planned_production_shard_execution_owner_receipt_request_packet_check_count();
}

std::string format_archive_plan_json() {
    return "{\"archiveRoot\":\"e\""
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
           ",\"combinedArchiveFolderAllowed\":" + json_bool(combined_archive_folder_allowed) +
           ",\"versionedArchiveDirectories\":[\"e/1592/\",\"e/1593/\",\"e/1594/\",\"e/1595/\",\"e/1596/\"]"
           ",\"screenshotDirectoryName\":\"project-standard-picture-subdir\""
           ",\"explanationDirectoryName\":\"project-standard-explanation-subdir\""
           ",\"batchArchiveFolderUsed\":false}";
}

std::string format_receipt_request_slots_json() {
    return "{\"miniKvReceiptRequests\":" +
           json_string_array({"mini-kv.abort-owner-receipt",
                              "mini-kv.rollback-owner-receipt",
                              "mini-kv.lifecycle-owner-receipt",
                              "mini-kv.cleanup-no-start-proof"}) +
           ",\"javaReceiptRequests\":" +
           json_string_array({"java.abort-rollback-owner-receipt",
                              "java.lifecycle-owner-receipt",
                              "java.cleanup-no-start-echo-receipt"}) +
           ",\"crossProjectReceiptRequests\":" +
           json_string_array({"cross-project.cleanup-reconciliation-receipt",
                              "cross-project.owner-receipt-handoff-index"}) +
           ",\"requestedReceiptSlotCount\":" + std::to_string(requested_receipt_slot_count) +
           ",\"signedMiniKvReceiptPresent\":" + json_bool(signed_mini_kv_receipt_present) +
           ",\"signedJavaReceiptPresent\":" + json_bool(signed_java_receipt_present) +
           ",\"signedCrossProjectCleanupReceiptPresent\":" +
           json_bool(signed_cross_project_cleanup_receipt_present) +
           ",\"ownerReceiptRequestPacketOnly\":" + json_bool(owner_receipt_request_packet_only) +
           "}";
}

} // namespace

std::string format_production_shard_execution_owner_receipt_request_packet_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_shard_execution_owner_receipt_request_packet_stages(),
        current_stage_count);
}

std::string production_shard_execution_owner_receipt_request_packet_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_shard_execution_owner_receipt_request_packet_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_quality_gate::published_stage_count();
    const bool source_complete = source_quality_gate_complete();
    const int planned_check_count =
        checks::planned_production_shard_execution_owner_receipt_request_packet_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count ==
            stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-production-shard-execution-owner-receipt-request-packet.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketMode\":\"request-only-owner-receipt-slot-packet\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProductionShardExecutionReleaseVersion\":\"Node v2087\""
           ",\"sourceNodeProductionShardExecutionProfile\":\"production-shard-execution-owner-receipt-request-packet\""
           ",\"sourceNodeWaitsForMiniKv\":" + json_bool(node_waits_for_mini_kv) +
           ",\"sourceNodeRequiresExistingSignedReceipts\":" +
           json_bool(node_requires_existing_signed_receipts) +
           ",\"sourceNodeAllowsJavaMiniKvParallel\":true"
           ",\"sourceNodeClaimsReceiptsExist\":false"
           ",\"sourceCodeWalkthroughQualityGateNonParticipationCommand\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\""
           ",\"sourceCodeWalkthroughQualityGateNonParticipationContract\":\"shard-route-code-walkthrough-quality-gate-non-participation.v1\""
           ",\"sourceCodeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\""
           ",\"sourceCodeWalkthroughQualityGateNonParticipationFixturePath\":\"fixtures/release/shard-readiness-v1591.json\""
           ",\"sourceCodeWalkthroughQualityGateNonParticipationPublishedStageCount\":" +
           std::to_string(source_stage_count) +
           ",\"sourceCodeWalkthroughQualityGateNonParticipationComplete\":" + json_bool(source_complete) +
           ",\"sourceCodeWalkthroughQualityGateNonParticipationStatus\":" +
           json_string(source_quality_gate::code_walkthrough_quality_gate_non_participation_status()) +
           ",\"sourceCodeWalkthroughQualityGateNonParticipationDigestMarker\":" +
           json_string(source_quality_gate::code_walkthrough_quality_gate_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1591\""
           ",\"productionShardExecutionOwnerReceiptRequestPacketStage\":" + json_string(stage.stage) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeStart\":\"v1592\""
           ",\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketDeclared\":true"
           ",\"ownerReceiptRequestPacketOnly\":" + json_bool(owner_receipt_request_packet_only) +
           ",\"productionShardExecutionOwnerReceiptRequestPacketChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceQualityGateFixtureFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"receiptRequestSlots\":" + format_receipt_request_slots_json() +
           ",\"signedReceiptPresence\":{\"miniKv\":false,\"java\":false,\"crossProjectCleanup\":false,\"anySignedSiblingReceipt\":false}"
           ",\"receiptRequestWarnings\":" +
           json_string_array({"request packet is not a signed receipt",
                              "missing signed sibling receipts keep production shard execution blocked",
                              "Java and mini-kv may implement slots in parallel without Node waiting",
                              "screenshots and explanations are archived per version, not in one batch folder"}) +
           ",\"receiptRequestBlockers\":" +
           json_string_array({"signed mini-kv owner receipt absent",
                              "signed Java owner receipt absent",
                              "cross-project cleanup reconciliation receipt absent",
                              "production authority remains blocked until all owner receipts are reviewed"}) +
           ",\"archivePlan\":" + format_archive_plan_json() +
           ",\"moduleSplit\":[\"production_shard_execution_owner_receipt_request_packet_core\",\"production_shard_execution_owner_receipt_request_packet_stages\",\"production_shard_execution_owner_receipt_request_packet_checks\",\"production_shard_execution_owner_receipt_request_packet_validation\"]"
           ",\"stageCatalog\":" +
           format_production_shard_execution_owner_receipt_request_packet_stage_catalog_json() +
           ",\"productionShardExecutionOwnerReceiptRequestPacketChecks\":" +
           checks::format_production_shard_execution_owner_receipt_request_packet_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_shard_execution_owner_receipt_request_packet_validation_json(
               source_stage_count,
               source_complete,
               node_waits_for_mini_kv,
               node_requires_existing_signed_receipts,
               signed_mini_kv_receipt_present,
               signed_java_receipt_present,
               signed_cross_project_cleanup_receipt_present,
               owner_receipt_request_packet_only,
               requested_receipt_slot_count,
               per_version_archive_required,
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
               stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1591 remains the frozen code walkthrough quality gate source",
                              "Node v2087 asks for exact owner receipt slots, not receipt fabrication",
                              "mini-kv records request slots while keeping production shard execution closed",
                              "each release in v1592-v1596 has its own screenshot and explanation archive"}) +
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
           ",\"executionAttempted\":false"
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           ",\"readOnly\":true}";
}

std::string production_shard_execution_owner_receipt_request_packet_digest_marker() {
    const auto& stage = current_stage();
    return shard_route_preview_stage_catalog::format_digest_marker(
        stage,
        current_stage_count,
        stages::planned_production_shard_execution_owner_receipt_request_packet_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet
