#include "minikv/runtime_evidence.hpp"

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace minikv::runtime_evidence {
namespace {

constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ull;
constexpr std::uint64_t fnv_prime = 1099511628211ull;

std::uint64_t fnv1a64(std::string_view text, std::uint64_t seed = fnv_offset_basis) {
    std::uint64_t hash = seed;
    for (const unsigned char ch : text) {
        hash ^= ch;
        hash *= fnv_prime;
    }
    return hash;
}

void append_part(std::string& source, std::string_view value) {
    source += std::to_string(value.size());
    source.push_back(':');
    source += value;
    source.push_back(';');
}

} // namespace

std::string json_string(std::string_view value) {
    constexpr char hex[] = "0123456789ABCDEF";
    std::string result = "\"";
    for (const unsigned char ch : value) {
        switch (ch) {
        case '"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        case '\b':
            result += "\\b";
            break;
        case '\f':
            result += "\\f";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        default:
            if (ch < 0x20) {
                result += "\\u00";
                result.push_back(hex[(ch >> 4) & 0x0F]);
                result.push_back(hex[ch & 0x0F]);
            } else {
                result.push_back(static_cast<char>(ch));
            }
            break;
        }
    }
    result.push_back('"');
    return result;
}

std::string json_bool(bool value) {
    return value ? "true" : "false";
}

std::string json_string_array(const std::vector<std::string>& values) {
    std::string response = "[";
    bool first = true;
    for (const auto& value : values) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += json_string(value);
    }
    response += "]";
    return response;
}

std::string json_object(const std::vector<JsonField>& fields) {
    std::string response = "{";
    bool first = true;
    for (const auto& field : fields) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "\"";
        response += field.name;
        response += "\":";
        response += field.value;
    }
    response += "}";
    return response;
}

std::string digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    std::string source;
    append_part(source, prefix);
    for (const auto& part : parts) {
        append_part(source, part.value);
    }
    return "fnv1a64:" + [](std::uint64_t value) {
        std::ostringstream output;
        output << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << value;
        return output.str();
    }(fnv1a64(source));
}

} // namespace minikv::runtime_evidence
