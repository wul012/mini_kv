#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_request_package_checks {

struct CandidateAcceptanceCheck {
    int sequence;
    std::string_view check_code;
    std::string_view verifies;
    std::string_view failure_state;
};

std::span<const CandidateAcceptanceCheck> candidate_document_acceptance_checks();
int planned_candidate_document_acceptance_check_count();
std::string format_candidate_document_acceptance_checks_json(int completed_check_count);

} // namespace minikv::shard_preview_candidate_request_package_checks
