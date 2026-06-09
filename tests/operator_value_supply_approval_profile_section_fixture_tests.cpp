#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1111; version <= 1135; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewOperatorValueSupplyApprovalProfileSection\"");
        assert_contains(fixture, "\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1110\"");
        assert_contains(fixture, "\"sourceApprovalPacketDraftReleaseVersion\":\"v660\"");
        assert_contains(fixture, "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
        assert_contains(fixture, "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
        assert_contains(fixture, "\"sourceSignedApprovalCaptureArtifactPreflightReleaseVersion\":\"v735\"");
        assert_contains(fixture, "\"operatorValueSupplyApprovalProfileSectionCount\":5");
        assert_contains(fixture, "\"nodeRendererExecuted\":false");
        assert_contains(fixture, "\"approvalMaterialImported\":false");
        assert_contains(fixture, "\"signedApprovalCaptured\":false");
        assert_contains(fixture, "\"secretValueRead\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1135\"");
    assert_contains(current, "\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":\"v1135\"");

    return 0;
}
