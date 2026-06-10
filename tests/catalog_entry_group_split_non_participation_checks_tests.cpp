#include "minikv/shard_preview_catalog_entry_group_split_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_catalog_entry_group_split_non_participation_checks;

    const auto records = checks::catalog_entry_group_split_non_participation_checks();
    assert(checks::planned_catalog_entry_group_split_non_participation_check_count() == 35);
    assert(records.size() == 35);
    assert(records.front().check_code == std::string{"source-release-evidence-review-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-catalog-entry-group-split-chain-created"});

    const auto empty = checks::format_catalog_entry_group_split_non_participation_checks_json(0);
    assert(empty == "[]");

    const auto json = checks::format_catalog_entry_group_split_non_participation_checks_json(35);
    assert_contains(json, "\"checkCode\":\"node-catalog-types-module-context\"");
    assert_contains(json, "\"checkCode\":\"node-aggregate-registry-context\"");
    assert_contains(json, "\"checkCode\":\"node-no-service-startup-context\"");
    assert_contains(json, "\"checkCode\":\"no-catalog-entry-group-split-chain-created\"");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationOnly\":true");
    assert_contains(json, "\"nodeCatalogEntryRecordsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogRegistryExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogEndpointReadByMiniKv\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvCatalogSplitStarted\":false");
    assert_contains(json, "\"catalogEndpointRead\":false");
    assert_contains(json, "\"catalogRecordsImported\":false");
    assert_contains(json, "\"catalogRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_catalog_entry_group_split_non_participation_checks_json(36);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
