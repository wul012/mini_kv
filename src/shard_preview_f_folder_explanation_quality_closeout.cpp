#include "minikv/shard_preview_f_folder_explanation_quality_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout_checks.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout_rules.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout_validation.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout {
namespace {

namespace checks = shard_preview_f_folder_explanation_quality_closeout_checks;
namespace rules = shard_preview_f_folder_explanation_quality_closeout_rules;
namespace source_closeout = shard_preview_production_shard_execution_external_artifact_dry_run_closeout;
namespace validation = shard_preview_f_folder_explanation_quality_closeout_validation;

constexpr std::string_view command_name = "SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v2103-f-folder-explanation-expansion-closeout-roadmap.md";
constexpr std::string_view release_version = "v1607";
constexpr std::string_view source_frozen_release_version = "v1606";
constexpr std::string_view source_frozen_fixture_path = "fixtures/release/shard-readiness-v1606.json";
constexpr bool node_v2103_closeout_observed = true;
constexpr bool long_chinese_explanations_required = true;
constexpr bool chinese_writing_required = true;
constexpr int minimum_walkthrough_chinese_character_count = 3000;
constexpr bool insufficient_word_count_expands_version_workload = true;
constexpr bool filler_padding_allowed = false;
constexpr bool short_explanations_rejected = true;
constexpr bool human_explanation_separate_from_machine_evidence = true;
constexpr bool screenshot_dirs_require_real_screenshots = true;
constexpr bool keep_picture_dirs_absent_without_screenshots = true;
constexpr bool single_version_large_scope = true;
constexpr bool multi_version_fragmentation_allowed = false;
constexpr bool rewrites_historical_walkthroughs = false;
constexpr bool scans_node_folders = false;
constexpr bool imports_node_quality_route = false;
constexpr bool creates_picture_dirs_without_evidence = false;
constexpr bool starts_node_services = false;
constexpr bool starts_java_service = false;
constexpr bool starts_mini_kv_service = false;
constexpr bool active_router_installed = false;
constexpr bool write_routing_allowed = false;
constexpr bool mutates_store = false;
constexpr bool touches_wal = false;
constexpr bool execution_allowed = false;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

int source_external_artifact_closeout_stage_count() {
    return source_closeout::published_stage_count();
}

bool source_external_artifact_closeout_complete() {
    return source_external_artifact_closeout_stage_count() == 5;
}

int completed_check_count() {
    return checks::planned_f_folder_explanation_quality_closeout_check_count();
}

std::string format_archive_policy_json() {
    return "{\"archiveRoot\":\"f\""
           ",\"archiveDirectory\":\"f/1607/\""
           ",\"explanationDirectory\":\"f/1607/解释/\""
           ",\"pictureDirectory\":\"f/1607/图片/\""
           ",\"pictureDirectoryCreatedOnlyWithRealScreenshot\":true"
           ",\"keepPictureDirsAbsentWithoutScreenshots\":" +
           json_bool(keep_picture_dirs_absent_without_screenshots) +
           ",\"humanExplanationSeparateFromMachineEvidence\":" +
           json_bool(human_explanation_separate_from_machine_evidence) +
           ",\"combinedArchiveFolderAllowed\":false}";
}

std::string format_granularity_policy_json() {
    return "{\"singleVersionLargeScope\":" + json_bool(single_version_large_scope) +
           ",\"multiVersionFragmentationAllowed\":" + json_bool(multi_version_fragmentation_allowed) +
           ",\"internalQualityTrackCount\":5"
           ",\"internalQualityTracks\":" +
           json_string_array({"source freeze and Node plan intake",
                              "Chinese long explanation rule",
                              "f-root archive layout rule",
                              "picture directory evidence rule",
                              "read-only execution boundary rule"}) +
           ",\"versionNumberIncrement\":\"one-version-only\"}";
}

std::string format_boundary_policy_json() {
    return "{\"rewritesHistoricalWalkthroughs\":" + json_bool(rewrites_historical_walkthroughs) +
           ",\"scansNodeFolders\":" + json_bool(scans_node_folders) +
           ",\"importsNodeQualityRoute\":" + json_bool(imports_node_quality_route) +
           ",\"createsPictureDirsWithoutEvidence\":" + json_bool(creates_picture_dirs_without_evidence) +
           ",\"startsNodeServices\":" + json_bool(starts_node_services) +
           ",\"startsJavaService\":" + json_bool(starts_java_service) +
           ",\"startsMiniKvService\":" + json_bool(starts_mini_kv_service) +
           ",\"activeRouterInstalled\":" + json_bool(active_router_installed) +
           ",\"writeRoutingAllowed\":" + json_bool(write_routing_allowed) +
           ",\"mutatesStore\":" + json_bool(mutates_store) +
           ",\"touchesWal\":" + json_bool(touches_wal) +
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           "}";
}

} // namespace

