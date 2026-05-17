#pragma once

#include <cctype>
#include <string>
#include <string_view>

namespace minikv {

[[nodiscard]] inline std::string trim_copy(std::string_view text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())) != 0) {
        text.remove_prefix(1);
    }

    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())) != 0) {
        text.remove_suffix(1);
    }

    return std::string{text};
}

[[nodiscard]] inline std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

} // namespace minikv
