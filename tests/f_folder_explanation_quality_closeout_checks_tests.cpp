#include "minikv/shard_preview_f_folder_explanation_quality_closeout_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    namespace checks = minikv::shard_preview_f_folder_explanation_quality_closeout_checks;
    using minikv::test_support::assert_contains;

    const auto records = checks::f_folder_explanation_quality_closeout_checks();
    assert(records.size() == 19);
    assert(records.front().check_code == std::string{"source-v1606-closeout-read"});
    assert(records.back().check_code == std::string{"no-execution-authority"});
    assert(checks::planned_f_folder_explanation_quality_closeout_check_count() == 19);

    const auto json = checks::format_f_folder_explanation_quality_closeout_checks_json(19);
    assert_contains(json, "\"checkCode\":\"single-version-scope-locked\"");
    assert_contains(json, "\"checkCode\":\"chinese-writing-required\"");
    assert_contains(json, "\"checkCode\":\"minimum-3000-chinese-character-walkthrough\"");
    assert_contains(json, "\"checkCode\":\"no-filler-padding\"");
    assert_contains(json, "\"checkCode\":\"workload-expands-if-explanation-short\"");
    assert_contains(json, "\"checkCode\":\"no-node-folder-scan\"");
    assert_contains(json, "\"explanationQualityEvidence\":true");
    assert_contains(json, "\"chineseWritingRequired\":true");
    assert_contains(json, "\"longChineseExplanationsRequired\":true");
    assert_contains(json, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_contains(json, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_contains(json, "\"fillerPaddingAllowed\":false");
    assert_contains(json, "\"shortExplanationsRejected\":true");
    assert_contains(json, "\"singleVersionLargeScope\":true");
    assert_contains(json, "\"multiVersionFragmentationAllowed\":false");
    assert_contains(json, "\"scansNodeFolders\":false");
    assert_contains(json, "\"importsNodeQualityRoute\":false");
    assert_contains(json, "\"rewritesHistoricalWalkthroughs\":false");
    assert_contains(json, "\"createsPictureDirsWithoutEvidence\":false");
    assert_contains(json, "\"startsNodeServices\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto partial = checks::format_f_folder_explanation_quality_closeout_checks_json(7);
    assert_contains(partial, "\"checkCode\":\"workload-expands-if-explanation-short\"");
    assert(partial.find("no-execution-authority") == std::string::npos);

    bool threw = false;
    try {
        (void)checks::format_f_folder_explanation_quality_closeout_checks_json(20);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
