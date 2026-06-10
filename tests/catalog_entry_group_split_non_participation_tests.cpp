#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_catalog_entry_group_split_non_participation;

    const auto json = evidence::format_catalog_entry_group_split_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-catalog-entry-group-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeCatalogEntryGroupSplitCloseoutReleaseVersion\":\"Node v1756\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeCatalogEntryGroupSplitOnly\":true");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1295\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":35");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1330\"");
    assert_contains(json, "\"plannedCatalogEntryGroupSplitNonParticipationCheckCount\":35");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationOnly\":true");
    assert_contains(json, "\"catalog-interfaces-types-module\"");
    assert_contains(json, "\"aggregate-entry-group-registry\"");
    assert_contains(json, "\"no-service-startup-constraint\"");
    assert_contains(json, "\"github-actions-closeout\"");
    assert_contains(json, "\"nodeCatalogEntryRecordsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogRegistryExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogMarkdownRenderedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsCatalogSplit\":false");
    assert_contains(json, "\"miniKvConsumesCatalogRecords\":false");
    assert_contains(json, "\"miniKvReadsCatalogEndpoint\":false");
    assert_contains(json, "\"catalogEndpointRead\":false");
    assert_contains(json, "\"catalogRecordsImported\":false");
    assert_contains(json, "\"catalogRegistryExecuted\":false");
    assert_contains(json, "\"catalogMarkdownRendered\":false");
    assert_contains(json, "\"catalogRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":true");

    const auto digest = evidence::catalog_entry_group_split_non_participation_digest_marker();
    assert(digest.find("v1330-route-preview-catalog-entry-group-split-non-participation-github-actions-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 35);
    assert(evidence::catalog_entry_group_split_non_participation_status() == "route-preview-catalog-entry-group-split-non-participation-github-actions-closeout-read-only");

    return 0;
}
