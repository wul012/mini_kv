#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const std::string closeout =
        minikv::shard_route_preview_verification_report_archive_closeout::format_closeout_json();
    assert_contains(closeout, "\"contract\":\"shard-route-preview-verification-report-archive-closeout.v1\"");
    assert_contains(closeout, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(closeout, "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-chain\"");
    assert_contains(closeout, "\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(closeout, "\"sourceCloseoutCommand\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(closeout, "\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(closeout, "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(closeout, "\"sourcePreviewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(closeout, "\"sourceArchiveReleaseVersion\":\"v360\"");
    assert_contains(closeout, "\"sourceArchiveFixturePath\":\"fixtures/release/shard-readiness-v360.json\"");
    assert_contains(closeout, "\"sourceArchivePublishedStageCount\":20");
    assert_contains(closeout, "\"sourceArchivePlannedStageCount\":20");
    assert_contains(closeout, "\"sourceArchiveChainComplete\":true");
    assert_contains(closeout, "\"sourceArchiveDigestMarker\":\"v360-route-preview-verification-report-archive-closeout-audit-20-of-20-stages\"");
    assert_contains(closeout, "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-java-echo-audit\"");
    assert_contains(closeout, "\"closeoutStageSequence\":15");
    assert_contains(closeout, "\"closeoutReleaseVersion\":\"v375\"");
    assert_contains(closeout, "\"publishedStageCount\":15");
    assert_contains(closeout, "\"plannedStageCount\":20");
    assert_contains(closeout, "\"sourceFrozenReleaseVersion\":\"v374\"");
    assert_contains(closeout, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v374.json\"");
    assert_contains(closeout, "\"archivedArchiveReleaseRangeStart\":\"v341\"");
    assert_contains(closeout, "\"archivedArchiveReleaseRangeEnd\":\"v360\"");
    assert_contains(closeout, "\"archivedArchiveReleaseCount\":20");
    assert_contains(closeout, "\"archivedReportReleaseRangeStart\":\"v320\"");
    assert_contains(closeout, "\"archivedReportReleaseRangeEnd\":\"v339\"");
    assert_contains(closeout, "\"archivedReportCloseoutReleaseVersion\":\"v340\"");
    assert_contains(closeout, "\"archiveCloseoutCommandAvailable\":true");
    assert_contains(closeout, "\"archiveCloseoutChainComplete\":false");
    assert_contains(closeout, "\"filesystemReadPerformed\":false");
    assert_contains(closeout, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(closeout, "\"readOnly\":true");
    assert_contains(closeout, "\"mutatesStore\":false");
    assert_contains(closeout, "\"touchesWal\":false");
    assert_contains(closeout, "\"executesRoute\":false");
    assert_contains(closeout, "\"activeRouterInstalled\":false");
    assert_contains(closeout, "\"writeRoutingAllowed\":false");
    assert_contains(closeout, "\"executionAllowed\":false");
    assert_not_contains(closeout, "\"executionAllowed\":true");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
    assert(result.response == closeout);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
