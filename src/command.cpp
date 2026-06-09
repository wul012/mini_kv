#include "minikv/command.hpp"

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
#include "minikv/shard_preview_candidate_request_package.hpp"
#include "minikv/shard_preview_candidate_request_package_integrity.hpp"
#include "minikv/shard_preview_candidate_submission_precheck.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/string_utils.hpp"
#include "minikv/snapshot.hpp"
#include "minikv/wal.hpp"

#include <array>
#include <chrono>
#include <condition_variable>
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

std::string command_token(std::string_view text) {
    std::istringstream input{std::string{text}};
    std::string command;
    input >> command;
    return to_upper(command);
}

enum class CommandDispatchVerb {
    Ping,
    Set,
    SetNxEx,
    Get,
    Del,
    Expire,
    Ttl,
    Size,
    Keys,
    KeysJson,
    Save,
    Load,
    Compact,
    WalInfo,
    ResetStats,
    RuntimeEvidence,
    ExplainJson,
    CheckJson,
    Help,
    Quit,
    Unknown,
};

struct CommandDispatchEntry {
    std::string_view command;
    CommandDispatchVerb verb;
};

constexpr std::array<CommandDispatchEntry, 64> command_dispatch_table = {{
    {"PING", CommandDispatchVerb::Ping},
    {"SET", CommandDispatchVerb::Set},
    {"SETNXEX", CommandDispatchVerb::SetNxEx},
    {"GET", CommandDispatchVerb::Get},
    {"DEL", CommandDispatchVerb::Del},
    {"EXPIRE", CommandDispatchVerb::Expire},
    {"TTL", CommandDispatchVerb::Ttl},
    {"SIZE", CommandDispatchVerb::Size},
    {"KEYS", CommandDispatchVerb::Keys},
    {"KEYSJSON", CommandDispatchVerb::KeysJson},
    {"SAVE", CommandDispatchVerb::Save},
    {"LOAD", CommandDispatchVerb::Load},
    {"COMPACT", CommandDispatchVerb::Compact},
    {"WALINFO", CommandDispatchVerb::WalInfo},
    {"RESETSTATS", CommandDispatchVerb::ResetStats},
    {"STATS", CommandDispatchVerb::RuntimeEvidence},
    {"STATSJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SMOKEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"STORAGEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"HEALTH", CommandDispatchVerb::RuntimeEvidence},
    {"INFO", CommandDispatchVerb::RuntimeEvidence},
    {"INFOJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTE", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTCLOSEOUTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEARCHIVEMAINTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEARCHIVEMAINTVERIFYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEWORKSHEETVERIFYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEIMPORTPREFLIGHTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUEDRAFTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYPRECHECKJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON",
     CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTECANDIDATEINTAKEPACKETJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTECANDIDATEMATERIALREQUESTJSON", CommandDispatchVerb::RuntimeEvidence},
    {"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON", CommandDispatchVerb::RuntimeEvidence},
    {"COMMANDS", CommandDispatchVerb::RuntimeEvidence},
    {"COMMANDSJSON", CommandDispatchVerb::RuntimeEvidence},
    {"EXPLAINJSON", CommandDispatchVerb::ExplainJson},
    {"CHECKJSON", CommandDispatchVerb::CheckJson},
    {"HELP", CommandDispatchVerb::Help},
    {"EXIT", CommandDispatchVerb::Quit},
    {"QUIT", CommandDispatchVerb::Quit},
}};

CommandDispatchVerb lookup_command_dispatch_verb(std::string_view command) {
    for (const auto& entry : command_dispatch_table) {
        if (entry.command == command) {
            return entry.verb;
        }
    }
    return CommandDispatchVerb::Unknown;
}

std::string metrics_command_name(std::string_view command) {
    std::string name = to_upper(command);
    if (name.empty() || !command_contracts::is_known_command(name)) {
        return "UNKNOWN";
    }
    return name;
}

bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}

CommandResult usage(std::string_view command) {
    return {std::string{"ERR usage: "} + std::string{command}};
}

CommandResult wal_error() {
    return {"ERR wal append failed"};
}

