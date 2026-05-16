#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence {

struct JsonField {
    std::string_view name;
    std::string value;
};

struct DigestPart {
    std::string value;
};

std::string json_string(std::string_view value);
std::string json_bool(bool value);
std::string json_string_array(const std::vector<std::string>& values);
std::string json_object(const std::vector<JsonField>& fields);
std::string digest(std::string_view prefix, const std::vector<DigestPart>& parts);

} // namespace minikv::runtime_evidence
