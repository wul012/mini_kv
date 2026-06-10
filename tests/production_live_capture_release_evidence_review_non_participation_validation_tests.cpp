#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_validation;

    const auto passed =
        validation::format_production_live_capture_release_evidence_review_non_participation_validation_json(
            35, true, false, false, false, 35, 35, 35, 35);
    assert_contains(passed, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"releaseEvidenceReviewClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvReleaseEvidenceReviewStarted\":false");
    assert_contains(passed, "\"releaseReviewEndpointRead\":false");
    assert_contains(passed, "\"releaseReviewPayloadImported\":false");
    assert_contains(passed, "\"releaseReviewFixtureImported\":false");
    assert_contains(passed, "\"nodeReleaseReviewDigestGeneratedByMiniKv\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_production_live_capture_release_evidence_review_non_participation_validation_json(
            34, true, false, false, false, 35, 35, 35, 35);
    assert_contains(failed_source,
                    "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_verification =
        validation::format_production_live_capture_release_evidence_review_non_participation_validation_json(
            35, true, false, true, false, 35, 35, 35, 35);
    assert_contains(failed_verification,
                    "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":false");
    assert_contains(failed_verification, "\"releaseEvidenceReviewClosed\":false");

    const auto failed_count =
        validation::format_production_live_capture_release_evidence_review_non_participation_validation_json(
            35, true, false, false, false, 35, 34, 35, 35);
    assert_contains(failed_count,
                    "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
