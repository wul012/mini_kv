#pragma once

#include "test_support.hpp"

#include <string>
#include <string_view>

namespace minikv::test_support {

inline std::size_t occurrence_count(std::string_view text, std::string_view needle) {
    std::size_t count = 0;
    std::size_t offset = 0;
    while (true) {
        offset = text.find(needle, offset);
        if (offset == std::string_view::npos) {
            return count;
        }
        ++count;
        offset += needle.size();
    }
}

inline void assert_read_only_evidence_boundaries(const std::string& evidence) {
    assert_contains(evidence, "\"filesystemReadPerformed\":false");
    assert_contains(evidence, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(evidence, "\"readOnly\":true");
    assert_contains(evidence, "\"mutatesStore\":false");
    assert_contains(evidence, "\"touchesWal\":false");
    assert_contains(evidence, "\"activeRouterInstalled\":false");
    assert_contains(evidence, "\"writeRoutingAllowed\":false");
    assert_contains(evidence, "\"writeCommandsAllowed\":false");
    assert_contains(evidence, "\"loadRestoreCompactAllowed\":false");
    assert_contains(evidence, "\"startsMiniKvService\":false");
    assert_contains(evidence, "\"liveReadAllowed\":false");
    assert_contains(evidence, "\"executionAllowed\":false");
    assert_not_contains(evidence, "\"executionAllowed\":true");
}

inline void assert_metadata_command_contract(const std::string& explain_json,
                                             const std::string& check_json,
                                             std::string_view command) {
    assert_contains(explain_json, "\"command\":\"" + std::string{command} + "\"");
    assert_contains(explain_json, "\"category\":\"read\"");
    assert_contains(explain_json, "\"mutates_store\":false");
    assert_contains(explain_json, "\"touches_wal\":false");
    assert_contains(explain_json, "\"allowed_by_parser\":true");
    assert_contains(explain_json, "\"side_effects\":[\"metadata_read\"]");

    assert_contains(check_json, "\"command\":\"" + std::string{command} + "\"");
    assert_contains(check_json, "\"read_only\":true");
    assert_contains(check_json, "\"write_command\":false");
    assert_contains(check_json, "\"execution_allowed\":false");
    assert_not_contains(check_json, "\"execution_allowed\":true");
}

} // namespace minikv::test_support
