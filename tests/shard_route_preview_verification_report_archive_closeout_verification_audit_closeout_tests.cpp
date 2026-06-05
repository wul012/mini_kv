#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"
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
    const std::string closeout =
        minikv::shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
            format_closeout_json();
    assert_contains(
        closeout,
        "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout.v1\"");
    assert_contains(closeout, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(
        closeout,
        "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit-closeout\"");
    assert_contains(closeout, "\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(closeout, "\"sourceAuditReleaseVersion\":\"v420\"");
    assert_contains(closeout, "\"sourceAuditFixturePath\":\"fixtures/release/shard-readiness-v420.json\"");
    assert_contains(closeout, "\"sourceAuditPublishedStageCount\":20");
    assert_contains(closeout, "\"sourceAuditPlannedStageCount\":20");
    assert_contains(closeout, "\"sourceAuditChainComplete\":true");
    assert_contains(
        closeout,
        "\"sourceAuditDigestMarker\":\"v420-route-preview-verification-report-archive-closeout-verification-audit-release-package-20-of-20-stages\"");
    assert_contains(
        closeout,
        "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-verification-audit-closeout-checkjson-contract\"");
    assert_contains(closeout, "\"closeoutStageSequence\":4");
    assert_contains(closeout, "\"closeoutReleaseVersion\":\"v424\"");
    assert_contains(closeout, "\"publishedStageCount\":4");
    assert_contains(closeout, "\"plannedStageCount\":20");
    assert_contains(closeout, "\"sourceFrozenReleaseVersion\":\"v423\"");
    assert_contains(closeout, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v423.json\"");
    assert_contains(closeout, "\"closedAuditReleaseRangeStart\":\"v401\"");
    assert_contains(closeout, "\"closedAuditReleaseRangeEnd\":\"v420\"");
    assert_contains(closeout, "\"closedAuditReleaseCount\":20");
    assert_contains(closeout, "\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\"");
    assert_contains(closeout, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(closeout, "\"closeoutCheckCount\":12");
    assert_contains(closeout, "\"closeoutPassedCount\":12");
    assert_contains(closeout, "\"archiveCloseoutVerificationAuditCloseoutCommandAvailable\":true");
    assert_contains(closeout, "\"archiveCloseoutVerificationAuditCloseoutChainComplete\":false");
    assert_contains(closeout, "\"filesystemReadPerformed\":false");
    assert_contains(closeout, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(closeout, "\"readOnly\":true");
    assert_contains(closeout, "\"mutatesStore\":false");
    assert_contains(closeout, "\"touchesWal\":false");
    assert_contains(closeout, "\"activeRouterInstalled\":false");
    assert_contains(closeout, "\"writeRoutingAllowed\":false");
    assert_contains(closeout, "\"writeCommandsAllowed\":false");
    assert_contains(closeout, "\"loadRestoreCompactAllowed\":false");
    assert_contains(closeout, "\"startsMiniKvService\":false");
    assert_contains(closeout, "\"liveReadAllowed\":false");
    assert_contains(closeout, "\"executionAllowed\":false");
    assert_not_contains(closeout, "\"executionAllowed\":true");

    const std::string shard = minikv::shard_readiness::format_json();
    assert(occurrence_count(shard, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseout\":") ==
           1);
    assert_contains(shard, "\"closeoutReleaseVersion\":\"v424\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");
    assert(result.response == closeout);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
