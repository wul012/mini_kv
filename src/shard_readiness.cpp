#include "minikv/shard_readiness.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview.hpp"
#include "minikv/shard_route_preview_archive_maintenance.hpp"
#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"
#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"
#include "minikv/shard_route_preview_operator_import_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_draft.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck.hpp"
#include "minikv/shard_route_preview_verification.hpp"
#include "minikv/shard_route_preview_verification_report.hpp"
#include "minikv/shard_route_preview_worksheet_verification.hpp"
#include "minikv/shard_route_preview_verification_report_archive.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_closeout.hpp"
#include "minikv/shard_readiness_approval_inputs.hpp"
#include "minikv/shard_readiness_boundary_fields.hpp"
#include "minikv/shard_readiness_history.hpp"
#include "minikv/shard_readiness_node_compatibility.hpp"
#include "minikv/shard_readiness_release_catalog.hpp"
#include "minikv/shard_readiness_route_catalog_cleanup_catalog.hpp"
#include "minikv/shard_readiness_route_catalog_evidence_chain.hpp"
#include "minikv/shard_readiness_route_catalog_post_closeout.hpp"
#include "minikv/shard_readiness_route_catalog_closeout.hpp"
#include "minikv/shard_readiness_slot_preview.hpp"
#include "minikv/shard_readiness_slot_preview_audit.hpp"
#include "minikv/version.hpp"

#include "shard_readiness_lifecycle_sections.hpp"
#include "shard_readiness_runtime_gate_sections.hpp"
#include "shard_readiness_summary_sections.hpp"

#include <string>
#include <string_view>

