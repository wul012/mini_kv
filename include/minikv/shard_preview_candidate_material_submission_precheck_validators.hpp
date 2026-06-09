#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_material_submission_precheck_validators {

struct MaterialSubmissionPrecheckValidator {
    int sequence;
    std::string_view validator_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const MaterialSubmissionPrecheckValidator> candidate_material_submission_precheck_validators();
int planned_candidate_material_submission_precheck_validator_count();
std::string format_candidate_material_submission_precheck_validators_json(int published_validator_count);

} // namespace minikv::shard_preview_candidate_material_submission_precheck_validators
