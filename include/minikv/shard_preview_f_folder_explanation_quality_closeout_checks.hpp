#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout_checks {

struct FFolderExplanationQualityCloseoutCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const FFolderExplanationQualityCloseoutCheck> f_folder_explanation_quality_closeout_checks();
int planned_f_folder_explanation_quality_closeout_check_count();
std::string format_f_folder_explanation_quality_closeout_checks_json(int published_check_count);

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout_checks
