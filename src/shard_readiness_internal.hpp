#pragma once

#include <string>
#include <string_view>

namespace minikv::shard_readiness::internal {

constexpr std::string_view contract_version = "shard-readiness.v1";
constexpr std::string_view release_version = "v1607";

std::string json_string(std::string_view value);
std::string evidence_digest();
std::string format_boundaries_json();
std::string format_command_catalog_json();
std::string format_command_catalog_quality_json();

} // namespace minikv::shard_readiness::internal
