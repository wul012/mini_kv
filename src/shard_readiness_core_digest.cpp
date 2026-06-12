#include "shard_readiness_internal.hpp"

#include "minikv/shard_readiness.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview.hpp"
#include "minikv/shard_route_preview_archive_maintenance.hpp"
#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"
#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"
#include "minikv/shard_route_preview_operator_import_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_draft.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"
#include "minikv/shard_preview_candidate_request_package.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity.hpp"
#include "minikv/shard_preview_candidate_submission_precheck.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
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
#include "minikv/shard_preview_candidate_intake_packet.hpp"
#include "minikv/shard_preview_candidate_material_request.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"
#include "minikv/shard_preview_candidate_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"
#include "minikv/shard_preview_production_live_capture_archive_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"
#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"
#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section.hpp"
#include "minikv/version.hpp"

namespace minikv::shard_readiness::internal {

std::string json_string(std::string_view value) { return runtime_evidence::json_string(value); }

std::string evidence_digest() {
    const std::string digest_prefix = "mini-kv-shard-readiness-" + std::string{release_version};
    return runtime_evidence::digest(
        digest_prefix,
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
            {"commandCatalogSingleSourceRefactor=v1546-v1607-no-runtime-behavior-change"},
            {"commandCatalogCount=" + std::to_string(command_catalog::count())},
            {"commandCatalogHelpCompletionContractsDeduplicated=true"},
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
            {"shardRoutePreviewVerificationReport=" + shard_route_preview_verification_report::rollout_digest_marker()},
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
            {"shardRoutePreviewOperatorValueSupplyApprovalTemplate=" +
             shard_route_preview_operator_value_supply_approval_template::
                 value_supply_approval_template_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalTemplate=" +
             shard_route_preview_operator_value_supply_signed_approval_template::
                 signed_approval_template_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCapturePreflight=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_preflight::
                 signed_approval_capture_preflight_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactPreflight=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight::
                 signed_approval_capture_artifact_preflight_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftAuthoringReadiness=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness::
                 signed_approval_capture_artifact_draft_authoring_readiness_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftInstructionPreflight=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight::
                 signed_approval_capture_artifact_draft_instruction_preflight_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageReviewPreflight=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight::
                 signed_approval_capture_artifact_draft_text_package_review_preflight_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageReviewCloseoutAudit=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit::
                 signed_approval_capture_artifact_draft_text_package_review_closeout_audit_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageComparisonCloseoutAudit"
             "=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit::
                 signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_digest_marker()},
            {"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageComparedPackageEvidenceI"
             "ntakeAudit=" +
             shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit::
                 signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_digest_marker()},
            {"shardPreviewCandidateDocumentRequestPackage=" +
             shard_preview_candidate_request_package::candidate_document_request_package_digest_marker()},
            {"shardPreviewCandidateRequestPackageIntegrity=" +
             shard_preview_candidate_request_package_integrity::candidate_request_package_integrity_digest_marker()},
            {"shardPreviewCandidateSubmissionPrecheck=" +
             shard_preview_candidate_submission_precheck::candidate_submission_precheck_digest_marker()},
            {"shardPreviewCandidateIntakePacket=" +
             shard_preview_candidate_intake_packet::candidate_intake_packet_digest_marker()},
            {"shardPreviewCandidateMaterialRequest=" +
             shard_preview_candidate_material_request::candidate_material_request_digest_marker()},
            {"shardPreviewCandidateMaterialRequestIntegrity=" +
             shard_preview_candidate_material_request_integrity::candidate_material_request_integrity_digest_marker()},
            {"shardPreviewCandidateMaterialSubmissionPrecheck=" +
             shard_preview_candidate_material_submission_precheck::
                 candidate_material_submission_precheck_digest_marker()},
            {"shardPreviewCandidateMaterialSubmissionPrecheckIntegrity=" +
             shard_preview_candidate_material_submission_precheck_integrity::
                 candidate_material_submission_precheck_integrity_digest_marker()},
            {"shardPreviewCandidateProfileSection=" +
             shard_preview_candidate_profile_section::candidate_profile_section_digest_marker()},
            {"shardPreviewSignedApprovalArtifactDraftProfileSection=" +
             shard_preview_signed_approval_artifact_draft_profile_section::
                 signed_approval_artifact_draft_profile_section_digest_marker()},
            {"shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity=" +
             shard_preview_signed_approval_artifact_draft_profile_section_integrity::
                 signed_approval_artifact_draft_profile_section_integrity_digest_marker()},
            {"shardPreviewSignedApprovalArtifactDraftTextPackageProfileSection=" +
             shard_preview_signed_approval_artifact_draft_text_package_profile_section::
                 signed_approval_artifact_draft_text_package_profile_section_digest_marker()},
            {"shardPreviewOperatorValueSupplyApprovalProfileSection=" +
             shard_preview_operator_value_supply_approval_profile_section::
                 operator_value_supply_approval_profile_section_digest_marker()},
            {"shardPreviewOperatorValueSupplyProfileSection=" +
             shard_preview_operator_value_supply_profile_section::
                 operator_value_supply_profile_section_digest_marker()},
            {"shardPreviewOperatorValueSupplyProfileSectionIntegrity=" +
             shard_preview_operator_value_supply_profile_section_integrity::
                 operator_value_supply_profile_section_integrity_digest_marker()},
            {"shardPreviewProductionLiveCaptureNonParticipation=" +
             shard_preview_production_live_capture_non_participation::
                 production_live_capture_non_participation_digest_marker()},
            {"shardPreviewProductionLiveCaptureArchiveNonParticipation=" +
             shard_preview_production_live_capture_archive_non_participation::
                 production_live_capture_archive_non_participation_digest_marker()},
            {"shardPreviewProductionLiveCaptureArchiveVerificationNonParticipation=" +
             shard_preview_production_live_capture_archive_verification_non_participation::
                 production_live_capture_archive_verification_non_participation_digest_marker()},
            {"shardPreviewProductionLiveCaptureReleaseEvidenceReviewNonParticipation=" +
             shard_preview_production_live_capture_release_evidence_review_non_participation::
                 production_live_capture_release_evidence_review_non_participation_digest_marker()},
            {"shardPreviewCatalogEntryGroupSplitNonParticipation=" +
             shard_preview_catalog_entry_group_split_non_participation::
                 catalog_entry_group_split_non_participation_digest_marker()},
            {"shardPreviewTypeBarrelSplitNonParticipation=" +
             shard_preview_type_barrel_split_non_participation::type_barrel_split_non_participation_digest_marker()},
            {"shardPreviewTypeBarrelSplitFollowUpNonParticipation=" +
             shard_preview_type_barrel_split_follow_up_non_participation::
                 type_barrel_split_follow_up_non_participation_digest_marker()},
            {"shardPreviewTypeBarrelSplitFollowUpFixtureAudit=" +
             shard_preview_type_barrel_split_follow_up_fixture_audit::
                 type_barrel_split_follow_up_fixture_audit_digest_marker()},
            {"shardPreviewDisabledPrecheckUpstreamEchoNonParticipation=" +
             shard_preview_disabled_precheck_upstream_echo_non_participation::
                 disabled_precheck_upstream_echo_non_participation_digest_marker()},
            {"shardPreviewSandboxEndpointCredentialResolverUpstreamEchoNonParticipation=" +
             shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation::
                 sandbox_endpoint_credential_resolver_upstream_echo_non_participation_digest_marker()},
            {"shardPreviewImplementationPlanUpstreamEchoCloseoutNonParticipation=" +
             shard_preview_implementation_plan_upstream_echo_closeout_non_participation::
                 implementation_plan_upstream_echo_closeout_non_participation_digest_marker()},
            {"shardPreviewReleaseWindowReadinessPacketSplitNonParticipation=" +
             shard_preview_release_window_readiness_packet_split_non_participation::
                 release_window_readiness_packet_split_non_participation_digest_marker()},
            {"shardPreviewDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipation=" +
             shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation::
                 disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_digest_marker()},
            {"shardPreviewBlockedExecutionRehearsalSplitNonParticipation=" +
             shard_preview_blocked_execution_rehearsal_split_non_participation::
                 blocked_execution_rehearsal_split_non_participation_digest_marker()},
            {"shardPreviewPrecheckUpstreamReceiptVerificationSplitNonParticipation=" +
             shard_preview_precheck_upstream_receipt_verification_split_non_participation::
                 precheck_upstream_receipt_verification_split_non_participation_digest_marker()},
            {"shardPreviewRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipation=" +
             shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation::
                 runtime_execution_packet_approval_gate_archive_verification_non_participation_digest_marker()},
            {"shardPreviewCodeWalkthroughQualityGateNonParticipation=" +
             shard_preview_code_walkthrough_quality_gate_non_participation::
                 code_walkthrough_quality_gate_non_participation_digest_marker()},
            {"shardPreviewProductionShardExecutionOwnerReceiptRequestPacket=" +
             shard_preview_production_shard_execution_owner_receipt_request_packet::
                 production_shard_execution_owner_receipt_request_packet_digest_marker()},
            {"shardPreviewProductionShardExecutionMiniKvOwnerReceiptBundle=" +
             shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle::
                 production_shard_execution_mini_kv_owner_receipt_bundle_digest_marker()},
            {"shardPreviewProductionShardExecutionExternalArtifactDryRunCloseout=" +
             shard_preview_production_shard_execution_external_artifact_dry_run_closeout::
                 production_shard_execution_external_artifact_dry_run_closeout_digest_marker()},
            {"shardPreviewFFolderExplanationQualityCloseout=" +
             shard_preview_f_folder_explanation_quality_closeout::
                 f_folder_explanation_quality_closeout_digest_marker()},
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
            {"nodeRouteCatalogCleanupPostCloseoutContinuity=v262-v261-continuity-continuity-node-v549-final-"
             "verification-ready-no-execution"},
        });
}

} // namespace minikv::shard_readiness::internal
