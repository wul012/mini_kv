#include "minikv/shard_preview_production_live_capture_archive_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_production_live_capture_archive_non_participation;

    const auto json = evidence::format_production_live_capture_archive_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-production-live-capture-archive-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveCloseoutReleaseVersion\":\"Node v1656\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeArchiveModuleSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1195\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v1225\"");
    assert_contains(json, "\"plannedProductionLiveCaptureArchiveNonParticipationCheckCount\":30");
    assert_contains(json, "\"productionLiveCaptureArchiveNonParticipationOnly\":true");
    assert_contains(json, "\"capture-archive-endpoint-constants\"");
    assert_contains(json, "\"ci-build-stability\"");
    assert_contains(json, "\"nodeCaptureArchiveExecuted\":false");
    assert_contains(json, "\"nodeArchiveLoaderExecuted\":false");
    assert_contains(json, "\"miniKvStartsCaptureArchive\":false");
    assert_contains(json, "\"miniKvReadsArchiveEndpoint\":false");
    assert_contains(json, "\"archiveEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"archivePayloadImported\":false");
    assert_contains(json, "\"archiveFixtureImported\":false");
    assert_contains(json, "\"archiveRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":true");

    const auto digest = evidence::production_live_capture_archive_non_participation_digest_marker();
    assert(digest.find("v1225-route-preview-production-live-capture-archive-non-participation-closeout-summary") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 30);
    assert(evidence::production_live_capture_archive_non_participation_status() ==
           "route-preview-production-live-capture-archive-non-participation-closeout-summary-read-only");

    return 0;
}