namespace minikv::shard_readiness {
namespace {

constexpr std::string_view contract_version = "shard-readiness.v1";
constexpr std::string_view release_version = "v624";

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string format_boundaries_json() {
    return "{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false,"
           "\"activeRouterInstalled\":false,\"archivedNodeEvidenceMutated\":false}";
}

std::string format_command_catalog_json() {
    return "{\"command\":\"SHARDJSON\",\"category\":\"read\",\"mutatesStore\":false,"
           "\"touchesWal\":false,\"stable\":true,\"extraArgsAllowed\":false,"
           "\"sideEffects\":[\"metadata_read\"]}";
}

std::string evidence_digest() {
    return runtime_evidence::digest(
        "mini-kv-shard-readiness-v624",
        {
            {std::string{contract_version}},
            {std::string{version}},
            {std::string{release_version}},
            {"shardEnabled=false"},
            {"shardCount=1"},
            {"slotCount=16"},
            {"routingMode=single-shard-readiness-prototype"},
            {fixture_path()},
            {"commandCatalog=read-no-mutate-no-wal"},
            {"fixtureParity=runtime-matches-current-fixture"},
            {"historicalFallback=v261-frozen-no-rolling-current"},
            {"archivedNodeEvidence=v370-v480-preserved"},
            {"shardReadinessHistoryMaintenance=v180-formatter-split-no-execution"},
            {"activePrototypePlan=prerequisite-only-no-activation"},
            {"activePrototypePlanFreeze=v153-frozen-no-router-no-write"},
            {"consumerHandoff=frozen-evidence-only-no-live-read"},
            {"liveReadGatePlan=prerequisite-only-no-start-no-execution"},
            {"liveReadGatePlanFreeze=v153-frozen-no-runtime-probe"},
            {"operatorServiceLifecycleTemplate=template-only-no-runtime"},
            {"operatorServiceLifecycleTemplateFreeze=v153-frozen-template-only"},
            {"operatorServiceLifecycleEvidence=declared-no-runtime-gate"},
            {"operatorServiceLifecycleEvidenceFreeze=v153-frozen-declared-no-runtime"},
            {"runtimeExecutionArtifactIntakePreflight=blocked-0-of-6-no-execution"},
            {"runtimeExecutionArtifactIntakePreflightFreeze=v154-frozen-candidate-0-of-6"},
            {"miniKvRuntimeExecutionArtifactCandidate=candidate-no-runtime-no-approval"},
            {"miniKvRuntimeExecutionArtifactCandidateFreeze=v154-frozen-candidate-only"},
            {"runtimeExecutionApprovalGateInputPrecheck=blocked-0-of-3-no-execution"},
            {"runtimeExecutionApprovalGateInputPrecheckFreeze=v155-frozen-0-of-3"},
            {"miniKvFinalApprovalGateInput=complete-mini-kv-input-global-runtime-blocked"},
            {"miniKvFinalApprovalGateInputFreeze=v156-frozen-final-input-no-runtime"},
            {"runtimeExecutionApprovalInputTemplateValidatorEcho=v402-template-only-no-canonical-inputs"},
            {"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze=v157-frozen-template-only"},
            {"runtimeExecutionCanonicalApprovalInputPrecheck=blocked-0-of-3-no-execution"},
            {"nodeRouteGroupSplitCompatibility=v418-route-refactor-contract-stable"},
            {"boundaryCatalogMaintenance=v160-contract-preserving-split-no-execution"},
            {"boundaryCatalogIndex=v262-v29-node-v549-post-closeout-continuity-no-execution"},
            {"slotTablePreview=v162-read-only-no-router-no-write-no-execution"},
            {"slotTablePreviewAudit=v163-contiguous-no-duplicates-no-router-no-execution"},
            {"slotTablePreviewAuditMaintenance=v164-formatter-split-contract-preserving"},
            {"shardRoutePreview=" + shard_route_preview::rollout_digest_marker()},
            {"shardRoutePreviewVerification=" + shard_route_preview_verification::rollout_digest_marker()},
            {"shardRoutePreviewVerificationReport=" +
             shard_route_preview_verification_report::rollout_digest_marker()},
            {"shardRoutePreviewVerificationReportCloseout=" +
             shard_route_preview_verification_report_closeout::closeout_digest_marker()},
            {"shardRoutePreviewVerificationReportArchive=" +
             shard_route_preview_verification_report_archive::archive_digest_marker()},
            {"shardRoutePreviewVerificationReportArchiveCloseout=" +
             shard_route_preview_verification_report_archive_closeout::closeout_digest_marker()},
            {"shardRoutePreviewVerificationReportArchiveCloseoutVerification=" +
             shard_route_preview_verification_report_archive_closeout_verification::verification_digest_marker()},
            {"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAudit=" +
             shard_route_preview_verification_report_archive_closeout_verification_audit::audit_digest_marker()},
            {"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseout=" +
             shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
                 closeout_digest_marker()},
            {"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArchiveVerification=" +
             shard_route_preview_audit_closeout_archive_verification::archive_verification_digest_marker()},
            {"shardRoutePreviewArchiveMaintenance=" +
             shard_route_preview_archive_maintenance::maintenance_digest_marker()},
            {"shardRoutePreviewArchiveMaintenanceVerification=" +
             shard_route_preview_archive_maintenance_verification::verification_digest_marker()},
            {"shardRoutePreviewWorksheetVerification=" +
             shard_route_preview_worksheet_verification::worksheet_verification_digest_marker()},
            {"shardRoutePreviewOperatorImportPreflight=" +
             shard_route_preview_operator_import_preflight::import_preflight_digest_marker()},
            {"shardRoutePreviewOperatorValueDraft=" +
             shard_route_preview_operator_value_draft::value_draft_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplyEnvelope=" +
             shard_route_preview_operator_value_supply_envelope::value_supply_envelope_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplyPrecheck=" +
             shard_route_preview_operator_value_supply_precheck::value_supply_precheck_digest_marker()},
            {"shardReadinessReleaseCatalog=v165-versioned-catalog-no-execution"},
            {"shardReadinessReleaseCatalogAudit=v166-catalog-consistency-no-execution"},
            {"nodeRouteSplitCompatibilityWindow=v232-node-v433-v472-route-catalog-cleanup-closeout-no-execution"},
            {"nodeRouteSplitCompatibilityWindowAudit=v232-window-span-count-parse-audit-no-execution"},
            {"nodeRouteCatalogCloseoutSnapshot=v185-node-v464-validation-snapshot-no-execution"},
            {"nodeRouteCatalogAnchorRemovalAudit=v186-node-v467-anchor-removal-no-execution"},
            {"nodeRouteCatalogExpectedIntegritySnapshotAudit=v187-node-v471-integrity-snapshot-no-execution"},
            {"nodeRouteCatalogExpectedIntegritySnapshotFreeze=v188-v187-frozen-no-rolling-current"},
            {"nodeRouteCatalogEvidenceChain=v189-v185-v188-versioned-catalog-no-execution"},
            {"nodeRouteCatalogEvidenceChainAudit=v190-node-v472-closeout-validation-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoff=v191-v190-handoff-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoffAudit=v192-v191-handoff-audit-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze=v193-v192-handoff-audit-freeze-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutReleaseCatalog=v194-v190-v193-release-catalog-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit=v195-v194-release-catalog-audit-no-execution"},
            {"nodeRouteCatalogCleanupCloseoutCatalogMaintenance=v196-formatter-split-no-execution"},
            {"nodeRouteCatalogCleanupLatestEvidencePackage=v197-v194-v196-package-no-execution"},
            {"nodeRouteCatalogCleanupLatestEvidencePackageAudit=v198-v197-package-audit-no-execution"},
            {"nodeRouteCatalogCleanupEvidenceBatchCloseout=v199-v194-v198-closeout-no-execution"},
            {"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit=v200-v199-closeout-audit-no-execution"},
            {"nodeRouteCatalogCleanupPostCloseoutContinuity=v262-v261-continuity-continuity-node-v549-final-verification-ready-no-execution"},
        });
}

} // namespace

std::string fixture_path() {
    return "fixtures/release/shard-readiness.json";
}

std::string format_json() {
    return "{\"contract\":" + json_string(contract_version) +
           ",\"evidenceType\":\"shard_readiness\"" +
           ",\"project\":\"mini-kv\"" +
           ",\"version\":" + json_string(version) +
           ",\"releaseVersion\":" + json_string(release_version) +
           ",\"readOnly\":true" +
           ",\"executionAllowed\":false" +
           ",\"shardEnabled\":false" +
           ",\"shardCount\":1" +
           ",\"slotCount\":" + std::to_string(slot_preview::slot_count()) +
           ",\"routingMode\":\"single-shard-readiness-prototype\"" +
           ",\"evidencePath\":" + json_string(fixture_path()) +
           ",\"status\":\"route-preview-value-supply-precheck-redaction-policy-read-only\"" +
           ",\"shardMap\":" + slot_preview::format_shard_map_json() +
           ",\"keyRoutingSamples\":" + slot_preview::format_route_samples_json() +
           ",\"slotTablePreview\":" + slot_preview::format_slot_table_preview_json() +
           ",\"slotTablePreviewAudit\":" + slot_preview_audit::format_slot_table_preview_audit_json() +
           ",\"slotTablePreviewAuditMaintenance\":" +
           slot_preview_audit::format_slot_table_preview_audit_maintenance_json() +
           ",\"shardRoutePreview\":" + shard_route_preview::format_rollout_json() +
           ",\"shardRoutePreviewVerification\":" + shard_route_preview_verification::format_rollout_json() +
           ",\"shardRoutePreviewVerificationReport\":" +
           shard_route_preview_verification_report::format_rollout_json() +
           ",\"shardRoutePreviewVerificationReportCloseout\":" +
           shard_route_preview_verification_report_closeout::format_closeout_json() +
           ",\"shardRoutePreviewVerificationReportArchive\":" +
           shard_route_preview_verification_report_archive::format_archive_json() +
           ",\"shardRoutePreviewVerificationReportArchiveCloseout\":" +
           shard_route_preview_verification_report_archive_closeout::format_closeout_json() +
           ",\"shardRoutePreviewVerificationReportArchiveCloseoutVerification\":" +
           shard_route_preview_verification_report_archive_closeout_verification::format_verification_json() +
           ",\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAudit\":" +
           shard_route_preview_verification_report_archive_closeout_verification_audit::format_audit_json() +
           ",\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseout\":" +
           shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::format_closeout_json() +
           ",\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArchiveVerification\":" +
           shard_route_preview_audit_closeout_archive_verification::format_archive_verification_json() +
           ",\"shardRoutePreviewArchiveMaintenance\":" +
           shard_route_preview_archive_maintenance::format_maintenance_json() +
           ",\"shardRoutePreviewArchiveMaintenanceVerification\":" +
           shard_route_preview_archive_maintenance_verification::format_verification_json() +
           ",\"shardRoutePreviewWorksheetVerification\":" +
           shard_route_preview_worksheet_verification::format_worksheet_verification_json() +
           ",\"shardRoutePreviewOperatorImportPreflight\":" +
           shard_route_preview_operator_import_preflight::format_import_preflight_json() +
           ",\"shardRoutePreviewOperatorValueDraft\":" +
           shard_route_preview_operator_value_draft::format_value_draft_json() +
           ",\"shardRoutePreviewOperatorValueSupplyEnvelope\":" +
           shard_route_preview_operator_value_supply_envelope::format_value_supply_envelope_json() +
           ",\"shardRoutePreviewOperatorValueSupplyPrecheck\":" +
           shard_route_preview_operator_value_supply_precheck::format_value_supply_precheck_json() +
           ",\"shardReadinessReleaseCatalog\":" +
           release_catalog::format_release_catalog_json() +
           ",\"shardReadinessReleaseCatalogAudit\":" +
           release_catalog::format_release_catalog_audit_json() +
           ",\"boundaries\":" + format_boundaries_json() +
           ",\"diagnostics\":" + history::format_diagnostics_json() +
           ",\"commandCatalog\":" + format_command_catalog_json() +
           ",\"fixtureParity\":" + history::format_fixture_parity_json() +
           ",\"archiveCompatibility\":" + history::format_archive_compatibility_json() +
           ",\"historicalFallback\":" + history::format_historical_fallback_json() +
           ",\"shardReadinessHistoryMaintenance\":" +
           history::format_history_formatter_maintenance_json() +
           ",\"activePrototypePlan\":" + lifecycle_sections::format_active_prototype_plan_json() +
           ",\"activePrototypePlanFreeze\":" + lifecycle_sections::format_active_prototype_plan_freeze_json() +
           ",\"consumerHandoff\":" + lifecycle_sections::format_consumer_handoff_json() +
           ",\"liveReadGatePlan\":" + lifecycle_sections::format_live_read_gate_plan_json() +
           ",\"liveReadGatePlanFreeze\":" + lifecycle_sections::format_live_read_gate_plan_freeze_json() +
           ",\"operatorServiceLifecycleTemplate\":" +
           lifecycle_sections::format_operator_service_lifecycle_template_json() +
           ",\"operatorServiceLifecycleTemplateFreeze\":" +
           lifecycle_sections::format_operator_service_lifecycle_template_freeze_json() +
           ",\"operatorServiceLifecycleEvidence\":" +
           lifecycle_sections::format_operator_service_lifecycle_evidence_json() +
           ",\"operatorServiceLifecycleEvidenceFreeze\":" +
           lifecycle_sections::format_operator_service_lifecycle_evidence_freeze_json() +
           ",\"runtimeExecutionArtifactIntakePreflight\":" +
           runtime_gate_sections::format_runtime_execution_artifact_intake_preflight_json() +
           ",\"runtimeExecutionArtifactIntakePreflightFreeze\":" +
           runtime_gate_sections::format_runtime_execution_artifact_intake_preflight_freeze_json() +
           ",\"miniKvRuntimeExecutionArtifactCandidate\":" +
           runtime_gate_sections::format_mini_kv_runtime_execution_artifact_candidate_json() +
           ",\"miniKvRuntimeExecutionArtifactCandidateFreeze\":" +
           runtime_gate_sections::format_mini_kv_runtime_execution_artifact_candidate_freeze_json() +
           ",\"runtimeExecutionApprovalGateInputPrecheck\":" +
           runtime_gate_sections::format_runtime_execution_approval_gate_input_precheck_json() +
           ",\"runtimeExecutionApprovalGateInputPrecheckFreeze\":" +
           runtime_gate_sections::format_runtime_execution_approval_gate_input_precheck_freeze_json() +
           ",\"miniKvFinalApprovalGateInput\":" +
           runtime_gate_sections::format_mini_kv_final_approval_gate_input_json() +
           ",\"miniKvFinalApprovalGateInputFreeze\":" +
           approval_inputs::format_final_approval_gate_input_freeze_json() +
           ",\"runtimeExecutionApprovalInputTemplateValidatorEcho\":" +
           approval_inputs::format_template_validator_echo_json() +
           ",\"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze\":" +
           approval_inputs::format_template_validator_echo_freeze_json() +
           ",\"runtimeExecutionCanonicalApprovalInputPrecheck\":" +
           approval_inputs::format_canonical_approval_input_precheck_json() +
           ",\"nodeRouteGroupSplitCompatibility\":" +
           node_compatibility::format_route_group_split_compatibility_json() +
           ",\"nodeRouteSplitCompatibilityWindow\":" +
           node_compatibility::format_route_split_compatibility_window_json() +
           ",\"nodeRouteSplitCompatibilityWindowAudit\":" +
           node_compatibility::format_route_split_compatibility_window_audit_json() +
           ",\"nodeRouteCatalogCloseoutSnapshot\":" +
           route_catalog_closeout::format_closeout_snapshot_json() +
           ",\"nodeRouteCatalogAnchorRemovalAudit\":" +
           route_catalog_closeout::format_anchor_removal_audit_json() +
           ",\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":" +
           route_catalog_closeout::format_expected_integrity_snapshot_audit_json() +
           ",\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":" +
           route_catalog_closeout::format_expected_integrity_snapshot_freeze_json() +
           ",\"nodeRouteCatalogEvidenceChain\":" +
           route_catalog_evidence_chain::format_route_catalog_evidence_chain_json() +
           ",\"nodeRouteCatalogEvidenceChainAudit\":" +
           route_catalog_evidence_chain::format_route_catalog_evidence_chain_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoff\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":" +
           route_catalog_evidence_chain::format_route_catalog_cleanup_closeout_handoff_audit_freeze_json() +
           ",\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_release_catalog_json() +
           ",\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_release_catalog_audit_json() +
           ",\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_closeout_catalog_maintenance_json() +
           ",\"nodeRouteCatalogCleanupLatestEvidencePackage\":" +
           route_catalog_cleanup_catalog::format_latest_route_catalog_cleanup_evidence_package_json() +
           ",\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":" +
           route_catalog_cleanup_catalog::format_latest_route_catalog_cleanup_evidence_package_audit_json() +
           ",\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_evidence_batch_closeout_json() +
           ",\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":" +
           route_catalog_cleanup_catalog::format_route_catalog_cleanup_evidence_batch_closeout_audit_json() +
           ",\"nodeRouteCatalogCleanupPostCloseoutContinuity\":" +
           route_catalog_post_closeout::format_post_closeout_continuity_json() +
           ",\"boundaryCatalogMaintenance\":" +
           boundary_fields::format_catalog_maintenance_json() +
           ",\"boundaryCatalogIndex\":" +
           boundary_fields::format_catalog_index_json() +
           ",\"readOnlyBoundaryFields\":" +
           boundary_fields::format_read_only_boundary_fields_json() +
           ",\"evidenceDigest\":" + json_string(evidence_digest()) +
           ",\"notes\":" + summary_sections::format_notes_json() +
           "}";
}

} // namespace minikv::shard_readiness