CommandResult wal_disabled_error() {
    return {"ERR WAL not enabled"};
}

CommandResult wal_compact_error() {
    return {"ERR wal compact failed"};
}

CommandResult snapshot_error(std::string_view action) {
    return {std::string{"ERR snapshot "} + std::string{action} + " failed"};
}

// Keeps WAL append+mutation atomic while letting compaction run outside the write command scope.
class WalCommandGate {
public:
    class Scope {
    public:
        Scope() = default;
        Scope(WalCommandGate& gate, bool compaction) : gate_(&gate), compaction_(compaction) {}

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

        Scope(Scope&& other) noexcept : gate_(other.gate_), compaction_(other.compaction_) {
            other.gate_ = nullptr;
        }

        Scope& operator=(Scope&& other) noexcept {
            if (this != &other) {
                release();
                gate_ = other.gate_;
                compaction_ = other.compaction_;
                other.gate_ = nullptr;
            }
            return *this;
        }

        ~Scope() {
            release();
        }

    private:
        void release() {
            if (gate_ == nullptr) {
                return;
            }
            gate_->leave(compaction_);
            gate_ = nullptr;
        }

        WalCommandGate* gate_ = nullptr;
        bool compaction_ = false;
    };

    Scope enter_command() {
        std::unique_lock lock{mutex_};
        condition_.wait(lock, [this] { return !command_active_ && !compaction_active_; });
        command_active_ = true;
        return Scope{*this, false};
    }

    Scope enter_compaction() {
        std::unique_lock lock{mutex_};
        condition_.wait(lock, [this] { return !command_active_ && !compaction_active_; });
        compaction_active_ = true;
        return Scope{*this, true};
    }

private:
    void leave(bool compaction) {
        {
            std::lock_guard lock{mutex_};
            if (compaction) {
                compaction_active_ = false;
            } else {
                command_active_ = false;
            }
        }
        condition_.notify_all();
    }

    std::mutex mutex_;
    std::condition_variable condition_;
    bool command_active_ = false;
    bool compaction_active_ = false;
};

WalCommandGate& wal_command_gate() {
    static WalCommandGate gate;
    return gate;
}

std::optional<std::chrono::seconds> parse_positive_seconds(std::string_view text) {
    std::istringstream input{std::string{text}};
    long long seconds = 0;
    char extra = '\0';

    if (!(input >> seconds) || seconds <= 0 || (input >> extra)) {
        return std::nullopt;
    }

    return std::chrono::seconds{seconds};
}

std::string format_expires_at(std::string_view key, Store::TimePoint expires_at) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"EXPIREAT "} + std::string{key} + " " + std::to_string(epoch_millis);
}

std::string format_setex_at(std::string_view key, Store::TimePoint expires_at, std::string_view value) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"SETEXAT "} + std::string{key} + " " + std::to_string(epoch_millis) + " " + std::string{value};
}

CommandConnectionStats connection_stats(const CommandProcessorOptions& options) {
    if (options.connection_stats) {
        return options.connection_stats();
    }
    return {};
}

} // namespace

