#include "minikv/shard_preview_candidate_material_request_integrity_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_material_request_integrity_checks;

    const auto catalog = checks::candidate_material_request_integrity_checks();
    assert(catalog.size() == 15);
    assert(checks::planned_candidate_material_request_integrity_check_count() == 15);
    assert(catalog.front().check_code == std::string{"source-material-request-fixture-frozen"});
    assert(catalog.back().check_code == std::string{"router-write-wal-execution-boundary"});

    const auto partial_json = checks::format_candidate_material_request_integrity_checks_json(3);
    assert_contains(partial_json, "\"checkCode\":\"source-material-request-fixture-frozen\"");
    assert_contains(partial_json, "\"checkCode\":\"node-stop-condition-respected\"");
    assert_contains(partial_json, "\"checkCode\":\"fixture-range-declared\"");
    assert(partial_json.find("\"checkCode\":\"release-continuity\"") == std::string::npos);

    const auto full_json = checks::format_candidate_material_request_integrity_checks_json(15);
    assert_contains(full_json, "\"checkCode\":\"fixture-encoding-bom-guard\"");
    assert_contains(full_json, "\"checkCode\":\"archive-reference-boundary\"");
    assert_contains(full_json, "\"checkCode\":\"no-material-consumption-boundary\"");
    assert_contains(full_json, "\"materialRead\":false");
    assert_contains(full_json, "\"materialAccepted\":false");
    assert_contains(full_json, "\"documentIntakeOpened\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_candidate_material_request_integrity_checks_json(16);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
