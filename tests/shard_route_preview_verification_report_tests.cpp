#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification_report.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const std::string report =
        minikv::shard_route_preview_verification_report::format_report_json("orderops:alpha");
    assert_contains(report, "\"contract\":\"shard-route-preview-verification-report.v1\"");
    assert_contains(report, "\"command\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(report, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(report, "\"previewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(report, "\"reportMode\":\"read-only-route-preview-verification-package\"");
    assert_contains(report, "\"key\":\"orderops:alpha\"");
    assert_contains(report, "\"slot\":5");
    assert_contains(report, "\"slotCount\":16");
    assert_contains(report, "\"pinnedFixtureSample\":true");
    assert_contains(report, "\"verificationPassed\":true");
    assert_contains(report, "\"checkCount\":9");
    assert_contains(report, "\"passedCount\":9");
    assert_contains(report, "\"failedCount\":0");
    assert_contains(report, "\"reportStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_contains(report, "\"reportStageSequence\":20");
    assert_contains(report, "\"reportReleaseVersion\":\"v339\"");
    assert_contains(report, "\"sourceFrozenReleaseVersion\":\"v338\"");
    assert_contains(report, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v338.json\"");
    assert_contains(report, "\"readOnly\":true");
    assert_contains(report, "\"mutatesStore\":false");
    assert_contains(report, "\"touchesWal\":false");
    assert_contains(report, "\"executesRoute\":false");
    assert_contains(report, "\"activeRouterInstalled\":false");
    assert_contains(report, "\"writeRoutingAllowed\":false");
    assert_contains(report, "\"executionAllowed\":false");

    const std::string rollout = minikv::shard_route_preview_verification_report::format_rollout_json();
    assert_contains(rollout, "\"rolloutMode\":\"read-only-shard-route-preview-verification-report-rollout\"");
    assert_contains(rollout, "\"sourceFrozenReleaseVersion\":\"v338\"");
    assert_contains(rollout, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v338.json\"");
    assert_contains(rollout, "\"rolloutStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_contains(rollout, "\"rolloutStageSequence\":20");
    assert_contains(rollout, "\"rolloutReleaseVersion\":\"v339\"");
    assert_contains(rollout, "\"publishedStageCount\":20");
    assert_contains(rollout, "\"reportCommandAvailable\":true");
    assert_contains(rollout, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(rollout, "\"previewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(rollout, "\"commands\":[\"SHARDROUTEVERIFYREPORTJSON\"]");
    assert_contains(rollout, "\"activeRouterInstalled\":false");
    assert_contains(rollout, "\"writeRoutingAllowed\":false");
    assert_contains(rollout, "\"executionAllowed\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTJSON");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTJSON key");

    result = processor.execute("SHARDROUTEVERIFYREPORTJSON orderops:alpha extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTJSON key");

    result = processor.execute("SHARDROUTEVERIFYREPORTJSON orderops:alpha");
    assert(result.response == report);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTJSON orderops:alpha");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"key\":\"orderops:alpha\"");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTJSON orderops:alpha");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"not a write command\"");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
