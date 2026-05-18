#pragma once

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::managed_audit_receipts::detail {

using runtime_evidence::JsonField;

inline std::string bool_part(bool value) {
    return runtime_evidence::json_bool(value);
}

inline std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

inline std::string field_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

inline std::string json_object(const std::vector<JsonField>& fields) {
    return runtime_evidence::json_object(fields);
}

} // namespace minikv::managed_audit_receipts::detail
