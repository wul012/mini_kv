#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification_report_archive.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const std::string archive = minikv::shard_route_preview_verification_report_archive::format_archive_json();
    assert_contains(archive, "\"contract\":\"shard-route-preview-verification-report-archive.v1\"");
    assert_contains(archive, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(archive, "\"archiveMode\":\"read-only-route-preview-verification-report-archive-chain\"");
    assert_contains(archive, "\"sourceCloseoutCommand\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(archive, "\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(archive, "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(archive, "\"sourcePreviewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(archive, "\"sourceCloseoutReleaseVersion\":\"v340\"");
    assert_contains(archive, "\"sourceCloseoutFixturePath\":\"fixtures/release/shard-readiness-v340.json\"");
    assert_contains(archive, "\"archiveStage\":\"route-preview-verification-report-archive-java-echo-audit\"");
    assert_contains(archive, "\"archiveStageSequence\":12");
    assert_contains(archive, "\"archiveReleaseVersion\":\"v352\"");
    assert_contains(archive, "\"publishedStageCount\":12");
    assert_contains(archive, "\"plannedStageCount\":20");
    assert_contains(archive, "\"sourceFrozenReleaseVersion\":\"v351\"");
    assert_contains(archive, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v351.json\"");
    assert_contains(archive, "\"archivedReportReleaseRangeStart\":\"v320\"");
    assert_contains(archive, "\"archivedReportReleaseRangeEnd\":\"v339\"");
    assert_contains(archive, "\"archivedCloseoutReleaseVersion\":\"v340\"");
    assert_contains(archive, "\"archivedReportReleaseCount\":20");
    assert_contains(archive, "\"archivedCloseoutReleaseCount\":1");
    assert_contains(archive, "\"archiveManifestPublished\":true");
    assert_contains(archive, "\"archiveVerificationPlannedReleaseVersion\":\"v342\"");
    assert_contains(archive, "\"archiveVerificationPublished\":true");
    assert_contains(archive, "\"archiveChainComplete\":false");
    assert_contains(archive, "\"filesystemReadPerformed\":false");
    assert_contains(archive, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(archive, "\"readOnly\":true");
    assert_contains(archive, "\"mutatesStore\":false");
    assert_contains(archive, "\"touchesWal\":false");
    assert_contains(archive, "\"executesRoute\":false");
    assert_contains(archive, "\"activeRouterInstalled\":false");
    assert_contains(archive, "\"writeRoutingAllowed\":false");
    assert_contains(archive, "\"executionAllowed\":false");
    assert_not_contains(archive, "\"executionAllowed\":true");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVEJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVEJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVEJSON");
    assert(result.response == archive);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVEJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
