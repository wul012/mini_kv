#pragma once

#include <cctype>
#include <optional>
#include <string>
#include <string_view>

namespace minikv::test_support {
namespace receipt_fixture_parity_detail {

inline bool is_json_whitespace(char ch) { return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'; }

inline std::optional<std::size_t> json_string_end(std::string_view text, std::size_t start) {
    if (start >= text.size() || text[start] != '"') {
        return std::nullopt;
    }

    bool escaped = false;
    for (std::size_t index = start + 1; index < text.size(); ++index) {
        const unsigned char ch = static_cast<unsigned char>(text[index]);
        if (escaped) {
            escaped = false;
            continue;
        }
        if (ch == '\\') {
            escaped = true;
            continue;
        }
        if (ch == '"') {
            return index + 1;
        }
        if (ch < 0x20) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

inline std::optional<std::size_t> json_object_end(std::string_view text, std::size_t start) {
    if (start >= text.size() || text[start] != '{') {
        return std::nullopt;
    }

    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;
    for (std::size_t index = start; index < text.size(); ++index) {
        const char ch = text[index];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }

        if (ch == '"') {
            in_string = true;
        } else if (ch == '{') {
            ++depth;
        } else if (ch == '}') {
            if (depth == 0) {
                return std::nullopt;
            }
            --depth;
            if (depth == 0) {
                return index + 1;
            }
        }
    }
    return std::nullopt;
}

inline bool field_name_is_plain(std::string_view field_name) {
    if (field_name.empty()) {
        return false;
    }
    for (const unsigned char ch : field_name) {
        if (ch < 0x20 || ch == '"' || ch == '\\') {
            return false;
        }
    }
    return true;
}

inline bool json_structure_is_balanced(std::string_view text) {
    std::string stack;
    bool in_string = false;
    bool escaped = false;

    for (const unsigned char raw_ch : text) {
        const char ch = static_cast<char>(raw_ch);
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            } else if (raw_ch < 0x20) {
                return false;
            }
            continue;
        }

        if (ch == '"') {
            in_string = true;
        } else if (ch == '{' || ch == '[') {
            stack.push_back(ch);
        } else if (ch == '}' || ch == ']') {
            if (stack.empty()) {
                return false;
            }
            const char expected_open = ch == '}' ? '{' : '[';
            if (stack.back() != expected_open) {
                return false;
            }
            stack.pop_back();
        }
    }

    return !in_string && !escaped && stack.empty();
}

} // namespace receipt_fixture_parity_detail

inline std::optional<std::string> extract_json_object_field(std::string_view text, std::string_view field_name) {
    using namespace receipt_fixture_parity_detail;

    if (!field_name_is_plain(field_name) || !json_structure_is_balanced(text)) {
        return std::nullopt;
    }

    const std::string key = "\"" + std::string{field_name} + "\"";
    std::optional<std::string> extracted;

    for (std::size_t index = 0; index < text.size();) {
        if (text[index] != '"') {
            ++index;
            continue;
        }

        const auto token_end = json_string_end(text, index);
        if (!token_end.has_value()) {
            return std::nullopt;
        }

        const bool key_matches = text.substr(index, *token_end - index) == key;
        std::size_t cursor = *token_end;
        while (cursor < text.size() && is_json_whitespace(text[cursor])) {
            ++cursor;
        }

        if (key_matches && cursor < text.size() && text[cursor] == ':') {
            ++cursor;
            while (cursor < text.size() && is_json_whitespace(text[cursor])) {
                ++cursor;
            }
            if (cursor >= text.size() || text[cursor] != '{') {
                return std::nullopt;
            }

            const auto object_end = json_object_end(text, cursor);
            if (!object_end.has_value() || extracted.has_value()) {
                return std::nullopt;
            }
            extracted = std::string{text.substr(cursor, *object_end - cursor)};
        }

        index = *token_end;
    }

    return extracted;
}

} // namespace minikv::test_support
