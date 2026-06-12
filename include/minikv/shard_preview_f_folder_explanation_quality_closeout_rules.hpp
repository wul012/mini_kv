#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout_rules {

struct FFolderExplanationQualityRule {
    int sequence;
    std::string_view rule_code;
    std::string_view applies_to;
    std::string_view required_evidence;
};

std::span<const FFolderExplanationQualityRule> f_folder_explanation_quality_rules();
int planned_f_folder_explanation_quality_rule_count();
std::string format_f_folder_explanation_quality_rules_json(int published_rule_count);

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout_rules