void CommandMetricsTracker::record(const CommandResult& result) {
    record("UNKNOWN", result, 0);
}

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
    : store_(store),
      wal_(wal),
      options_(std::move(options)),
      metrics_tracker_(options_.metrics_tracker ? options_.metrics_tracker : std::make_shared<CommandMetricsTracker>()) {
}

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
        return {command_response_formatters::format_stats(
            live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
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
        return {command_response_formatters::format_stats_json(
            live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
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
        return {command_response_formatters::format_smoke_json(
            live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_), options_.runtime_info)};
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
        return {command_response_formatters::format_health(
            live_keys, wal_, wal_report, metrics_tracker_->stats(), connection_stats(options_))};
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

        return {shard_route_preview_operator_value_supply_approval_template::
                    format_value_supply_approval_template_json()};
    }
    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_template::
                    format_signed_approval_template_json()};
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

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight::
                    format_signed_approval_capture_artifact_draft_text_package_review_preflight_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit::
                    format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit::
                    format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON");
        }

        return {shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit::
                    format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_json()};
    }

    if (command == "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON") {
        if (has_extra_token(input)) {
            return usage("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON");
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

CommandProcessorMetrics CommandProcessor::metrics() const {
    return metrics_tracker_->stats();
}

CommandResult CommandProcessor::execute_trimmed(std::string_view trimmed) {
    std::istringstream input{std::string{trimmed}};

    std::string command;
    input >> command;
    command = to_upper(command);

    switch (lookup_command_dispatch_verb(command)) {
    case CommandDispatchVerb::Ping: {
        std::string message;
        std::getline(input >> std::ws, message);

        return {message.empty() ? "PONG" : message};
    }

    case CommandDispatchVerb::Set: {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return usage("SET key value");
        }

        return execute_with_wal(
            [key, value] { return std::optional<std::string>{std::string{"SET "} + key + " " + value}; },
            [this, key, value] {
                const bool inserted = store_.set(key, value);
                return CommandResult{inserted ? "OK inserted" : "OK updated"};
            });
    }

    case CommandDispatchVerb::SetNxEx: {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        std::string value;
        std::getline(input >> std::ws, value);

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || value.empty()) {
            return usage("SETNXEX key seconds value");
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return execute_with_wal(
            [this, key, expires_at, value]() -> std::optional<std::string> {
                if (store_.contains(key)) {
                    return std::nullopt;
                }
                return format_setex_at(key, expires_at, value);
            },
            [this, key, value, expires_at] {
                const bool claimed = store_.set_if_absent(key, value, expires_at);
                return CommandResult{claimed ? "1" : "0"};
            });
    }

    case CommandDispatchVerb::Get: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("GET key");
        }

        const auto value = store_.get(key);
        if (!value.has_value()) {
            return {"(nil)"};
        }

        return {*value};
    }

    case CommandDispatchVerb::Del: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("DEL key");
        }

        return execute_with_wal(
            [this, key]() -> std::optional<std::string> {
                if (!store_.contains(key)) {
                    return std::nullopt;
                }
                return std::string{"DEL "} + key;
            },
            [this, key] {
                const bool erased = store_.erase(key);
                return CommandResult{erased ? "1" : "0"};
            });
    }

    case CommandDispatchVerb::Expire: {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
            return usage("EXPIRE key seconds");
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return execute_with_wal(
            [this, key, expires_at]() -> std::optional<std::string> {
                if (!store_.contains(key)) {
                    return std::nullopt;
                }
                return format_expires_at(key, expires_at);
            },
            [this, key, expires_at] {
                const bool updated = store_.expire_at(key, expires_at);
                return CommandResult{updated ? "1" : "0"};
            });
    }

    case CommandDispatchVerb::Ttl: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("TTL key");
        }

        const auto ttl = store_.ttl(key);
        if (!ttl.has_value()) {
            return {"-2"};
        }

        return {std::to_string(ttl->count())};
    }

    case CommandDispatchVerb::Size: {
        if (has_extra_token(input)) {
            return usage("SIZE");
        }

        return {std::to_string(store_.size())};
    }

    case CommandDispatchVerb::Keys: {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYS [prefix]");
            }

            return {command_response_formatters::format_prefixed_keys(prefix, store_.keys_with_prefix(prefix))};
        }

        return {command_response_formatters::format_keys(store_.keys())};
    }

    case CommandDispatchVerb::KeysJson: {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYSJSON [prefix]");
            }

            return {command_response_formatters::format_keys_json(prefix, store_.keys_with_prefix(prefix))};
        }

        return {command_response_formatters::format_keys_json(std::nullopt, store_.keys())};
    }

    case CommandDispatchVerb::Save: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("SAVE path");
        }

        std::size_t saved = 0;
        if (!SnapshotFile::save(store_, path, &saved)) {
            return snapshot_error("save");
        }

        return {std::string{"OK saved "} + std::to_string(saved)};
    }

    case CommandDispatchVerb::Load: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("LOAD path");
        }

        std::size_t loaded = 0;
        if (!SnapshotFile::load(store_, path, &loaded)) {
            return snapshot_error("load");
        }

        return {std::string{"OK loaded "} + std::to_string(loaded)};
    }

    case CommandDispatchVerb::Compact: {
        if (has_extra_token(input)) {
            return usage("COMPACT");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::size_t compacted = 0;
        {
            auto scope = wal_command_gate().enter_compaction();
            if (!wal_->compact(store_, &compacted)) {
                return wal_compact_error();
            }
        }

        return {std::string{"OK compacted "} + std::to_string(compacted)};
    }

    case CommandDispatchVerb::WalInfo: {
        if (has_extra_token(input)) {
            return usage("WALINFO");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        auto scope = wal_command_gate().enter_command();
        return {command_response_formatters::format_walinfo(wal_->maintenance_report(store_))};
    }

    case CommandDispatchVerb::ResetStats: {
        if (has_extra_token(input)) {
            return usage("RESETSTATS");
        }

        metrics_tracker_->reset();
        return {"OK stats reset"};
    }

    case CommandDispatchVerb::RuntimeEvidence:
        return execute_runtime_evidence_command(command, input);

    case CommandDispatchVerb::ExplainJson: {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("EXPLAINJSON command");
        }

        return {command_contracts::format_explain_json(target_command)};
    }

    case CommandDispatchVerb::CheckJson: {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("CHECKJSON command");
        }

        return {command_contracts::format_check_json(target_command, wal_ != nullptr)};
    }

    case CommandDispatchVerb::Help:
        if (has_extra_token(input)) {
            return usage("HELP");
        }

        return {help_text()};

    case CommandDispatchVerb::Quit:
        if (has_extra_token(input)) {
            return usage("QUIT");
        }

        return {"BYE", true};

    case CommandDispatchVerb::Unknown:
        return {"ERR unknown command"};
    }
    return {"ERR unknown command"};
}

