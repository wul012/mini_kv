#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_archive_maintenance_verification.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view verification_command = "SHARDROUTEARCHIVEMAINTVERIFYJSON";

} // namespace

int main() {
    const std::string verification =
        minikv::shard_route_preview_archive_maintenance_verification::format_verification_json();
    minikv::test_support::assert_contains(
        verification, "\"contract\":\"shard-route-preview-archive-maintenance-verification.v1\"");
    minikv::test_support::assert_contains(verification, "\"command\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    minikv::test_support::assert_contains(
        verification,
        "\"verificationMode\":\"read-only-route-preview-archive-maintenance-chain-verification\"");
    minikv::test_support::assert_contains(verification, "\"sourceMaintenanceCommand\":\"SHARDROUTEARCHIVEMAINTJSON\"");
    minikv::test_support::assert_contains(verification, "\"sourceMaintenanceReleaseVersion\":\"v485\"");
    minikv::test_support::assert_contains(verification, "\"sourceMaintenancePublishedStageCount\":25");
    minikv::test_support::assert_contains(verification, "\"sourceMaintenanceChainComplete\":true");
    minikv::test_support::assert_contains(
        verification,
        "\"verificationStage\":\"route-preview-archive-maintenance-verification-explain-contract\"");
    minikv::test_support::assert_contains(verification, "\"verificationStageSequence\":8");
    minikv::test_support::assert_contains(verification, "\"verificationReleaseVersion\":\"v493\"");
    minikv::test_support::assert_contains(verification, "\"publishedStageCount\":8");
    minikv::test_support::assert_contains(verification, "\"plannedStageCount\":25");
    minikv::test_support::assert_contains(verification, "\"sourceFrozenReleaseVersion\":\"v492\"");
    minikv::test_support::assert_contains(verification,
                                          "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v492.json\"");
    minikv::test_support::assert_contains(verification, "\"verifiedMaintenanceReleaseRangeStart\":\"v461\"");
    minikv::test_support::assert_contains(verification, "\"verifiedMaintenanceReleaseRangeEnd\":\"v485\"");
    minikv::test_support::assert_contains(verification, "\"stageChainHelperApplied\":true");
    minikv::test_support::assert_contains(verification, "\"verificationCheckCount\":15");
    minikv::test_support::assert_contains(verification, "\"verificationPassedCount\":15");
    minikv::test_support::assert_contains(verification, "\"archiveMaintenanceVerificationCommandAvailable\":true");
    minikv::test_support::assert_contains(verification, "\"archiveMaintenanceVerificationChainComplete\":false");
    minikv::test_support::assert_contains(verification, "\"publishedCountWithinCatalog\":true");
    minikv::test_support::assert_contains(verification, "\"publishedCountMatchesPlan\":false");
    minikv::test_support::assert_contains(verification, "\"sequencesContiguous\":true");
    minikv::test_support::assert_read_only_evidence_boundaries(verification);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewArchiveMaintenanceVerification\":") ==
           1);
    minikv::test_support::assert_contains(shard, "\"releaseVersion\":\"v493\"");
    minikv::test_support::assert_contains(
        shard, "\"status\":\"route-preview-archive-maintenance-verification-explain-contract-read-only\"");
    minikv::test_support::assert_contains(shard, "\"verificationReleaseVersion\":\"v493\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEARCHIVEMAINTVERIFYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEARCHIVEMAINTVERIFYJSON");

    result = processor.execute("SHARDROUTEARCHIVEMAINTVERIFYJSON");
    assert(result.response == verification);
    assert(store.size() == 0);

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEARCHIVEMAINTVERIFYJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEARCHIVEMAINTVERIFYJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, verification_command);

    return 0;
}
