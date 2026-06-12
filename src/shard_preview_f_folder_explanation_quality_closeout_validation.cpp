#include "minikv/shard_preview_f_folder_explanation_quality_closeout_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_f_folder_explanation_quality_closeout_validation_json(
    int source_external_artifact_closeout_stage_count,
    bool source_external_artifact_closeout_complete,
    bool node_v2103_closeout_observed,
    bool long_chinese_explanations_required,
    bool chinese_writing_required,
    int minimum_walkthrough_chinese_character_count,
    bool insufficient_word_count_expands_version_workload,
    bool filler_padding_allowed,
    bool short_explanations_rejected,
    bool human_explanation_separate_from_machine_evidence,
    bool screenshot_dirs_require_real_screenshots,
    bool keep_picture_dirs_absent_without_screenshots,
    bool single_version_large_scope,
    bool multi_version_fragmentation_allowed,
    bool rewrites_historical_walkthroughs,
    bool scans_node_folders,
    bool imports_node_quality_route,
    bool creates_picture_dirs_without_evidence,
    bool starts_node_services,
    bool starts_java_service,
    bool starts_mini_kv_service,
    bool active_router_installed,
    bool write_routing_allowed,
    bool mutates_store,
    bool touches_wal,
    bool execution_allowed,
    int planned_rule_count,
    int published_rule_count,
    int planned_check_count,
    int completed_check_count) {
    const bool source_locked =
        source_external_artifact_closeout_stage_count == 5 && source_external_artifact_closeout_complete;
    const bool explanation_policy_locked = node_v2103_closeout_observed && long_chinese_explanations_required &&
                                           chinese_writing_required &&
                                           minimum_walkthrough_chinese_character_count >= 3000 &&
                                           insufficient_word_count_expands_version_workload &&
                                           !filler_padding_allowed && short_explanations_rejected &&
                                           human_explanation_separate_from_machine_evidence;
    const bool archive_policy_locked =
        screenshot_dirs_require_real_screenshots && keep_picture_dirs_absent_without_screenshots;
    const bool granularity_policy_locked = single_version_large_scope && !multi_version_fragmentation_allowed;
    const bool cross_project_boundary_closed =
        !rewrites_historical_walkthroughs && !scans_node_folders && !imports_node_quality_route &&
        !creates_picture_dirs_without_evidence;
    const bool authority_boundary_closed =
        !starts_node_services && !starts_java_service && !starts_mini_kv_service && !active_router_installed &&
        !write_routing_allowed && !mutates_store && !touches_wal && !execution_allowed;
    const bool counts_aligned =
        planned_rule_count == 13 && published_rule_count == 13 && planned_check_count == 19 &&
        completed_check_count == 19;
    const bool validation_passed = source_locked && explanation_policy_locked && archive_policy_locked &&
                                   granularity_policy_locked && cross_project_boundary_closed &&
                                   authority_boundary_closed && counts_aligned;

    return "{\"fFolderExplanationQualityCloseoutValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceExternalArtifactCloseoutStageCount\":" +
           std::to_string(source_external_artifact_closeout_stage_count) +
           ",\"sourceExternalArtifactCloseoutComplete\":" +
           json_bool(source_external_artifact_closeout_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeV2103CloseoutObserved\":" + json_bool(node_v2103_closeout_observed) +
           ",\"longChineseExplanationsRequired\":" + json_bool(long_chinese_explanations_required) +
           ",\"chineseWritingRequired\":" + json_bool(chinese_writing_required) +
           ",\"minimumWalkthroughChineseCharacterCount\":" +
           std::to_string(minimum_walkthrough_chinese_character_count) +
           ",\"insufficientWordCountExpandsVersionWorkload\":" +
           json_bool(insufficient_word_count_expands_version_workload) +
           ",\"fillerPaddingAllowed\":" + json_bool(filler_padding_allowed) +
           ",\"shortExplanationsRejected\":" + json_bool(short_explanations_rejected) +
           ",\"humanExplanationSeparateFromMachineEvidence\":" +
           json_bool(human_explanation_separate_from_machine_evidence) +
           ",\"explanationPolicyLocked\":" + json_bool(explanation_policy_locked) +
           ",\"screenshotDirsRequireRealScreenshots\":" +
           json_bool(screenshot_dirs_require_real_screenshots) +
           ",\"keepPictureDirsAbsentWithoutScreenshots\":" +
           json_bool(keep_picture_dirs_absent_without_screenshots) +
           ",\"archivePolicyLocked\":" + json_bool(archive_policy_locked) +
           ",\"singleVersionLargeScope\":" + json_bool(single_version_large_scope) +
           ",\"multiVersionFragmentationAllowed\":" + json_bool(multi_version_fragmentation_allowed) +
           ",\"granularityPolicyLocked\":" + json_bool(granularity_policy_locked) +
           ",\"rewritesHistoricalWalkthroughs\":" + json_bool(rewrites_historical_walkthroughs) +
           ",\"scansNodeFolders\":" + json_bool(scans_node_folders) +
           ",\"importsNodeQualityRoute\":" + json_bool(imports_node_quality_route) +
           ",\"createsPictureDirsWithoutEvidence\":" + json_bool(creates_picture_dirs_without_evidence) +
           ",\"crossProjectBoundaryClosed\":" + json_bool(cross_project_boundary_closed) +
           ",\"startsNodeServices\":" + json_bool(starts_node_services) +
           ",\"startsJavaService\":" + json_bool(starts_java_service) +
           ",\"startsMiniKvService\":" + json_bool(starts_mini_kv_service) +
           ",\"activeRouterInstalled\":" + json_bool(active_router_installed) +
           ",\"writeRoutingAllowed\":" + json_bool(write_routing_allowed) +
           ",\"mutatesStore\":" + json_bool(mutates_store) +
           ",\"touchesWal\":" + json_bool(touches_wal) +
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           ",\"authorityBoundaryClosed\":" + json_bool(authority_boundary_closed) +
           ",\"plannedFFolderExplanationQualityRuleCount\":" + std::to_string(planned_rule_count) +
           ",\"publishedFFolderExplanationQualityRuleCount\":" + std::to_string(published_rule_count) +
           ",\"plannedFFolderExplanationQualityCloseoutCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedFFolderExplanationQualityCloseoutCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout_validation
