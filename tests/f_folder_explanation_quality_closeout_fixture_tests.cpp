#include "test_support.hpp"

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::release_fixture_path;

    const auto current = read_fixture_text(release_fixture_path("shard-readiness.json"));
    const auto versioned = read_fixture_text(release_fixture_path("shard-readiness-v1607.json"));

    assert_contains(current, "\"releaseVersion\":\"v1607\"");
    assert_contains(versioned, "\"releaseVersion\":\"v1607\"");
    assert_contains(current, "\"status\":\"f-folder-explanation-quality-closeout-single-large-version-read-only\"");
    assert_contains(versioned, "\"status\":\"f-folder-explanation-quality-closeout-single-large-version-read-only\"");
    assert_contains(current, "\"shardPreviewFFolderExplanationQualityCloseout\":{\"contract\":\"shard-f-folder-explanation-quality-closeout.v1\"");
    assert_contains(versioned, "\"command\":\"SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON\"");
    assert_contains(versioned, "\"fFolderExplanationQualityCloseoutReleaseVersion\":\"v1607\"");
    assert_contains(versioned, "\"sourceExternalArtifactCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(versioned, "\"chineseWritingRequired\":true");
    assert_contains(versioned, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_contains(versioned, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_contains(versioned, "\"fillerPaddingAllowed\":false");
    assert_contains(versioned, "\"singleVersionLargeScope\":true");
    assert_contains(versioned, "\"multiVersionFragmentationAllowed\":false");
    assert_contains(versioned, "\"plannedFFolderExplanationQualityRuleCount\":13");
    assert_contains(versioned, "\"completedFFolderExplanationQualityCloseoutCheckCount\":19");
    assert_contains(versioned, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
    assert_contains(versioned, "\"scansNodeFolders\":false");
    assert_contains(versioned, "\"importsNodeQualityRoute\":false");
    assert_contains(versioned, "\"createsPictureDirsWithoutEvidence\":false");
    assert_contains(versioned, "\"executionAllowed\":false");

    return 0;
}
