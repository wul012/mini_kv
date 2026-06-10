#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_production_live_capture_release_evidence_review_non_participation;

    const auto json = evidence::format_production_live_capture_release_evidence_review_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-preview-production-live-capture-release-evidence-review-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_contains(
        json,
        "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewCloseoutReleaseVersion\":\"Node v1721\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeReleaseEvidenceReviewModuleSplitOnly\":true");
    assert_contains(json,
                    "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1260\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":35");
    assert_contains(json, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_contains(json, "\"plannedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":35");
    assert_contains(json, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationOnly\":true");
    assert_contains(json, "\"release-review-profile-field-guide-message-contracts\"");
    assert_contains(json, "\"archive-verification-upstream-consumer-validation\"");
    assert_contains(json, "\"status-live-probe-route-wiring\"");
    assert_contains(json, "\"github-actions-closeout\"");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewLoaderExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseReviewFixtureImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseReviewDigestGeneratedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsReleaseEvidenceReview\":false");
    assert_contains(json, "\"miniKvReadsReleaseReviewEndpoint\":false");
    assert_contains(json, "\"releaseReviewEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"releaseReviewPayloadImported\":false");
    assert_contains(json, "\"releaseReviewFixtureImported\":false");
    assert_contains(json, "\"releaseEvidenceReviewRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":true");

    const auto digest = evidence::production_live_capture_release_evidence_review_non_participation_digest_marker();
    assert(digest.find("v1295-route-preview-production-live-capture-release-evidence-review-non-participation-closeout-summary") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 35);
    assert(evidence::production_live_capture_release_evidence_review_non_participation_status() ==
           "route-preview-production-live-capture-release-evidence-review-non-participation-closeout-summary-read-only");

    return 0;
}
