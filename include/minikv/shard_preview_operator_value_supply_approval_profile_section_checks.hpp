#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_operator_value_supply_approval_profile_section_checks {

struct OperatorValueSupplyApprovalProfileSectionCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const OperatorValueSupplyApprovalProfileSectionCheck>
operator_value_supply_approval_profile_section_checks();
int planned_operator_value_supply_approval_profile_section_check_count();
std::string format_operator_value_supply_approval_profile_section_checks_json(int published_check_count);

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section_checks
