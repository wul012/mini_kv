#pragma once

#include "minikv/runtime_evidence.hpp"

#include <cassert>
#include <string>
#include <string_view>
#include <utility>

namespace minikv::test_support {

inline std::string stable_fixture_digest(std::string_view prefix, const std::string& object_json) {
    return runtime_evidence::digest(prefix, {{object_json}});
}

inline std::size_t count_occurrences(std::string_view text, std::string_view needle) {
    assert(!needle.empty());
    std::size_t count = 0;
    for (std::size_t position = text.find(needle); position != std::string_view::npos;
         position = text.find(needle, position + needle.size())) {
        ++count;
    }
    return count;
}

inline std::size_t count_top_level_fields(std::string_view object_json) {
    assert(!object_json.empty() && object_json.front() == '{' && object_json.back() == '}');

    std::size_t depth = 0;
    std::size_t count = 0;
    bool in_string = false;
    bool escaped = false;
    bool top_level_string_candidate = false;

    for (std::size_t index = 0; index < object_json.size(); ++index) {
        const char ch = object_json[index];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
                if (top_level_string_candidate) {
                    std::size_t cursor = index + 1;
                    while (cursor < object_json.size() &&
                           (object_json[cursor] == ' ' || object_json[cursor] == '\t' || object_json[cursor] == '\n' ||
                            object_json[cursor] == '\r')) {
                        ++cursor;
                    }
                    if (cursor < object_json.size() && object_json[cursor] == ':') {
                        ++count;
                    }
                }
                top_level_string_candidate = false;
            }
            continue;
        }

        if (ch == '"') {
            in_string = true;
            top_level_string_candidate = depth == 1;
        } else if (ch == '{' || ch == '[') {
            ++depth;
        } else if (ch == '}' || ch == ']') {
            assert(depth > 0);
            --depth;
        }
    }

    assert(depth == 0);
    assert(!in_string);
    return count;
}

inline std::string replace_once(std::string text, std::string_view from, std::string_view to) {
    const auto position = text.find(std::string{from});
    assert(position != std::string::npos);
    assert(text.find(std::string{from}, position + from.size()) == std::string::npos);
    text.replace(position, from.size(), std::string{to});
    return text;
}

inline std::string named_apostrophe_runtime_surface_canonical_json(std::string fixture_object_json,
                                                                   std::string_view fixture_legacy_phrase,
                                                                   std::string_view runtime_canonical_phrase) {
    assert(count_occurrences(fixture_object_json, "\\u0027") == 1);
    assert(count_occurrences(fixture_object_json, fixture_legacy_phrase) == 1);
    assert(count_occurrences(fixture_object_json, runtime_canonical_phrase) == 0);
    return replace_once(std::move(fixture_object_json), fixture_legacy_phrase, runtime_canonical_phrase);
}

} // namespace minikv::test_support
