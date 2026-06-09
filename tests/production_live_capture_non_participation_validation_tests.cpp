#include "minikv/shard_preview_production_live_capture_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_production_live_capture_non_participation_validation;

    const auto passed =
        validation::format_production_live_capture_non_participation_validation_json(10, true, false, false, false,
                                                                                     25, 25, 25, 25);
    assert_contains(passed, "\"productionLiveCaptureNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"liveCaptureClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvLiveCaptureStarted\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_production_live_capture_non_participation_validation_json(9, true, false, false, false, 25,
                                                                                     25, 25, 25);
    assert_contains(failed_source, "\"productionLiveCaptureNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_live_capture =
        validation::format_production_live_capture_non_participation_validation_json(10, true, false, true, false, 25,
                                                                                     25, 25, 25);
    assert_contains(failed_live_capture, "\"productionLiveCaptureNonParticipationValidationPassed\":false");
    assert_contains(failed_live_capture, "\"liveCaptureClosed\":false");

    const auto failed_count =
        validation::format_production_live_capture_non_participation_validation_json(10, true, false, false, false,
                                                                                     25, 24, 25, 25);
    assert_contains(failed_count, "\"productionLiveCaptureNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
