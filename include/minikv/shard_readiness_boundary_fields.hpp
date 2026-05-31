#pragma once

#include <string>

namespace minikv::shard_readiness::boundary_fields {

std::string format_catalog_maintenance_json();
std::string format_read_only_boundary_fields_json();

} // namespace minikv::shard_readiness::boundary_fields
