#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification_report_closeout.hpp"
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
        minikv::shard_route_preview_verification_report_closeout::format_closeout_json();
    assert_contains(closeout, "\"contract\":\"shard-route-preview-verification-report-closeout.v1\"");
    assert_contains(closeout, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(closeout, "\"closeoutMode\":\"read-only-route-preview-verification-report-closeout-summary\"");
    assert_contains(closeout, "\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(closeout, "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(closeout, "\"sourcePreviewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(closeout, "\"closedReleaseRangeStart\":\"v320\"");
    assert_contains(closeout, "\"closedReleaseRangeEnd\":\"v339\"");
    assert_contains(closeout, "\"closedVersionCount\":20");
    assert_contains(closeout, "\"reportStageCount\":20");
    assert_contains(closeout, "\"latestReportReleaseVersion\":\"v339\"");
    assert_contains(closeout, "\"latestReportStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_contains(closeout, "\"versionedCurrentFixturePath\":\"fixtures/release/shard-readiness-v339.json\"");
    assert_contains(closeout, "\"allReportStagesPublished\":true");
    assert_contains(closeout, "\"stageCatalogClosed\":true");
    assert_contains(closeout, "\"archiveManifestPlannedReleaseVersion\":\"v341\"");
    assert_contains(closeout, "\"archiveVerificationPlannedReleaseVersion\":\"v342\"");
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

    auto result = processor.execute("SHARDROUTEVERIFYREPORTCLOSEOUTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTCLOSEOUTJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTCLOSEOUTJSON");
    assert(result.response == closeout);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTCLOSEOUTJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTCLOSEOUTJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
