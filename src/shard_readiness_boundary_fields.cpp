#include "minikv/shard_readiness_boundary_fields.hpp"

#include "shard_readiness_boundary_field_names.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_readiness::boundary_fields {

std::string format_read_only_boundary_fields_json() {
    return runtime_evidence::json_string_array(read_only_boundary_field_names());
}

} // namespace minikv::shard_readiness::boundary_fields
