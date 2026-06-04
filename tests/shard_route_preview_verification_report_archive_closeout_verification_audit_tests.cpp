#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"
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
    const std::string audit =
        minikv::shard_route_preview_verification_report_archive_closeout_verification_audit::format_audit_json();
    assert_contains(audit, "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit.v1\"");
    assert_contains(audit, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(audit, "\"auditMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit\"");
    assert_contains(audit, "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(audit, "\"sourceVerificationReleaseVersion\":\"v400\"");
    assert_contains(audit, "\"sourceVerificationFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_contains(audit, "\"sourceVerificationPublishedStageCount\":20");
    assert_contains(audit, "\"sourceVerificationPlannedStageCount\":20");
    assert_contains(audit, "\"sourceVerificationChainComplete\":true");
    assert_contains(audit,
                    "\"sourceVerificationDigestMarker\":\"v400-route-preview-verification-report-archive-closeout-verification-release-package-20-of-20-stages\"");
    assert_contains(audit, "\"auditStage\":\"route-preview-verification-report-archive-closeout-verification-audit-release-package\"");
    assert_contains(audit, "\"auditStageSequence\":20");
    assert_contains(audit, "\"auditReleaseVersion\":\"v420\"");
    assert_contains(audit, "\"publishedStageCount\":20");
    assert_contains(audit, "\"plannedStageCount\":20");
    assert_contains(audit, "\"sourceFrozenReleaseVersion\":\"v419\"");
    assert_contains(audit, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v419.json\"");
    assert_contains(audit, "\"auditedVerificationReleaseRangeStart\":\"v381\"");
    assert_contains(audit, "\"auditedVerificationReleaseRangeEnd\":\"v400\"");
    assert_contains(audit, "\"auditedVerificationReleaseCount\":20");
    assert_contains(audit, "\"sourceVerificationVersionedFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_contains(audit, "\"auditCheckCount\":12");
    assert_contains(audit, "\"auditPassedCount\":12");
    assert_contains(audit, "\"singleVerificationSectionExpected\":true");
    assert_contains(audit, "\"singleAuditSectionExpected\":true");
    assert_contains(audit, "\"duplicateVerificationSectionRemovedInCurrent\":true");
    assert_contains(audit, "\"archiveCloseoutVerificationAuditCommandAvailable\":true");
    assert_contains(audit, "\"archiveCloseoutVerificationAuditChainComplete\":true");
    assert_contains(audit, "\"filesystemReadPerformed\":false");
    assert_contains(audit, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(audit, "\"readOnly\":true");
    assert_contains(audit, "\"mutatesStore\":false");
    assert_contains(audit, "\"touchesWal\":false");
    assert_contains(audit, "\"activeRouterInstalled\":false");
    assert_contains(audit, "\"writeRoutingAllowed\":false");
    assert_contains(audit, "\"writeCommandsAllowed\":false");
    assert_contains(audit, "\"loadRestoreCompactAllowed\":false");
    assert_contains(audit, "\"startsMiniKvService\":false");
    assert_contains(audit, "\"liveReadAllowed\":false");
    assert_contains(audit, "\"executionAllowed\":false");
    assert_not_contains(audit, "\"executionAllowed\":true");

    const std::string shard = minikv::shard_readiness::format_json();
    assert(occurrence_count(shard, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerification\":") == 1);
    assert(occurrence_count(shard, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAudit\":") == 1);
    assert_contains(shard, "\"auditReleaseVersion\":\"v420\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");
    assert(result.response == audit);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}