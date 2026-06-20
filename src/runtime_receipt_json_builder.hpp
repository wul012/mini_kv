#pragma once

#include <cstdint>
#include <initializer_list>
#include <span>
#include <string>
#include <string_view>

namespace minikv::runtime_receipt_json {

struct OrderedJsonField {
    std::string_view name;
    std::string value_json;
};

std::string json_string(std::string_view value);
std::string json_bool(bool value);
std::string json_integer(std::int64_t value);

std::string json_object(std::span<const OrderedJsonField> fields);
std::string json_object(std::initializer_list<OrderedJsonField> fields);

std::string json_array(std::span<const std::string> values_json);
std::string json_array(std::initializer_list<std::string> values_json);

} // namespace minikv::runtime_receipt_json
