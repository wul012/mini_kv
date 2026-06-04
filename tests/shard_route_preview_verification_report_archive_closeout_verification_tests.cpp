#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const std::string verification =
        minikv::shard_route_preview_verification_report_archive_closeout_verification::format_verification_json();
    assert_contains(verification,
                    "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification.v1\"");
    assert_contains(verification, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(verification,
                    "\"verificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification\"");
    assert_contains(verification, "\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(verification, "\"sourceArchiveCloseoutReleaseVersion\":\"v380\"");
    assert_contains(verification, "\"sourceArchiveCloseoutFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_contains(verification, "\"sourceArchiveCloseoutPublishedStageCount\":20");
    assert_contains(verification, "\"sourceArchiveCloseoutPlannedStageCount\":20");
    assert_contains(verification, "\"sourceArchiveCloseoutChainComplete\":true");
    assert_contains(verification,
                    "\"sourceArchiveCloseoutDigestMarker\":\"v380-route-preview-verification-report-archive-closeout-release-package-20-of-20-stages\"");
    assert_contains(verification, "\"verificationStage\":\"route-preview-verification-report-archive-closeout-verification-checkjson-contract\"");
    assert_contains(verification, "\"verificationStageSequence\":4");
    assert_contains(verification, "\"verificationReleaseVersion\":\"v384\"");
    assert_contains(verification, "\"publishedStageCount\":4");
    assert_contains(verification, "\"plannedStageCount\":20");
    assert_contains(verification, "\"sourceFrozenReleaseVersion\":\"v383\"");
    assert_contains(verification, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v383.json\"");
    assert_contains(verification, "\"verifiedCloseoutReleaseRangeStart\":\"v361\"");
    assert_contains(verification, "\"verifiedCloseoutReleaseRangeEnd\":\"v380\"");
    assert_contains(verification, "\"verifiedCloseoutReleaseCount\":20");
    assert_contains(verification, "\"sourceCloseoutVersionedFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_contains(verification, "\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(verification, "\"verificationCheckCount\":11");
    assert_contains(verification, "\"verificationPassedCount\":11");
    assert_contains(verification, "\"sourceCommandAvailable\":true");
    assert_contains(verification, "\"sourceChainComplete\":true");
    assert_contains(verification, "\"sourcePublishedStageCountMatchesPlan\":true");
    assert_contains(verification, "\"sourceDigestMarkerPinned\":true");
    assert_contains(verification, "\"versionedFixtureExpected\":true");
    assert_contains(verification, "\"archiveCloseoutVerificationCommandAvailable\":true");
    assert_contains(verification, "\"archiveCloseoutVerificationChainComplete\":false");
    assert_contains(verification, "\"filesystemReadPerformed\":false");
    assert_contains(verification, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(verification, "\"readOnly\":true");
    assert_contains(verification, "\"mutatesStore\":false");
    assert_contains(verification, "\"touchesWal\":false");
    assert_contains(verification, "\"executesRoute\":false");
    assert_contains(verification, "\"activeRouterInstalled\":false");
    assert_contains(verification, "\"writeRoutingAllowed\":false");
    assert_contains(verification, "\"writeCommandsAllowed\":false");
    assert_contains(verification, "\"loadRestoreCompactAllowed\":false");
    assert_contains(verification, "\"startsMiniKvService\":false");
    assert_contains(verification, "\"liveReadAllowed\":false");
    assert_contains(verification, "\"executionAllowed\":false");
    assert_not_contains(verification, "\"executionAllowed\":true");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");
    assert(result.response == verification);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}