std::string CommandProcessor::help_text() {
    return "Commands:\n"
           "  PING [message]\n"
           "  SET key value\n"
           "  SETNXEX key seconds value\n"
           "  GET key\n"
           "  DEL key\n"
           "  EXPIRE key seconds\n"
           "  TTL key\n"
           "  SIZE\n"
           "  KEYS [prefix]\n"
           "  KEYSJSON [prefix]\n"
           "  SAVE path\n"
           "  LOAD path\n"
           "  COMPACT\n"
           "  WALINFO\n"
           "  STATS\n"
           "  STATSJSON\n"
           "  SMOKEJSON\n"
           "  RESETSTATS\n"
           "  HEALTH\n"
           "  INFO\n"
           "  INFOJSON\n"
           "  SHARDJSON\n"
           "  SHARDROUTE key\n"
           "  SHARDROUTEJSON key\n"
           "  SHARDROUTEVERIFYJSON key\n"
           "  SHARDROUTEVERIFYREPORTJSON key\n"
           "  SHARDROUTEVERIFYREPORTCLOSEOUTJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVEJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\n"
           "  SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\n"
           "  SHARDROUTEARCHIVEMAINTJSON\n"
           "  SHARDROUTEARCHIVEMAINTVERIFYJSON\n"
           "  SHARDROUTEWORKSHEETVERIFYJSON\n"
           "  SHARDROUTEIMPORTPREFLIGHTJSON\n"
           "  SHARDROUTEVALUEDRAFTJSON\n"
           "  SHARDROUTEVALUESUPPLYJSON\n"
           "  SHARDROUTEVALUESUPPLYPRECHECKJSON\n"
           "  SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\n"
           "  SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON\n"
           "  SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\n"
           "  SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\n"
           "  SHARDROUTECANDIDATEINTAKEPACKETJSON\n"
           "  SHARDROUTECANDIDATEMATERIALREQUESTJSON\n"
           "  SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\n"
           "  COMMANDS\n"
           "  COMMANDSJSON\n"
           "  EXPLAINJSON command\n"
           "  CHECKJSON command\n"
           "  STORAGEJSON\n"
           "  HELP\n"
           "  EXIT\n"
           "  QUIT";
}

} // namespace minikv
