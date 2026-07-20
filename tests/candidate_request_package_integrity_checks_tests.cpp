#include "minikv/shard_preview_candidate_request_package_integrity_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_request_package_integrity_checks;

    const auto catalog = checks::candidate_request_package_integrity_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_candidate_request_package_integrity_check_count() == 25);
    assert(catalog.front().check_code == std::string{"source-fixture-frozen"});
    assert(catalog.back().check_code == std::string{"no-additional-request-echo"});

    const auto partial_json = checks::format_candidate_request_package_integrity_checks_json(3);
    assert_contains(partial_json, "\"checkCode\":\"source-fixture-frozen\"");
    assert_contains(partial_json, "\"checkCode\":\"fixture-range-declared\"");
    assert_contains(partial_json, "\"checkCode\":\"release-continuity\"");
    assert(partial_json.find("\"checkCode\":\"source-lineage-pinned\"") == std::string::npos);

    const auto full_json = checks::format_candidate_request_package_integrity_checks_json(25);
    assert_contains(full_json, "\"checkCode\":\"write-routing-boundary\"");
    assert_contains(full_json, "\"checkCode\":\"wal-boundary\"");
    assert_contains(full_json, "\"checkCode\":\"no-additional-request-echo\"");
    assert_contains(full_json, "\"documentIntakeOpened\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_candidate_request_package_integrity_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
