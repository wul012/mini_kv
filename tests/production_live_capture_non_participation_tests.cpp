#include "minikv/shard_preview_production_live_capture_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_production_live_capture_non_participation;

    const auto json = evidence::format_production_live_capture_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-production-live-capture-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureCloseoutReleaseVersion\":\"Node v1631\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeModuleSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1170\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":10");
    assert_contains(json, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v1195\"");
    assert_contains(json, "\"plannedProductionLiveCaptureNonParticipationCheckCount\":25");
    assert_contains(json, "\"productionLiveCaptureNonParticipationOnly\":true");
    assert_contains(json, "\"profile-types\"");
    assert_contains(json, "\"endpoint-constants\"");
    assert_contains(json, "\"nodeLiveCaptureExecuted\":false");
    assert_contains(json, "\"miniKvStartsLiveCapture\":false");
    assert_contains(json, "\"miniKvReadsRawEndpoint\":false");
    assert_contains(json, "\"liveEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"productionLiveReadAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"productionLiveCaptureNonParticipationValidationPassed\":true");

    const auto digest = evidence::production_live_capture_non_participation_digest_marker();
    assert(digest.find("v1195-route-preview-production-live-capture-non-participation-closeout-summary") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 25);
    assert(evidence::production_live_capture_non_participation_status() ==
           "route-preview-production-live-capture-non-participation-closeout-summary-read-only");

    return 0;
}
