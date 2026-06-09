#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_material_request_checks {

struct MaterialAcceptanceCheck {
    int sequence;
    std::string_view check_code;
    std::string_view verifies;
    std::string_view blocked_until_present;
};

std::span<const MaterialAcceptanceCheck> candidate_material_acceptance_checks();
int planned_candidate_material_acceptance_check_count();
std::string format_candidate_material_acceptance_checks_json(int published_check_count);

} // namespace minikv::shard_preview_candidate_material_request_checks
