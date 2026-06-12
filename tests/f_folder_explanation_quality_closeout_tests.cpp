#include "minikv/command.hpp"
#include "minikv/shard_preview_f_folder_explanation_quality_closeout.hpp"
#include "minikv/store.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace closeout = minikv::shard_preview_f_folder_explanation_quality_closeout;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;
    using minikv::test_support::assert_response_contains;

    const auto json = closeout::format_f_folder_explanation_quality_closeout_json();
    assert_contains(json, "\"contract\":\"shard-f-folder-explanation-quality-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v2103-f-folder-explanation-expansion-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceExternalArtifactCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"fFolderExplanationQualityCloseoutReleaseVersion\":\"v1607\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"nodeV2103CloseoutObserved\":true");
    assert_contains(json, "\"longChineseExplanationsRequired\":true");
    assert_contains(json, "\"chineseWritingRequired\":true");
    assert_contains(json, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_contains(json, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_contains(json, "\"fillerPaddingAllowed\":false");
    assert_contains(json, "\"shortExplanationsRejected\":true");
    assert_contains(json, "\"singleVersionLargeScope\":true");
    assert_contains(json, "\"multiVersionFragmentationAllowed\":false");
    assert_contains(json, "\"internalQualityTrackCount\":5");
    assert_contains(json, "\"plannedFFolderExplanationQualityRuleCount\":13");
    assert_contains(json, "\"completedFFolderExplanationQualityCloseoutCheckCount\":19");
    assert_contains(json, "\"archiveDirectory\":\"f/1607/\"");
    assert_contains(json, "\"explanationDirectory\":\"f/1607/解释/\"");
    assert_contains(json, "\"pictureDirectory\":\"f/1607/图片/\"");
    assert_contains(json, "\"pictureDirectoryCreatedOnlyWithRealScreenshot\":true");
    assert_contains(json, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
    assert_contains(json, "\"scansNodeFolders\":false");
    assert_contains(json, "\"importsNodeQualityRoute\":false");
    assert_contains(json, "\"createsPictureDirsWithoutEvidence\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_not_contains(json, "\"executionAllowed\":true");
    assert_not_contains(json, "\"touchesWal\":true");

    assert(closeout::f_folder_explanation_quality_closeout_status() ==
           "f-folder-explanation-quality-closeout-single-large-version-read-only");
    assert(closeout::published_quality_rule_count() == 13);
    assert(closeout::f_folder_explanation_quality_closeout_digest_marker().find(
               "v1607-f-folder-explanation-quality-closeout-single-large-version-min-3000-chinese-chars-13-rules-19-checks-read-only") !=
           std::string::npos);

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    auto result = processor.execute("SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");
    assert_response_contains(result, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");

    result = processor.execute("SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON extra");
    assert_response_contains(result, "ERR usage: SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");

    return 0;
}
