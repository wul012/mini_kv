#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_submission_precheck_validators {

struct SubmissionPrecheckValidator {
    int sequence;
    std::string_view validator_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const SubmissionPrecheckValidator> candidate_submission_precheck_validators();
int planned_candidate_submission_precheck_validator_count();
std::string format_candidate_submission_precheck_validators_json(int published_validator_count);

} // namespace minikv::shard_preview_candidate_submission_precheck_validators
