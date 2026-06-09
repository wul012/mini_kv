#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_material_submission_precheck_integrity_checks;

    const auto catalog = checks::candidate_material_submission_precheck_integrity_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_candidate_material_submission_precheck_integrity_check_count() == 25);
    assert(catalog.front().check_code == std::string_view{"source-precheck-fixture-frozen"});
    assert(catalog.back().check_code == std::string_view{"no-additional-submission-echo"});

    const auto json = checks::format_candidate_material_submission_precheck_integrity_checks_json(25);
    assert_contains(json, "\"checkCode\":\"source-precheck-fixture-frozen\"");
    assert_contains(json, "\"checkCode\":\"checkpoint-count-parity\"");
    assert_contains(json, "\"checkCode\":\"submission-gate-count-parity\"");
    assert_contains(json, "\"checkCode\":\"real-material-absence-boundary\"");
    assert_contains(json, "\"checkCode\":\"write-wal-boundary\"");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_candidate_material_submission_precheck_integrity_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
