#include "runtime_receipt_json_builder.hpp"

#include "minikv/runtime_evidence.hpp"

#include <vector>

namespace minikv::runtime_receipt_json {

std::string json_string(std::string_view value) { return runtime_evidence::json_string(value); }

std::string json_bool(bool value) { return runtime_evidence::json_bool(value); }

std::string json_integer(std::int64_t value) { return std::to_string(value); }

std::string json_object(std::span<const OrderedJsonField> fields) {
    std::vector<runtime_evidence::JsonField> encoded_fields;
    encoded_fields.reserve(fields.size());
    for (const auto& field : fields) {
        encoded_fields.push_back({field.name, field.value_json});
    }
    return runtime_evidence::json_object(encoded_fields);
}

std::string json_object(std::initializer_list<OrderedJsonField> fields) {
    return json_object(std::span<const OrderedJsonField>{fields.begin(), fields.size()});
}

std::string json_array(std::span<const std::string> values_json) {
    std::string response = "[";
    bool first = true;
    for (const auto& value_json : values_json) {
        if (!first) {
            response.push_back(',');
        }
        first = false;
        response += value_json;
    }
    response.push_back(']');
    return response;
}

std::string json_array(std::initializer_list<std::string> values_json) {
    return json_array(std::span<const std::string>{values_json.begin(), values_json.size()});
}

} // namespace minikv::runtime_receipt_json
