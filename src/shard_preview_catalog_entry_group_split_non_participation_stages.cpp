#include "minikv/shard_preview_catalog_entry_group_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1296;

constexpr std::array<StageRecord, 35> stage_catalog = {{
    {1, "v1296", "route-preview-catalog-entry-group-split-non-participation-type-module-catalog-types-boundary",
     "v1295", "fixtures/release/shard-readiness-v1295.json",
     "records Node v1722 type module catalog interfaces as Node-owned TypeScript types only"},    {2, "v1297", "route-preview-catalog-entry-group-split-non-participation-stable-profile-entrypoint-constant-boundary",
     "v1296", "fixtures/release/shard-readiness-v1296.json",
     "records Node v1723 public stable profile entrypoint constant without importing Node constants"},    {3, "v1298", "route-preview-catalog-entry-group-split-non-participation-foundation-live-window-entry-group-boundary",
     "v1297", "fixtures/release/shard-readiness-v1297.json",
     "keeps Node v1724 foundation and live read-only window entry group outside mini-kv runtime"},    {4, "v1299", "route-preview-catalog-entry-group-split-non-participation-operator-evidence-supply-entry-group-boundary",
     "v1298", "fixtures/release/shard-readiness-v1298.json",
     "keeps Node v1725 operator evidence supply entry group outside mini-kv"},    {5, "v1300", "route-preview-catalog-entry-group-split-non-participation-signed-approval-capture-draft-entry-group-boundary",
     "v1299", "fixtures/release/shard-readiness-v1299.json",
     "records Node v1726 signed approval capture draft entry group without opening approval execution"},    {6, "v1301", "route-preview-catalog-entry-group-split-non-participation-text-package-preflight-entry-group-boundary",
     "v1300", "fixtures/release/shard-readiness-v1300.json",
     "keeps Node v1727 text package preflight entries as Node catalog data"},    {7, "v1302", "route-preview-catalog-entry-group-split-non-participation-text-package-evidence-entry-group-boundary",
     "v1301", "fixtures/release/shard-readiness-v1301.json",
     "keeps Node v1728 text package evidence entries outside mini-kv"},    {8, "v1303", "route-preview-catalog-entry-group-split-non-participation-candidate-document-profile-entry-group-boundary",
     "v1302", "fixtures/release/shard-readiness-v1302.json",
     "records Node v1729 candidate document and profile section group without storing catalog records"},    {9, "v1304", "route-preview-catalog-entry-group-split-non-participation-aggregate-entry-group-registry-boundary",
     "v1303", "fixtures/release/shard-readiness-v1303.json",
     "keeps Node v1730 aggregate entry group registry from becoming a mini-kv registry"},    {10, "v1305", "route-preview-catalog-entry-group-split-non-participation-stable-public-catalog-api-boundary",
     "v1304", "fixtures/release/shard-readiness-v1304.json",
     "records Node v1731 stable public catalog entrypoint API while mini-kv keeps no API dependency"},    {11, "v1306", "route-preview-catalog-entry-group-split-non-participation-defensive-copy-behavior-boundary",
     "v1305", "fixtures/release/shard-readiness-v1305.json",
     "records Node v1732 defensive copy behavior without importing mutable catalog data"},    {12, "v1307", "route-preview-catalog-entry-group-split-non-participation-module-count-reexport-count-boundary",
     "v1306", "fixtures/release/shard-readiness-v1306.json",
     "records Node v1733 computed module counts and stable re-export counts as read-only plan context"},    {13, "v1308", "route-preview-catalog-entry-group-split-non-participation-catalog-validation-check-boundary",
     "v1307", "fixtures/release/shard-readiness-v1307.json",
     "keeps Node v1734 catalog validation checks inside Node"},    {14, "v1309", "route-preview-catalog-entry-group-split-non-participation-contiguous-entry-group-regression-boundary",
     "v1308", "fixtures/release/shard-readiness-v1308.json",
     "records Node v1735 contiguous split entry group regression coverage without executing Node tests"},    {15, "v1310", "route-preview-catalog-entry-group-split-non-participation-entry-group-id-order-boundary",
     "v1309", "fixtures/release/shard-readiness-v1309.json",
     "records Node v1736 entry group id order as maintenance metadata only"},    {16, "v1311", "route-preview-catalog-entry-group-split-non-participation-markdown-renderer-stability-boundary",
     "v1310", "fixtures/release/shard-readiness-v1310.json",
     "keeps Node v1737 Markdown renderer stability outside mini-kv rendering"},    {17, "v1312", "route-preview-catalog-entry-group-split-non-participation-controlled-preview-route-test-boundary",
     "v1311", "fixtures/release/shard-readiness-v1311.json",
     "records Node v1738 controlled read-only shard preview route tests without installing a route"},    {18, "v1313", "route-preview-catalog-entry-group-split-non-participation-review-artifact-barrel-test-boundary",
     "v1312", "fixtures/release/shard-readiness-v1312.json",
     "records Node v1739 review artifact barrel tests without importing review artifacts"},    {19, "v1314", "route-preview-catalog-entry-group-split-non-participation-typecheck-focused-catalog-test-boundary",
     "v1313", "fixtures/release/shard-readiness-v1313.json",
     "records Node v1740 typecheck and focused catalog tests without requiring fresh mini-kv evidence"},    {20, "v1315", "route-preview-catalog-entry-group-split-non-participation-adjacent-route-artifact-test-boundary",
     "v1314", "fixtures/release/shard-readiness-v1314.json",
     "keeps Node v1741 adjacent catalog route and review artifact tests Node-owned"},    {21, "v1316", "route-preview-catalog-entry-group-split-non-participation-build-after-split-boundary",
     "v1315", "fixtures/release/shard-readiness-v1315.json",
     "records Node v1742 build after catalog entry split without sibling startup"},    {22, "v1317", "route-preview-catalog-entry-group-split-non-participation-segmented-vitest-boundary",
     "v1316", "fixtures/release/shard-readiness-v1316.json",
     "keeps Node v1743 segmented Vitest verification from triggering mini-kv execution"},    {23, "v1318", "route-preview-catalog-entry-group-split-non-participation-no-fresh-java-evidence-boundary",
     "v1317", "fixtures/release/shard-readiness-v1317.json",
     "records Node v1744 no fresh Java evidence requirement as sibling-plan context"},    {24, "v1319", "route-preview-catalog-entry-group-split-non-participation-no-fresh-mini-kv-evidence-boundary",
     "v1318", "fixtures/release/shard-readiness-v1318.json",
     "records Node v1745 no fresh mini-kv evidence requirement while this module remains self-contained"},    {25, "v1320", "route-preview-catalog-entry-group-split-non-participation-no-service-startup-boundary",
     "v1319", "fixtures/release/shard-readiness-v1319.json",
     "records Node v1746 no service startup constraint and keeps Java mini-kv stopped"},    {26, "v1321", "route-preview-catalog-entry-group-split-non-participation-entry-group-ownership-doc-boundary",
     "v1320", "fixtures/release/shard-readiness-v1320.json",
     "records Node v1747 six catalog entry groups as Node ownership documentation"},    {27, "v1322", "route-preview-catalog-entry-group-split-non-participation-future-growth-stop-condition-boundary",
     "v1321", "fixtures/release/shard-readiness-v1321.json",
     "records Node v1748 stop condition for future catalog growth without changing mini-kv growth policy"},    {28, "v1323", "route-preview-catalog-entry-group-split-non-participation-catalog-module-size-guard-boundary",
     "v1322", "fixtures/release/shard-readiness-v1322.json",
     "records Node v1749 catalog module below giant-file threshold as maintainability context"},    {29, "v1324", "route-preview-catalog-entry-group-split-non-participation-focused-entry-group-size-boundary",
     "v1323", "fixtures/release/shard-readiness-v1323.json",
     "records Node v1750 each entry group within focused maintenance size"},    {30, "v1325", "route-preview-catalog-entry-group-split-non-participation-explanation-archive-boundary",
     "v1324", "fixtures/release/shard-readiness-v1324.json",
     "records Node v1751 explanation archive without reading Node archive files"},    {31, "v1326", "route-preview-catalog-entry-group-split-non-participation-code-walkthrough-archive-boundary",
     "v1325", "fixtures/release/shard-readiness-v1325.json",
     "records Node v1752 code walkthrough archive without importing docs"},    {32, "v1327", "route-preview-catalog-entry-group-split-non-participation-validation-output-cleanup-boundary",
     "v1326", "fixtures/release/shard-readiness-v1326.json",
     "records Node v1753 generated validation cleanup without mini-kv cleanup authority over Node"},    {33, "v1328", "route-preview-catalog-entry-group-split-non-participation-catalog-split-commit-boundary",
     "v1327", "fixtures/release/shard-readiness-v1327.json",
     "records Node v1754 commit boundary without modifying Node"},    {34, "v1329", "route-preview-catalog-entry-group-split-non-participation-tag-push-boundary",
     "v1328", "fixtures/release/shard-readiness-v1328.json",
     "records Node v1755 tag and push boundary as remote lifecycle metadata only"},    {35, "v1330", "route-preview-catalog-entry-group-split-non-participation-github-actions-closeout",
     "v1329", "fixtures/release/shard-readiness-v1329.json",
     "closes catalog entry group split non-participation with no catalog import route write WAL touch service startup or execution authority"},
}};

} // namespace

std::span<const StageRecord> catalog_entry_group_split_non_participation_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_catalog_entry_group_split_non_participation_release_number() {
    return first_release_number_value;
}

int planned_catalog_entry_group_split_non_participation_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages
