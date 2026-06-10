#include "minikv/shard_preview_type_barrel_split_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_type_barrel_split_non_participation_checks;

    const auto records = checks::type_barrel_split_non_participation_checks();
    assert(checks::planned_type_barrel_split_non_participation_check_count() == 25);
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"node-stable-type-barrel-boundary"});
    assert(records.back().check_code == std::string{"node-clean-workspace-ci-closeout"});

    const auto empty = checks::format_type_barrel_split_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_type_barrel_split_non_participation_checks_json(25);
    assert_contains(json, "\"checkCode\":\"node-stable-type-barrel-boundary\"");
    assert_contains(json, "\"checkCode\":\"node-profile-only-module-context\"");
    assert_contains(json, "\"checkCode\":\"node-evidence-endpoint-profile-boundary-context\"");
    assert_contains(json, "\"checkCode\":\"node-clean-workspace-ci-closeout\"");
    assert_contains(json, "\"typeBarrelSplitNonParticipationOnly\":true");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvTypeBarrelSplitStarted\":false");
    assert_contains(json, "\"miniKvConsumesNodeTypes\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_type_barrel_split_non_participation_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
