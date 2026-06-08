#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_request_package_integrity_checks {

struct IntegrityCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const IntegrityCheck> candidate_request_package_integrity_checks();
int planned_candidate_request_package_integrity_check_count();
std::string format_candidate_request_package_integrity_checks_json(int published_check_count);

} // namespace minikv::shard_preview_candidate_request_package_integrity_checks
