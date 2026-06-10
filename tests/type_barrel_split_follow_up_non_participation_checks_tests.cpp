#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_type_barrel_split_follow_up_non_participation_checks;

    const auto records = checks::type_barrel_split_follow_up_non_participation_checks();
    assert(checks::planned_type_barrel_split_follow_up_non_participation_check_count() == 20);
    assert(records.size() == 20);
    assert(records.front().check_code == std::string{"source-type-barrel-split-fixture-freeze"});
    assert(records.back().check_code == std::string{"clean-workspace-ci-closeout"});

    const auto empty = checks::format_type_barrel_split_follow_up_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_type_barrel_split_follow_up_non_participation_checks_json(20);
    assert_contains(json, "\"checkCode\":\"source-type-barrel-split-fixture-freeze\"");
    assert_contains(json, "\"checkCode\":\"profile-endpoint-no-read\"");
    assert_contains(json, "\"checkCode\":\"router-write-wal-guard\"");
    assert_contains(json, "\"checkCode\":\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationOnly\":true");
    assert_contains(json, "\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvTypeBarrelFollowUpStarted\":false");
    assert_contains(json, "\"miniKvConsumesNodeTypes\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_type_barrel_split_follow_up_non_participation_checks_json(21);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
