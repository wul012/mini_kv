#include "minikv/shard_preview_f_folder_explanation_quality_closeout_validation.hpp"

#include "test_support.hpp"

#include <string>

namespace {

struct ValidationCase {
    int source_stage_count = 5;
    bool source_complete = true;
    bool node_v2103_observed = true;
    bool long_chinese_required = true;
    bool chinese_writing_required = true;
    int minimum_walkthrough_chinese_character_count = 3000;
    bool insufficient_word_count_expands_version_workload = true;
    bool filler_padding_allowed = false;
    bool short_rejected = true;
    bool human_machine_split = true;
    bool screenshot_dirs_require_real_screenshots = true;
    bool keep_picture_dirs_absent = true;
    bool single_large_version = true;
    bool multi_version_fragmentation = false;
    bool rewrites_historical_walkthroughs = false;
    bool scans_node_folders = false;
    bool imports_node_quality_route = false;
    bool creates_picture_dirs_without_evidence = false;
    bool starts_node = false;
    bool starts_java = false;
    bool starts_mini_kv = false;
    bool active_router = false;
    bool write_routing = false;
    bool mutates_store = false;
    bool touches_wal = false;
    bool execution_allowed = false;
    int planned_rules = 13;
    int published_rules = 13;
    int planned_checks = 19;
    int completed_checks = 19;
};

std::string validation_json(const ValidationCase& validation = {}) {
    return minikv::shard_preview_f_folder_explanation_quality_closeout_validation::
        format_f_folder_explanation_quality_closeout_validation_json(
            validation.source_stage_count,
            validation.source_complete,
            validation.node_v2103_observed,
            validation.long_chinese_required,
            validation.chinese_writing_required,
            validation.minimum_walkthrough_chinese_character_count,
            validation.insufficient_word_count_expands_version_workload,
            validation.filler_padding_allowed,
            validation.short_rejected,
            validation.human_machine_split,
            validation.screenshot_dirs_require_real_screenshots,
            validation.keep_picture_dirs_absent,
            validation.single_large_version,
            validation.multi_version_fragmentation,
            validation.rewrites_historical_walkthroughs,
            validation.scans_node_folders,
            validation.imports_node_quality_route,
            validation.creates_picture_dirs_without_evidence,
            validation.starts_node,
            validation.starts_java,
            validation.starts_mini_kv,
            validation.active_router,
            validation.write_routing,
            validation.mutates_store,
            validation.touches_wal,
            validation.execution_allowed,
            validation.planned_rules,
            validation.published_rules,
            validation.planned_checks,
            validation.completed_checks);
}

void assert_passes(const std::string& json) {
    minikv::test_support::assert_contains(json, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
}

void assert_fails(const std::string& json) {
    minikv::test_support::assert_contains(json, "\"fFolderExplanationQualityCloseoutValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = validation_json();
    assert_passes(valid);
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"chineseWritingRequired\":true");
    assert_contains(valid, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_contains(valid, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_contains(valid, "\"fillerPaddingAllowed\":false");
    assert_contains(valid, "\"explanationPolicyLocked\":true");
    assert_contains(valid, "\"archivePolicyLocked\":true");
    assert_contains(valid, "\"granularityPolicyLocked\":true");
    assert_contains(valid, "\"crossProjectBoundaryClosed\":true");
    assert_contains(valid, "\"authorityBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");

    auto broken = ValidationCase{};
    broken.source_stage_count = 4;
    assert_fails(validation_json(broken));

    broken = {};
    broken.source_complete = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.node_v2103_observed = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.long_chinese_required = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.chinese_writing_required = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.minimum_walkthrough_chinese_character_count = 2999;
    assert_fails(validation_json(broken));

    broken = {};
    broken.insufficient_word_count_expands_version_workload = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.filler_padding_allowed = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.short_rejected = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.human_machine_split = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.screenshot_dirs_require_real_screenshots = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.keep_picture_dirs_absent = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.single_large_version = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.multi_version_fragmentation = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.rewrites_historical_walkthroughs = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.scans_node_folders = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.imports_node_quality_route = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.creates_picture_dirs_without_evidence = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_node = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_java = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_mini_kv = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.active_router = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.write_routing = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.mutates_store = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.touches_wal = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.execution_allowed = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.planned_rules = 12;
    assert_fails(validation_json(broken));

    broken = {};
    broken.published_rules = 12;
    assert_fails(validation_json(broken));

    broken = {};
    broken.planned_checks = 18;
    assert_fails(validation_json(broken));

    broken = {};
    broken.completed_checks = 18;
    assert_fails(validation_json(broken));

    return 0;
}
