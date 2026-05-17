#pragma once

#include "minikv/command.hpp"
#include "minikv/wal.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::command_response_formatters {

std::string format_keys(const std::vector<std::string>& keys);
std::string format_prefixed_keys(std::string_view prefix, const std::vector<std::string>& keys);
std::string format_keys_json(std::optional<std::string_view> prefix, std::vector<std::string> keys);
std::string format_walinfo(const WalMaintenanceReport& report);
std::string format_stats(std::size_t live_keys,
                         WriteAheadLog* wal,
                         const std::optional<WalMaintenanceReport>& wal_report,
                         const CommandProcessorMetrics& command_metrics,
                         const CommandConnectionStats& stats);
std::string format_health(std::size_t live_keys,
                          WriteAheadLog* wal,
                          const std::optional<WalMaintenanceReport>& wal_report,
                          const CommandProcessorMetrics& command_metrics,
                          const CommandConnectionStats& stats);
std::string format_info(std::size_t live_keys,
                        WriteAheadLog* wal,
                        const CommandRuntimeInfo& runtime_info);
std::string format_info_json(std::size_t live_keys,
                             WriteAheadLog* wal,
                             const CommandRuntimeInfo& runtime_info);
std::string format_stats_json(std::size_t live_keys,
                              WriteAheadLog* wal,
                              const std::optional<WalMaintenanceReport>& wal_report,
                              const CommandProcessorMetrics& command_metrics,
                              const CommandConnectionStats& stats);
std::string format_smoke_json(std::size_t live_keys,
                              WriteAheadLog* wal,
                              const std::optional<WalMaintenanceReport>& wal_report,
                              const CommandProcessorMetrics& command_metrics,
                              const CommandConnectionStats& stats,
                              const CommandRuntimeInfo& runtime_info);
std::string format_storage_evidence_json(std::size_t live_keys,
                                          WriteAheadLog* wal,
                                          const std::optional<WalMaintenanceReport>& wal_report);

} // namespace minikv::command_response_formatters
