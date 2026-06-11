#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

int main() {
    namespace checks = minikv::shard_preview_code_walkthrough_quality_gate_non_participation_checks;
    using minikv::test_support::assert_contains;

    const auto records = checks::code_walkthrough_quality_gate_non_participation_checks();
    assert(records.size() == 12);
    assert(checks::planned_code_walkthrough_quality_gate_non_participation_check_count() == 12);
    assert(records.front().sequence == 1);
    assert(records.front().check_code == "node-v2077-plan-observed-only");
    assert(records.back().sequence == 12);
    assert(records.back().check_code == "command-surface-read-only-clean-ci");

    const auto empty = checks::format_code_walkthrough_quality_gate_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_code_walkthrough_quality_gate_non_participation_checks_json(12);
    for (const auto& check : records) {
        assert_contains(json, "\"checkCode\":\"" + std::string{check.check_code} + "\"");
        assert_contains(json, "\"source\":\"" + std::string{check.source} + "\"");
        assert_contains(json, "\"protects\":\"" + std::string{check.protects} + "\"");
    }

    const std::string_view false_fields[] = {
        "nodeWalkthroughFilesReadByMiniKv",
        "nodeQualityRouteExecutedByMiniKv",
        "miniKvScansNodeRepository",
        "placeholderWalkthroughAllowed",
        "unsafeProductionClaimAllowed",
        "bulkHistoricalRelocationAllowed",
        "startsNodeServices",
        "startsJavaService",
        "startsMiniKvService",
        "mutatesStore",
        "touchesWal",
        "executionAllowed",
    };
    for (const auto field : false_fields) {
        assert_contains(json, "\"" + std::string{field} + "\":false");
    }
    assert_contains(json, "\"walkthroughQualityGateOnly\":true");

    bool threw_negative = false;
    try {
        (void)checks::format_code_walkthrough_quality_gate_non_participation_checks_json(-1);
    } catch (const std::out_of_range&) {
        threw_negative = true;
    }
    assert(threw_negative);

    bool threw_too_many = false;
    try {
        (void)checks::format_code_walkthrough_quality_gate_non_participation_checks_json(13);
    } catch (const std::out_of_range&) {
        threw_too_many = true;
    }
    assert(threw_too_many);

    return 0;
}
