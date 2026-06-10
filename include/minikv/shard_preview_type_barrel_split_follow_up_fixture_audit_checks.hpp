#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_checks {

struct TypeBarrelSplitFollowUpFixtureAuditCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const TypeBarrelSplitFollowUpFixtureAuditCheck>
type_barrel_split_follow_up_fixture_audit_checks();
int planned_type_barrel_split_follow_up_fixture_audit_check_count();
std::string format_type_barrel_split_follow_up_fixture_audit_checks_json(int published_check_count);

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_checks
