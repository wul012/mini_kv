#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1086; version <= 1110; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewSignedApprovalArtifactDraftTextPackageProfileSection\"");
        assert_contains(fixture, "\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1085\"");
        assert_contains(fixture, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
        assert_contains(fixture, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
        assert_contains(fixture, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
        assert_contains(fixture, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
        assert_contains(fixture, "\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
        assert_contains(fixture, "\"nodeRendererExecuted\":false");
        assert_contains(fixture, "\"textPackageMaterialImported\":false");
        assert_contains(fixture, "\"secretValueRead\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1170\"");
    assert_contains(current, "\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"v1110\"");

    return 0;
}
