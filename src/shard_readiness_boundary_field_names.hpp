#pragma once

#include <string>
#include <vector>

namespace minikv::shard_readiness::boundary_fields {

const std::vector<std::string>& read_only_boundary_field_names();

} // namespace minikv::shard_readiness::boundary_fields