std::string f_folder_explanation_quality_closeout_status() {
    return "f-folder-explanation-quality-closeout-single-large-version-read-only";
}

std::string format_f_folder_explanation_quality_closeout_json() {
    const int source_stage_count = source_external_artifact_closeout_stage_count();
    const bool source_complete = source_external_artifact_closeout_complete();
    const int planned_rule_count = rules::planned_f_folder_explanation_quality_rule_count();
    const int published_rule_count_value = published_quality_rule_count();
    const int planned_check_count = checks::planned_f_folder_explanation_quality_closeout_check_count();
    const int completed_checks = completed_check_count();

    return "{\"contract\":\"shard-f-folder-explanation-quality-closeout.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" +
           json_string(command_name) +
           ",\"fFolderExplanationQualityCloseoutMode\":\"single-large-version-quality-gate\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceExternalArtifactCloseoutCommand\":\"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON\""
           ",\"sourceExternalArtifactCloseoutContract\":\"shard-production-shard-execution-external-artifact-dry-run-closeout.v1\""
           ",\"sourceExternalArtifactCloseoutReleaseVersion\":" + json_string(source_frozen_release_version) +
           ",\"sourceExternalArtifactCloseoutFixturePath\":" + json_string(source_frozen_fixture_path) +
           ",\"sourceExternalArtifactCloseoutPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceExternalArtifactCloseoutComplete\":" + json_bool(source_complete) +
           ",\"sourceExternalArtifactCloseoutStatus\":" +
           json_string(source_closeout::production_shard_execution_external_artifact_dry_run_closeout_status()) +
           ",\"sourceExternalArtifactCloseoutDigestMarker\":" +
           json_string(source_closeout::production_shard_execution_external_artifact_dry_run_closeout_digest_marker()) +
           ",\"fFolderExplanationQualityCloseoutReleaseVersion\":" + json_string(release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(source_frozen_fixture_path) +
           ",\"nodeV2103CloseoutObserved\":" + json_bool(node_v2103_closeout_observed) +
           ",\"longChineseExplanationsRequired\":" + json_bool(long_chinese_explanations_required) +
           ",\"chineseWritingRequired\":" + json_bool(chinese_writing_required) +
           ",\"minimumWalkthroughChineseCharacterCount\":" +
           std::to_string(minimum_walkthrough_chinese_character_count) +
           ",\"insufficientWordCountExpandsVersionWorkload\":" +
           json_bool(insufficient_word_count_expands_version_workload) +
           ",\"fillerPaddingAllowed\":" + json_bool(filler_padding_allowed) +
           ",\"shortExplanationsRejected\":" + json_bool(short_explanations_rejected) +
           ",\"plannedFFolderExplanationQualityRuleCount\":" + std::to_string(planned_rule_count) +
           ",\"publishedFFolderExplanationQualityRuleCount\":" + std::to_string(published_rule_count_value) +
           ",\"plannedFFolderExplanationQualityCloseoutCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedFFolderExplanationQualityCloseoutCheckCount\":" + std::to_string(completed_checks) +
           ",\"granularityPolicy\":" + format_granularity_policy_json() +
           ",\"archivePolicy\":" + format_archive_policy_json() +
           ",\"boundaryPolicy\":" + format_boundary_policy_json() +
           ",\"qualityRules\":" +
           rules::format_f_folder_explanation_quality_rules_json(published_rule_count_value) +
           ",\"qualityChecks\":" +
           checks::format_f_folder_explanation_quality_closeout_checks_json(completed_checks) +
           ",\"validation\":" +
           validation::format_f_folder_explanation_quality_closeout_validation_json(
               source_stage_count,
               source_complete,
               node_v2103_closeout_observed,
               long_chinese_explanations_required,
               chinese_writing_required,
               minimum_walkthrough_chinese_character_count,
               insufficient_word_count_expands_version_workload,
               filler_padding_allowed,
               short_explanations_rejected,
               human_explanation_separate_from_machine_evidence,
               screenshot_dirs_require_real_screenshots,
               keep_picture_dirs_absent_without_screenshots,
               single_version_large_scope,
               multi_version_fragmentation_allowed,
               rewrites_historical_walkthroughs,
               scans_node_folders,
               imports_node_quality_route,
               creates_picture_dirs_without_evidence,
               starts_node_services,
               starts_java_service,
               starts_mini_kv_service,
               active_router_installed,
               write_routing_allowed,
               mutates_store,
               touches_wal,
               execution_allowed,
               planned_rule_count,
               published_rule_count_value,
               planned_check_count,
               completed_checks) +
           ",\"diagnostics\":" +
           json_string_array({"v1607 is one substantial version with five internal quality tracks",
                              "Chinese long explanations are required for non-trivial walkthrough and archive notes",
                              "A substantial version keeps at least one 3000-Chinese-character walkthrough without filler padding",
                              "picture directories are only created when real screenshot evidence exists",
                              "mini-kv records Node v2103 as planning context without scanning Node archives",
                              "storage, WAL, router, service startup, and execution authority remain closed"}) +
           ",\"singleVersionLargeScope\":" + json_bool(single_version_large_scope) +
           ",\"multiVersionFragmentationAllowed\":" + json_bool(multi_version_fragmentation_allowed) +
           ",\"humanExplanationSeparateFromMachineEvidence\":" +
           json_bool(human_explanation_separate_from_machine_evidence) +
           ",\"chineseWritingRequired\":" + json_bool(chinese_writing_required) +
           ",\"minimumWalkthroughChineseCharacterCount\":" +
           std::to_string(minimum_walkthrough_chinese_character_count) +
           ",\"insufficientWordCountExpandsVersionWorkload\":" +
           json_bool(insufficient_word_count_expands_version_workload) +
           ",\"fillerPaddingAllowed\":" + json_bool(filler_padding_allowed) +
           ",\"pictureDirsRequireRealScreenshots\":" +
           json_bool(screenshot_dirs_require_real_screenshots) +
           ",\"keepPictureDirsAbsentWithoutScreenshots\":" +
           json_bool(keep_picture_dirs_absent_without_screenshots) +
           ",\"rewritesHistoricalWalkthroughs\":" + json_bool(rewrites_historical_walkthroughs) +
           ",\"scansNodeFolders\":" + json_bool(scans_node_folders) +
           ",\"importsNodeQualityRoute\":" + json_bool(imports_node_quality_route) +
           ",\"createsPictureDirsWithoutEvidence\":" + json_bool(creates_picture_dirs_without_evidence) +
           ",\"startsNodeServices\":" + json_bool(starts_node_services) +
           ",\"startsJavaService\":" + json_bool(starts_java_service) +
           ",\"startsMiniKvService\":" + json_bool(starts_mini_kv_service) +
           ",\"activeRouterInstalled\":" + json_bool(active_router_installed) +
           ",\"writeRoutingAllowed\":" + json_bool(write_routing_allowed) +
           ",\"mutatesStore\":" + json_bool(mutates_store) +
           ",\"touchesWal\":" + json_bool(touches_wal) +
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           ",\"readOnly\":true}";
}

std::string f_folder_explanation_quality_closeout_digest_marker() {
    return std::string{release_version} +
           "-f-folder-explanation-quality-closeout-single-large-version-min-3000-chinese-chars-" +
           std::to_string(published_quality_rule_count()) + "-rules-" +
           std::to_string(completed_check_count()) + "-checks-read-only";
}

int published_quality_rule_count() {
    return rules::planned_f_folder_explanation_quality_rule_count();
}

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout
