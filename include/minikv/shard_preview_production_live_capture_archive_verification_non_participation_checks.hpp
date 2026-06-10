#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_checks {

struct ProductionLiveCaptureArchiveVerificationNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const ProductionLiveCaptureArchiveVerificationNonParticipationCheck>
production_live_capture_archive_verification_non_participation_checks();
int planned_production_live_capture_archive_verification_non_participation_check_count();
std::string format_production_live_capture_archive_verification_non_participation_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_checks
