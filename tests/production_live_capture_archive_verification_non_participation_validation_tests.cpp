#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_production_live_capture_archive_verification_non_participation_validation;

    const auto passed =
        validation::format_production_live_capture_archive_verification_non_participation_validation_json(
            30, true, false, false, false, 35, 35, 35, 35);
    assert_contains(passed, "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"archiveVerificationClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvCaptureArchiveVerificationStarted\":false");
    assert_contains(passed, "\"verificationEndpointRead\":false");
    assert_contains(passed, "\"verificationPayloadImported\":false");
    assert_contains(passed, "\"verificationFixtureImported\":false");
    assert_contains(passed, "\"archiveDigestRecomputedByMiniKv\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_production_live_capture_archive_verification_non_participation_validation_json(
            29, true, false, false, false, 35, 35, 35, 35);
    assert_contains(failed_source,
                    "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_verification =
        validation::format_production_live_capture_archive_verification_non_participation_validation_json(
            30, true, false, true, false, 35, 35, 35, 35);
    assert_contains(failed_verification,
                    "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":false");
    assert_contains(failed_verification, "\"archiveVerificationClosed\":false");

    const auto failed_count =
        validation::format_production_live_capture_archive_verification_non_participation_validation_json(
            30, true, false, false, false, 35, 34, 35, 35);
    assert_contains(failed_count,
                    "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
