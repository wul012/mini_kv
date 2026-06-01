#pragma once

#include <string>

namespace minikv::shard_readiness::history {

std::string format_diagnostics_json();
std::string format_fixture_parity_json();
std::string format_archive_compatibility_json();
std::string format_historical_fallback_json();
std::string format_history_formatter_maintenance_json();

} // namespace minikv::shard_readiness::history
