#include "minikv/shard_preview_production_live_capture_archive_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_production_live_capture_archive_non_participation_validation;

    const auto passed =
        validation::format_production_live_capture_archive_non_participation_validation_json(25, true, false, false,
                                                                                             false, 30, 30, 30, 30);
    assert_contains(passed, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"archiveCaptureClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvCaptureArchiveStarted\":false");
    assert_contains(passed, "\"archiveEndpointRead\":false");
    assert_contains(passed, "\"archivePayloadImported\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_production_live_capture_archive_non_participation_validation_json(24, true, false, false,
                                                                                             false, 30, 30, 30, 30);
    assert_contains(failed_source, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_archive =
        validation::format_production_live_capture_archive_non_participation_validation_json(25, true, false, true,
                                                                                             false, 30, 30, 30, 30);
    assert_contains(failed_archive, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":false");
    assert_contains(failed_archive, "\"archiveCaptureClosed\":false");

    const auto failed_count =
        validation::format_production_live_capture_archive_non_participation_validation_json(25, true, false, false,
                                                                                             false, 30, 29, 30, 30);
    assert_contains(failed_count, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
