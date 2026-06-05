#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_archive_maintenance.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view maintenance_command = "SHARDROUTEARCHIVEMAINTJSON";

} // namespace

int main() {
    const std::string maintenance =
        minikv::shard_route_preview_archive_maintenance::format_maintenance_json();
    minikv::test_support::assert_contains(maintenance,
                                          "\"contract\":\"shard-route-preview-archive-maintenance.v1\"");
    minikv::test_support::assert_contains(maintenance, "\"command\":\"SHARDROUTEARCHIVEMAINTJSON\"");
    minikv::test_support::assert_contains(
        maintenance, "\"maintenanceMode\":\"read-only-route-preview-archive-maintenance-ledger\"");
    minikv::test_support::assert_contains(
        maintenance,
        "\"sourceArchiveVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    minikv::test_support::assert_contains(maintenance, "\"sourceArchiveVerificationReleaseVersion\":\"v460\"");
    minikv::test_support::assert_contains(maintenance, "\"sourceArchiveVerificationPublishedStageCount\":20");
    minikv::test_support::assert_contains(maintenance, "\"sourceArchiveVerificationChainComplete\":true");
    minikv::test_support::assert_contains(maintenance,
                                          "\"maintenanceStage\":\"route-preview-archive-maintenance-walkthrough-index\"");
    minikv::test_support::assert_contains(maintenance, "\"maintenanceStageSequence\":16");
    minikv::test_support::assert_contains(maintenance, "\"maintenanceReleaseVersion\":\"v476\"");
    minikv::test_support::assert_contains(maintenance, "\"publishedStageCount\":16");
    minikv::test_support::assert_contains(maintenance, "\"plannedStageCount\":25");
    minikv::test_support::assert_contains(maintenance, "\"sourceFrozenReleaseVersion\":\"v475\"");
    minikv::test_support::assert_contains(maintenance,
                                          "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v475.json\"");
    minikv::test_support::assert_contains(maintenance, "\"maintainedReleaseRangeStart\":\"v441\"");
    minikv::test_support::assert_contains(maintenance, "\"maintainedReleaseRangeEnd\":\"v460\"");
    minikv::test_support::assert_contains(maintenance, "\"sharedStageCatalogFormatterApplied\":true");
    minikv::test_support::assert_contains(maintenance, "\"focusedTestHelperApplied\":true");
    minikv::test_support::assert_contains(maintenance, "\"maintenanceCheckCount\":14");
    minikv::test_support::assert_contains(maintenance, "\"maintenancePassedCount\":14");
    minikv::test_support::assert_contains(maintenance, "\"archiveMaintenanceCommandAvailable\":true");
    minikv::test_support::assert_contains(maintenance, "\"archiveMaintenanceChainComplete\":false");
    minikv::test_support::assert_read_only_evidence_boundaries(maintenance);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewArchiveMaintenance\":") == 1);
    minikv::test_support::assert_contains(shard, "\"releaseVersion\":\"v476\"");
    minikv::test_support::assert_contains(shard, "\"status\":\"route-preview-archive-maintenance-walkthrough-index-read-only\"");
    minikv::test_support::assert_contains(shard, "\"maintenanceReleaseVersion\":\"v476\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEARCHIVEMAINTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEARCHIVEMAINTJSON");

    result = processor.execute("SHARDROUTEARCHIVEMAINTJSON");
    assert(result.response == maintenance);
    assert(store.size() == 0);

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEARCHIVEMAINTJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEARCHIVEMAINTJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, maintenance_command);

    return 0;
}
