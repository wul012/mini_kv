#include "minikv/shard_preview_f_folder_explanation_quality_closeout_rules.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    namespace rules = minikv::shard_preview_f_folder_explanation_quality_closeout_rules;
    using minikv::test_support::assert_contains;

    const auto records = rules::f_folder_explanation_quality_rules();
    assert(records.size() == 13);
    assert(records.front().sequence == 1);
    assert(records.front().rule_code == std::string{"source-v1606-closeout-frozen"});
    assert(records.back().sequence == 13);
    assert(records.back().rule_code == std::string{"future-short-explanation-fails"});
    assert(rules::planned_f_folder_explanation_quality_rule_count() == 13);

    const auto json = rules::format_f_folder_explanation_quality_rules_json(13);
    assert_contains(json, "\"ruleCode\":\"chinese-walkthrough-writing-required\"");
    assert_contains(json, "\"ruleCode\":\"minimum-3000-chinese-character-walkthrough\"");
    assert_contains(json, "\"ruleCode\":\"insufficient-word-count-expands-workload\"");
    assert_contains(json, "\"ruleCode\":\"filler-padding-forbidden\"");
    assert_contains(json, "\"ruleCode\":\"single-version-large-scope\"");
    assert_contains(json, "\"ruleCode\":\"picture-dir-requires-real-screenshot\"");
    assert_contains(json, "\"mutatesArchive\":false");
    assert_contains(json, "\"scansNodeFolders\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto partial = rules::format_f_folder_explanation_quality_rules_json(6);
    assert_contains(partial, "\"ruleCode\":\"single-version-large-scope\"");
    assert(partial.find("future-short-explanation-fails") == std::string::npos);

    bool threw = false;
    try {
        (void)rules::format_f_folder_explanation_quality_rules_json(14);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
