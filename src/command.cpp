#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "command_wal_gate.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/command_contracts.hpp"
#include "minikv/command_response_formatters.hpp"
#include "minikv/shard_route_preview_archive_maintenance.hpp"
#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"
#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"
#include "minikv/shard_route_preview.hpp"
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
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
#include "minikv/shard_route_preview_worksheet_verification.hpp"
#include "minikv/shard_route_preview_verification.hpp"
#include "minikv/shard_route_preview_verification_report.hpp"
#include "minikv/shard_route_preview_verification_report_archive.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"
#include "minikv/shard_route_preview_verification_report_closeout.hpp"
#include "minikv/shard_preview_candidate_intake_packet.hpp"
#include "minikv/shard_preview_candidate_material_request.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"
#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"
#include "minikv/shard_preview_candidate_profile_section.hpp"
#include "minikv/shard_preview_candidate_request_package.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity.hpp"
#include "minikv/shard_preview_candidate_submission_precheck.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"
#include "minikv/shard_preview_production_live_capture_archive_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.hpp"
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
#include "minikv/shard_readiness.hpp"
#include "minikv/string_utils.hpp"
#include "minikv/wal.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace minikv {
namespace {

using command_internal::has_extra_token;
using command_internal::usage;
using command_internal::wal_command_gate;
using command_internal::wal_error;

std::string command_token(std::string_view text) {
    std::istringstream input{std::string{text}};
    std::string command;
    input >> command;
    return to_upper(command);
}

std::string metrics_command_name(std::string_view command) {
    std::string name = to_upper(command);
    if (name.empty() || !command_catalog::is_known(name)) {
        return "UNKNOWN";
    }
    return name;
}

CommandConnectionStats connection_stats(const CommandProcessorOptions& options) {
    if (options.connection_stats) {
        return options.connection_stats();
    }
    return {};
}

} // namespace

void CommandMetricsTracker::record(const CommandResult& result) { record("UNKNOWN", result, 0); }

void CommandMetricsTracker::record(std::string_view command, const CommandResult& result, std::uint64_t elapsed_ns) {
    const std::string bucket_name = metrics_command_name(command);
    const bool is_error = result.response.rfind("ERR ", 0) == 0;
    if (elapsed_ns == 0) {
        elapsed_ns = 1;
    }

    std::lock_guard lock{mutex_};
    ++totals_.total_commands;
    totals_.total_latency_ns += elapsed_ns;
    if (elapsed_ns > totals_.max_latency_ns) {
        totals_.max_latency_ns = elapsed_ns;
    }

    auto& bucket = breakdown_[bucket_name];
    bucket.command = bucket_name;
    ++bucket.total_commands;
    bucket.total_latency_ns += elapsed_ns;
    if (elapsed_ns > bucket.max_latency_ns) {
        bucket.max_latency_ns = elapsed_ns;
    }

    if (is_error) {
        ++totals_.error_commands;
        ++bucket.error_commands;
        return;
    }

    ++totals_.successful_commands;
    ++bucket.successful_commands;
}

void CommandMetricsTracker::reset() {
    std::lock_guard lock{mutex_};
    totals_ = {};
    breakdown_.clear();
}

CommandProcessorMetrics CommandMetricsTracker::stats() const {
    std::lock_guard lock{mutex_};
    CommandProcessorMetrics snapshot = totals_;
    snapshot.command_breakdown.clear();
    snapshot.command_breakdown.reserve(breakdown_.size());
    for (const auto& [_, metrics] : breakdown_) {
        snapshot.command_breakdown.push_back(metrics);
    }
    return snapshot;
}

CommandProcessor::CommandProcessor(Store& store, WriteAheadLog* wal, CommandProcessorOptions options)
    : store_(store), wal_(wal), options_(std::move(options)),
      metrics_tracker_(options_.metrics_tracker ? options_.metrics_tracker
                                                : std::make_shared<CommandMetricsTracker>()) {}

void CommandProcessor::auto_compact_wal_if_needed() {
    if (wal_ == nullptr || !options_.auto_compact_wal) {
        return;
    }

    auto scope = wal_command_gate().enter_compaction();
    (void)wal_->compact_if_recommended(store_);
}

CommandResult CommandProcessor::execute_with_wal(std::function<std::optional<std::string>()> wal_record,
                                                 std::function<CommandResult()> mutation) {
    if (wal_ == nullptr) {
        return mutation();
    }

    CommandResult result;
    {
        auto scope = wal_command_gate().enter_command();
        const auto record = wal_record();
        if (!record.has_value()) {
            return {"0"};
        }
        if (!wal_->append(*record)) {
            return wal_error();
        }

        result = mutation();
    }

    auto_compact_wal_if_needed();
    return result;
}

