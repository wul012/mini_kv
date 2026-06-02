#include "minikv/shard_readiness_boundary_fields.hpp"

#include "shard_readiness_boundary_field_names.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::boundary_fields {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_read_only_boundary_fields_json() {
    return json_string_array(read_only_boundary_field_names());
}

} // namespace minikv::shard_readiness::boundary_fields
