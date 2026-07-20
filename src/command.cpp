#include "minikv/command.hpp"

#include "command_evidence_dispatch.hpp"
#include "command_parse_helpers.hpp"
#include "command_wal_gate.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/command_response_formatters.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview.hpp"
#include "minikv/shard_route_preview_verification.hpp"
#include "minikv/shard_route_preview_verification_report.hpp"
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

    if (const auto* evidence = command_internal::find_evidence_command(command)) {
        if (has_extra_token(input)) {
            return usage(evidence->name);
        }

        return {evidence->format()};
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
