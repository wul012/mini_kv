#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

std::size_t occurrence_count(std::string_view text, std::string_view needle) {
    std::size_t count = 0;
    std::size_t offset = 0;
    while (true) {
        offset = text.find(needle, offset);
        if (offset == std::string_view::npos) {
            return count;
        }
        ++count;
        offset += needle.size();
    }
}

} // namespace

int main() {
    const std::string archive_verification =
        minikv::shard_route_preview_audit_closeout_archive_verification::format_archive_verification_json();
    assert_contains(
        archive_verification,
        "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification.v1\"");
    assert_contains(
        archive_verification,
        "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_contains(
        archive_verification,
        "\"archiveVerificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification\"");
    assert_contains(
        archive_verification,
        "\"sourceAuditCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(archive_verification, "\"sourceAuditCloseoutReleaseVersion\":\"v440\"");
    assert_contains(archive_verification, "\"sourceAuditCloseoutFixturePath\":\"fixtures/release/shard-readiness-v440.json\"");
    assert_contains(archive_verification, "\"sourceAuditCloseoutArchiveRootHint\":\"e/440/\"");
    assert_contains(archive_verification, "\"sourceAuditCloseoutPublishedStageCount\":20");
    assert_contains(archive_verification, "\"sourceAuditCloseoutPlannedStageCount\":20");
    assert_contains(archive_verification, "\"sourceAuditCloseoutChainComplete\":true");
    assert_contains(
        archive_verification,
        "\"sourceAuditCloseoutDigestMarker\":\"v440-route-preview-verification-report-archive-closeout-verification-audit-closeout-release-package-20-of-20-stages\"");
    assert_contains(
        archive_verification,
        "\"archiveVerificationStage\":\"route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-release-package\"");
    assert_contains(archive_verification, "\"archiveVerificationStageSequence\":20");
    assert_contains(archive_verification, "\"archiveVerificationReleaseVersion\":\"v460\"");
    assert_contains(archive_verification, "\"publishedStageCount\":20");
    assert_contains(archive_verification, "\"plannedStageCount\":20");
    assert_contains(archive_verification, "\"sourceFrozenReleaseVersion\":\"v459\"");
    assert_contains(archive_verification, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v459.json\"");
    assert_contains(archive_verification, "\"verifiedAuditCloseoutReleaseRangeStart\":\"v421\"");
    assert_contains(archive_verification, "\"verifiedAuditCloseoutReleaseRangeEnd\":\"v440\"");
    assert_contains(archive_verification, "\"verifiedAuditCloseoutReleaseCount\":20");
    assert_contains(archive_verification, "\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\"");
    assert_contains(archive_verification, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(archive_verification, "\"archiveVerificationCheckCount\":13");
    assert_contains(archive_verification, "\"archiveVerificationPassedCount\":13");
    assert_contains(archive_verification, "\"declaredArchiveVerificationOnly\":true");
    assert_contains(archive_verification, "\"archiveRootHintUsedForHumanReviewOnly\":true");
    assert_contains(archive_verification, "\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationCommandAvailable\":true");
    assert_contains(archive_verification, "\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationChainComplete\":true");
    assert_contains(archive_verification, "\"filesystemReadPerformed\":false");
    assert_contains(archive_verification, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(archive_verification, "\"readOnly\":true");
    assert_contains(archive_verification, "\"mutatesStore\":false");
    assert_contains(archive_verification, "\"touchesWal\":false");
    assert_contains(archive_verification, "\"activeRouterInstalled\":false");
    assert_contains(archive_verification, "\"writeRoutingAllowed\":false");
    assert_contains(archive_verification, "\"writeCommandsAllowed\":false");
    assert_contains(archive_verification, "\"loadRestoreCompactAllowed\":false");
    assert_contains(archive_verification, "\"startsMiniKvService\":false");
    assert_contains(archive_verification, "\"liveReadAllowed\":false");
    assert_contains(archive_verification, "\"executionAllowed\":false");
    assert_not_contains(archive_verification, "\"executionAllowed\":true");

    const std::string shard = minikv::shard_readiness::format_json();
    assert(occurrence_count(
               shard,
               "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArchiveVerification\":") ==
           1);
    assert_contains(shard, "\"releaseVersion\":\"v475\"");
    assert_contains(
        shard,
        "\"status\":\"route-preview-archive-maintenance-retention-audit-read-only\"");
    assert_contains(shard, "\"archiveVerificationReleaseVersion\":\"v460\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");
    assert(result.response == archive_verification);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");
    assert_contains(
        result.response,
        "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(
        result.response,
        "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