CommandResult CommandProcessor::execute_runtime_evidence_command(std::string_view command, std::istringstream& input) {
    if (command == "STATS") {
        if (has_extra_token(input)) {
            return usage("STATS");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            auto scope = wal_command_gate().enter_command();
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {command_response_formatters::format_stats(live_keys, wal_, wal_report, metrics_tracker_->stats(),
                                                          connection_stats(options_))};
    }

    if (command == "STATSJSON") {
        if (has_extra_token(input)) {
            return usage("STATSJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            auto scope = wal_command_gate().enter_command();
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {command_response_formatters::format_stats_json(live_keys, wal_, wal_report, metrics_tracker_->stats(),
                                                               connection_stats(options_))};
    }

    if (command == "SMOKEJSON") {
        if (has_extra_token(input)) {
            return usage("SMOKEJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            auto scope = wal_command_gate().enter_command();
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {command_response_formatters::format_smoke_json(live_keys, wal_, wal_report, metrics_tracker_->stats(),
                                                               connection_stats(options_), options_.runtime_info)};
    }

    if (command == "STORAGEJSON") {
        if (has_extra_token(input)) {
            return usage("STORAGEJSON");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            auto scope = wal_command_gate().enter_command();
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {command_response_formatters::format_storage_evidence_json(live_keys, wal_, wal_report)};
    }

    if (command == "HEALTH") {
        if (has_extra_token(input)) {
            return usage("HEALTH");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            auto scope = wal_command_gate().enter_command();
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {command_response_formatters::format_health(live_keys, wal_, wal_report, metrics_tracker_->stats(),
                                                           connection_stats(options_))};
    }

    if (command == "INFO") {
        if (has_extra_token(input)) {
            return usage("INFO");
        }

        return {command_response_formatters::format_info(store_.size(), wal_, options_.runtime_info)};
    }

    if (command == "INFOJSON") {
        if (has_extra_token(input)) {
            return usage("INFOJSON");
        }

        return {command_response_formatters::format_info_json(store_.size(), wal_, options_.runtime_info)};
    }

    if (command == "SHARDJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDJSON");
        }

        return {shard_readiness::format_json()};
    }

    if (command == "SHARDROUTE" || command == "SHARDROUTEJSON") {
        std::string key;
        input >> key;
        if (key.empty() || has_extra_token(input)) {
            return usage(std::string{command} + " key");
        }

        if (command == "SHARDROUTE") {
            return {shard_route_preview::format_route_text(key)};
        }
        return {shard_route_preview::format_route_json(key)};
    }

    if (command == "SHARDROUTEVERIFYJSON") {
        std::string key;
        input >> key;
        if (key.empty() || has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYJSON key");
        }

        return {shard_route_preview_verification::format_verification_json(key)};
    }

    if (command == "SHARDROUTEVERIFYREPORTJSON") {
        std::string key;
        input >> key;
        if (key.empty() || has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTJSON key");
        }

        return {shard_route_preview_verification_report::format_report_json(key)};
    }

    if (command == "SHARDROUTEVERIFYREPORTCLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTCLOSEOUTJSON");
        }

        return {shard_route_preview_verification_report_closeout::format_closeout_json()};
    }

    if (command == "SHARDROUTEVERIFYREPORTARCHIVEJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVEJSON");
        }

        return {shard_route_preview_verification_report_archive::format_archive_json()};
    }

    if (command == "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
        }

        return {shard_route_preview_verification_report_archive_closeout::format_closeout_json()};
    }
    if (command == "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");
        }

        return {shard_route_preview_verification_report_archive_closeout_verification::format_verification_json()};
    }
    if (command == "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");
        }

        return {shard_route_preview_verification_report_archive_closeout_verification_audit::format_audit_json()};
    }
    if (command == "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");
        }

        return {shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
                    format_closeout_json()};
    }
    if (command == "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");
        }

        return {shard_route_preview_audit_closeout_archive_verification::format_archive_verification_json()};
    }
    if (command == "SHARDROUTEARCHIVEMAINTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEARCHIVEMAINTJSON");
        }

        return {shard_route_preview_archive_maintenance::format_maintenance_json()};
    }
    if (command == "SHARDROUTEARCHIVEMAINTVERIFYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEARCHIVEMAINTVERIFYJSON");
        }

        return {shard_route_preview_archive_maintenance_verification::format_verification_json()};
    }
    if (command == "SHARDROUTEWORKSHEETVERIFYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEWORKSHEETVERIFYJSON");
        }

        return {shard_route_preview_worksheet_verification::format_worksheet_verification_json()};
    }
    if (command == "SHARDROUTEIMPORTPREFLIGHTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEIMPORTPREFLIGHTJSON");
        }

        return {shard_route_preview_operator_import_preflight::format_import_preflight_json()};
    }
    if (command == "SHARDROUTEVALUEDRAFTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUEDRAFTJSON");
        }

        return {shard_route_preview_operator_value_draft::format_value_draft_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYJSON");
        }

        return {shard_route_preview_operator_value_supply_envelope::format_value_supply_envelope_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYPRECHECKJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYPRECHECKJSON");
        }

        return {shard_route_preview_operator_value_supply_precheck::format_value_supply_precheck_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON");
        }

        return {
            shard_route_preview_operator_value_supply_approval_template::format_value_supply_approval_template_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");
        }

        return {
            shard_route_preview_operator_value_supply_signed_approval_template::format_signed_approval_template_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_preflight::
                    format_signed_approval_capture_preflight_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight::
                    format_signed_approval_capture_artifact_preflight_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness::
                    format_signed_approval_capture_artifact_draft_authoring_readiness_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight::
                    format_signed_approval_capture_artifact_draft_instruction_preflight_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON");
        }

        return {
            shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight::
                format_signed_approval_capture_artifact_draft_text_package_review_preflight_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON");
        }

        return {
            shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit::
                format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON") {
        if (has_extra_token(input)) {
            return usage(
                "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON");
        }

        return {
            shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit::
                format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_json()};
    }

    if (command ==
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTA"
                         "KEAUDITJSON");
        }

        return {
            shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit::
                format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECL"
                   "OSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPAC"
                         "KAGECLOSEOUTJSON");
        }

        return {shard_preview_candidate_request_package::format_candidate_document_request_package_closeout_json()};
    }

    if (command == "SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON");
        }

        return {shard_preview_candidate_request_package_integrity::format_candidate_request_package_integrity_json()};
    }

    if (command == "SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON");
        }

        return {shard_preview_candidate_submission_precheck::format_candidate_submission_precheck_json()};
    }

    if (command == "SHARDROUTECANDIDATEINTAKEPACKETJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEINTAKEPACKETJSON");
        }

        return {shard_preview_candidate_intake_packet::format_candidate_intake_packet_json()};
    }

    if (command == "SHARDROUTECANDIDATEMATERIALREQUESTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEMATERIALREQUESTJSON");
        }

        return {shard_preview_candidate_material_request::format_candidate_material_request_json()};
    }

    if (command == "SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON");
        }

        return {shard_preview_candidate_material_request_integrity::format_candidate_material_request_integrity_json()};
    }

    if (command == "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON");
        }

        return {
            shard_preview_candidate_material_submission_precheck::format_candidate_material_submission_precheck_json()};
    }

    if (command == "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON");
        }

        return {shard_preview_candidate_material_submission_precheck_integrity::
                    format_candidate_material_submission_precheck_integrity_json()};
    }

    if (command == "SHARDROUTECANDIDATEPROFILESECTIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECANDIDATEPROFILESECTIONJSON");
        }

        return {shard_preview_candidate_profile_section::format_candidate_profile_section_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON");
        }

        return {shard_preview_signed_approval_artifact_draft_profile_section::
                    format_signed_approval_artifact_draft_profile_section_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON");
        }

        return {shard_preview_signed_approval_artifact_draft_profile_section_integrity::
                    format_signed_approval_artifact_draft_profile_section_integrity_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON");
        }

        return {shard_preview_signed_approval_artifact_draft_text_package_profile_section::
                    format_signed_approval_artifact_draft_text_package_profile_section_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON");
        }

        return {shard_preview_operator_value_supply_approval_profile_section::
                    format_operator_value_supply_approval_profile_section_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYPROFILESECTIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYPROFILESECTIONJSON");
        }

        return {
            shard_preview_operator_value_supply_profile_section::format_operator_value_supply_profile_section_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON");
        }

        return {shard_preview_operator_value_supply_profile_section_integrity::
                    format_operator_value_supply_profile_section_integrity_json()};
    }

    if (command == "SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON");
        }

        return {shard_preview_production_live_capture_non_participation::
                    format_production_live_capture_non_participation_json()};
    }

    if (command == "SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON");
        }

        return {shard_preview_production_live_capture_archive_non_participation::
                    format_production_live_capture_archive_non_participation_json()};
    }

    if (command == "SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON");
        }

        return {shard_preview_production_live_capture_archive_verification_non_participation::
                    format_production_live_capture_archive_verification_non_participation_json()};
    }

    if (command == "SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON");
        }

        return {shard_preview_production_live_capture_release_evidence_review_non_participation::
                    format_production_live_capture_release_evidence_review_non_participation_json()};
    }

    if (command == "SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_catalog_entry_group_split_non_participation::
                    format_catalog_entry_group_split_non_participation_json()};
    }

    if (command == "SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON");
        }

        return {shard_preview_disabled_precheck_upstream_echo_non_participation::
                    format_disabled_precheck_upstream_echo_non_participation_json()};
    }

    if (command == "SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON");
        }

        return {shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation::
                    format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_json()};
    }

    if (command == "SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON");
        }

        return {shard_preview_implementation_plan_upstream_echo_closeout_non_participation::
                    format_implementation_plan_upstream_echo_closeout_non_participation_json()};
    }

    if (command == "SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_release_window_readiness_packet_split_non_participation::
                    format_release_window_readiness_packet_split_non_participation_json()};
    }

    if (command == "SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation::
                    format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_json()};
    }

    if (command == "SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_blocked_execution_rehearsal_split_non_participation::
                    format_blocked_execution_rehearsal_split_non_participation_json()};
    }

    if (command == "SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_precheck_upstream_receipt_verification_split_non_participation::
                    format_precheck_upstream_receipt_verification_split_non_participation_json()};
    }

    if (command == "SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON");
        }

        return {shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation::
                    format_runtime_execution_packet_approval_gate_archive_verification_non_participation_json()};
    }

    if (command == "SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON");
        }

        return {shard_preview_code_walkthrough_quality_gate_non_participation::
                    format_code_walkthrough_quality_gate_non_participation_json()};
    }

    if (command == "SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");
        }

        return {shard_preview_production_shard_execution_owner_receipt_request_packet::
                    format_production_shard_execution_owner_receipt_request_packet_json()};
    }

    if (command == "SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON");
        }

        return {shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle::
                    format_production_shard_execution_mini_kv_owner_receipt_bundle_json()};
    }

    if (command == "SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON");
        }

        return {shard_preview_production_shard_execution_external_artifact_dry_run_closeout::
                    format_production_shard_execution_external_artifact_dry_run_closeout_json()};
    }

    if (command == "SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");
        }

        return {
            shard_preview_f_folder_explanation_quality_closeout::format_f_folder_explanation_quality_closeout_json()};
    }

    if (command == "SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON");
        }

        return {shard_preview_type_barrel_split_non_participation::format_type_barrel_split_non_participation_json()};
    }

    if (command == "SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON");
        }

        return {shard_preview_type_barrel_split_follow_up_non_participation::
                    format_type_barrel_split_follow_up_non_participation_json()};
    }

    if (command == "SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON");
        }

        return {shard_preview_type_barrel_split_follow_up_fixture_audit::
                    format_type_barrel_split_follow_up_fixture_audit_json()};
    }

    if (command == "COMMANDS") {
        if (has_extra_token(input)) {
            return usage("COMMANDS");
        }

        return {command_contracts::format_commands()};
    }

    if (command == "COMMANDSJSON") {
        if (has_extra_token(input)) {
            return usage("COMMANDSJSON");
        }

        return {command_contracts::format_commands_json()};
    }

    return {"ERR unknown command"};
}

CommandResult CommandProcessor::execute(std::string_view line) {
    const std::string trimmed = trim_copy(line);
    if (trimmed.empty()) {
        return {};
    }

    const std::string command = command_token(trimmed);
    const auto started_at = std::chrono::steady_clock::now();
    CommandResult result = execute_trimmed(trimmed);
    const auto elapsed_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - started_at).count();
    const bool reset_succeeded = command == "RESETSTATS" && result.response == "OK stats reset";
    if (!reset_succeeded) {
        metrics_tracker_->record(command, result, static_cast<std::uint64_t>(elapsed_ns));
    }
    return result;
}

CommandProcessorMetrics CommandProcessor::metrics() const { return metrics_tracker_->stats(); }

} // namespace minikv
