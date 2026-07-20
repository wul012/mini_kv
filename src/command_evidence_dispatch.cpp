#include "command_evidence_dispatch.hpp"

#include "minikv/command_contracts.hpp"
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
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
#include "minikv/shard_route_preview_verification_report_archive.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_closeout.hpp"
#include "minikv/shard_route_preview_worksheet_verification.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"
#include "minikv/shard_preview_candidate_intake_packet.hpp"
#include "minikv/shard_preview_candidate_material_request.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"
#include "minikv/shard_preview_candidate_profile_section.hpp"
#include "minikv/shard_preview_candidate_request_package.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity.hpp"
#include "minikv/shard_preview_candidate_submission_precheck.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation.hpp"
#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_archive_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"
#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation.hpp"

#include <algorithm>
#include <array>

namespace minikv::command_internal {
namespace {

constexpr std::array commands = {
    EvidenceCommand{"SHARDROUTEVERIFYREPORTCLOSEOUTJSON",
                    &shard_route_preview_verification_report_closeout::format_closeout_json},
    EvidenceCommand{"SHARDROUTEVERIFYREPORTARCHIVEJSON",
                    &shard_route_preview_verification_report_archive::format_archive_json},
    EvidenceCommand{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON",
                    &shard_route_preview_verification_report_archive_closeout::format_closeout_json},
    EvidenceCommand{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON",
                    &shard_route_preview_verification_report_archive_closeout_verification::format_verification_json},
    EvidenceCommand{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON",
                    &shard_route_preview_verification_report_archive_closeout_verification_audit::format_audit_json},
    EvidenceCommand{
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON",
        &shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::format_closeout_json},
    EvidenceCommand{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON",
                    &shard_route_preview_audit_closeout_archive_verification::format_archive_verification_json},
    EvidenceCommand{"SHARDROUTEARCHIVEMAINTJSON", &shard_route_preview_archive_maintenance::format_maintenance_json},
    EvidenceCommand{"SHARDROUTEARCHIVEMAINTVERIFYJSON",
                    &shard_route_preview_archive_maintenance_verification::format_verification_json},
    EvidenceCommand{"SHARDROUTEWORKSHEETVERIFYJSON",
                    &shard_route_preview_worksheet_verification::format_worksheet_verification_json},
    EvidenceCommand{"SHARDROUTEIMPORTPREFLIGHTJSON",
                    &shard_route_preview_operator_import_preflight::format_import_preflight_json},
    EvidenceCommand{"SHARDROUTEVALUEDRAFTJSON", &shard_route_preview_operator_value_draft::format_value_draft_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYJSON",
                    &shard_route_preview_operator_value_supply_envelope::format_value_supply_envelope_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYPRECHECKJSON",
                    &shard_route_preview_operator_value_supply_precheck::format_value_supply_precheck_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON",
        &shard_route_preview_operator_value_supply_approval_template::format_value_supply_approval_template_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON",
        &shard_route_preview_operator_value_supply_signed_approval_template::format_signed_approval_template_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON",
                    &shard_route_preview_operator_value_supply_signed_approval_capture_preflight::
                        format_signed_approval_capture_preflight_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON",
                    &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight::
                        format_signed_approval_capture_artifact_preflight_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness::
            format_signed_approval_capture_artifact_draft_authoring_readiness_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight::
            format_signed_approval_capture_artifact_draft_instruction_preflight_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight::
            format_signed_approval_capture_artifact_draft_text_package_review_preflight_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit::
            format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit::
            format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON",
        &shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit::
            format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECL"
        "OSEOUTJSON",
        &shard_preview_candidate_request_package::format_candidate_document_request_package_closeout_json},
    EvidenceCommand{
        "SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON",
        &shard_preview_candidate_request_package_integrity::format_candidate_request_package_integrity_json},
    EvidenceCommand{"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON",
                    &shard_preview_candidate_submission_precheck::format_candidate_submission_precheck_json},
    EvidenceCommand{"SHARDROUTECANDIDATEINTAKEPACKETJSON",
                    &shard_preview_candidate_intake_packet::format_candidate_intake_packet_json},
    EvidenceCommand{"SHARDROUTECANDIDATEMATERIALREQUESTJSON",
                    &shard_preview_candidate_material_request::format_candidate_material_request_json},
    EvidenceCommand{
        "SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON",
        &shard_preview_candidate_material_request_integrity::format_candidate_material_request_integrity_json},
    EvidenceCommand{
        "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON",
        &shard_preview_candidate_material_submission_precheck::format_candidate_material_submission_precheck_json},
    EvidenceCommand{"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON",
                    &shard_preview_candidate_material_submission_precheck_integrity::
                        format_candidate_material_submission_precheck_integrity_json},
    EvidenceCommand{"SHARDROUTECANDIDATEPROFILESECTIONJSON",
                    &shard_preview_candidate_profile_section::format_candidate_profile_section_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON",
                    &shard_preview_signed_approval_artifact_draft_profile_section::
                        format_signed_approval_artifact_draft_profile_section_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON",
                    &shard_preview_signed_approval_artifact_draft_profile_section_integrity::
                        format_signed_approval_artifact_draft_profile_section_integrity_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON",
                    &shard_preview_signed_approval_artifact_draft_text_package_profile_section::
                        format_signed_approval_artifact_draft_text_package_profile_section_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON",
                    &shard_preview_operator_value_supply_approval_profile_section::
                        format_operator_value_supply_approval_profile_section_json},
    EvidenceCommand{
        "SHARDROUTEVALUESUPPLYPROFILESECTIONJSON",
        &shard_preview_operator_value_supply_profile_section::format_operator_value_supply_profile_section_json},
    EvidenceCommand{"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON",
                    &shard_preview_operator_value_supply_profile_section_integrity::
                        format_operator_value_supply_profile_section_integrity_json},
    EvidenceCommand{"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON",
                    &shard_preview_production_live_capture_non_participation::
                        format_production_live_capture_non_participation_json},
    EvidenceCommand{"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON",
                    &shard_preview_production_live_capture_archive_non_participation::
                        format_production_live_capture_archive_non_participation_json},
    EvidenceCommand{"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON",
                    &shard_preview_production_live_capture_archive_verification_non_participation::
                        format_production_live_capture_archive_verification_non_participation_json},
    EvidenceCommand{"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON",
                    &shard_preview_production_live_capture_release_evidence_review_non_participation::
                        format_production_live_capture_release_evidence_review_non_participation_json},
    EvidenceCommand{"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON",
                    &shard_preview_catalog_entry_group_split_non_participation::
                        format_catalog_entry_group_split_non_participation_json},
    EvidenceCommand{"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON",
                    &shard_preview_disabled_precheck_upstream_echo_non_participation::
                        format_disabled_precheck_upstream_echo_non_participation_json},
    EvidenceCommand{"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON",
                    &shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation::
                        format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_json},
    EvidenceCommand{"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON",
                    &shard_preview_implementation_plan_upstream_echo_closeout_non_participation::
                        format_implementation_plan_upstream_echo_closeout_non_participation_json},
    EvidenceCommand{"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON",
                    &shard_preview_release_window_readiness_packet_split_non_participation::
                        format_release_window_readiness_packet_split_non_participation_json},
    EvidenceCommand{"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON",
                    &shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation::
                        format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_json},
    EvidenceCommand{"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON",
                    &shard_preview_blocked_execution_rehearsal_split_non_participation::
                        format_blocked_execution_rehearsal_split_non_participation_json},
    EvidenceCommand{"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON",
                    &shard_preview_precheck_upstream_receipt_verification_split_non_participation::
                        format_precheck_upstream_receipt_verification_split_non_participation_json},
    EvidenceCommand{"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON",
                    &shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation::
                        format_runtime_execution_packet_approval_gate_archive_verification_non_participation_json},
    EvidenceCommand{"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON",
                    &shard_preview_code_walkthrough_quality_gate_non_participation::
                        format_code_walkthrough_quality_gate_non_participation_json},
    EvidenceCommand{"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON",
                    &shard_preview_production_shard_execution_owner_receipt_request_packet::
                        format_production_shard_execution_owner_receipt_request_packet_json},
    EvidenceCommand{"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON",
                    &shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle::
                        format_production_shard_execution_mini_kv_owner_receipt_bundle_json},
    EvidenceCommand{"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON",
                    &shard_preview_production_shard_execution_external_artifact_dry_run_closeout::
                        format_production_shard_execution_external_artifact_dry_run_closeout_json},
    EvidenceCommand{
        "SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON",
        &shard_preview_f_folder_explanation_quality_closeout::format_f_folder_explanation_quality_closeout_json},
    EvidenceCommand{
        "SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON",
        &shard_preview_type_barrel_split_non_participation::format_type_barrel_split_non_participation_json},
    EvidenceCommand{"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON",
                    &shard_preview_type_barrel_split_follow_up_non_participation::
                        format_type_barrel_split_follow_up_non_participation_json},
    EvidenceCommand{"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON",
                    &shard_preview_type_barrel_split_follow_up_fixture_audit::
                        format_type_barrel_split_follow_up_fixture_audit_json},
    EvidenceCommand{"COMMANDS", &command_contracts::format_commands},
    EvidenceCommand{"COMMANDSJSON", &command_contracts::format_commands_json},
};

consteval bool unique_names() {
    for (std::size_t left = 0; left < commands.size(); ++left) {
        for (std::size_t right = left + 1; right < commands.size(); ++right) {
            if (commands[left].name == commands[right].name) {
                return false;
            }
        }
    }
    return true;
}

static_assert(commands.size() == 62);
static_assert(unique_names());

} // namespace

std::span<const EvidenceCommand> evidence_commands() { return commands; }

const EvidenceCommand* find_evidence_command(std::string_view name) {
    const auto found =
        std::find_if(commands.begin(), commands.end(), [name](const auto& command) { return command.name == name; });
    return found == commands.end() ? nullptr : &*found;
}

} // namespace minikv::command_internal
