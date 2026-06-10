#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_production_live_capture_archive_verification_non_participation;

    const auto json = evidence::format_production_live_capture_archive_verification_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-preview-production-live-capture-archive-verification-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(
        json,
        "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationCloseoutReleaseVersion\":\"Node v1686\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeArchiveVerificationModuleSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1225\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":30");
    assert_contains(json, "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v1260\"");
    assert_contains(json, "\"plannedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":35");
    assert_contains(json, "\"productionLiveCaptureArchiveVerificationNonParticipationOnly\":true");
    assert_contains(json, "\"expected-archive-digest-recompute-builder\"");
    assert_contains(json, "\"release-evidence-review-consumer-validation\"");
    assert_contains(json, "\"github-actions-closeout\"");
    assert_contains(json, "\"nodeCaptureArchiveVerificationExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationLoaderExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"nodeVerificationEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeVerificationFixtureImportedByMiniKv\":false");
    assert_contains(json, "\"archiveDigestRecomputedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsCaptureArchiveVerification\":false");
    assert_contains(json, "\"miniKvReadsVerificationEndpoint\":false");
    assert_contains(json, "\"verificationEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"verificationPayloadImported\":false");
    assert_contains(json, "\"verificationFixtureImported\":false");
    assert_contains(json, "\"archiveVerificationRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":true");

    const auto digest = evidence::production_live_capture_archive_verification_non_participation_digest_marker();
    assert(digest.find("v1260-route-preview-production-live-capture-archive-verification-non-participation-closeout-summary") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 35);
    assert(evidence::production_live_capture_archive_verification_non_participation_status() ==
           "route-preview-production-live-capture-archive-verification-non-participation-closeout-summary-read-only");

    return 0;
}
