#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::release_fixture_path;

    for (int version = 1546; version <= 1565; ++version) {
        const auto fixture =
            read_fixture_text(release_fixture_path("shard-readiness-v" + std::to_string(version) + ".json"));
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(
            fixture,
            "\"status\":\"route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout-read-only\"");
        assert_contains(fixture, "\"commandCatalogQuality\":{\"releaseRangeStart\":\"v1546\"");
        assert_contains(fixture, "\"releaseRangeEnd\":\"v1565\"");
        assert_contains(fixture, "\"commandCount\":88");
        assert_contains(fixture, "\"dispatchCatalogSplit\":true");
        assert_contains(fixture, "\"contractCatalogDeduplicated\":true");
        assert_contains(fixture, "\"helpTextGeneratedFromCatalog\":true");
        assert_contains(fixture, "\"completionCandidatesGeneratedFromCatalog\":true");
        assert_contains(fixture, "\"keyCompletionPolicyGeneratedFromCatalog\":true");
        assert_contains(fixture, "\"runtimeCommandBehaviorChanged\":false");
        assert_contains(fixture, "\"storageBehaviorChanged\":false");
        assert_contains(fixture, "\"walBehaviorChanged\":false");
        assert_contains(fixture, "\"routerBehaviorChanged\":false");
        assert_contains(fixture, "\"nodeOrJavaFilesModified\":false");
    }

    const auto current = read_fixture_text(release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1591\"");
    assert_contains(
        current,
        "\"status\":\"code-walkthrough-quality-gate-non-participation-clean-ci-closeout-read-only\"");
    assert_contains(current, "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");
    assert_contains(current, "\"releaseRangeEnd\":\"v1591\"");
    assert_contains(current, "\"commandCount\":90");

    return 0;
}
