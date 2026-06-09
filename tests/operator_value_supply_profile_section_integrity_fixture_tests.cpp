#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1161; version <= 1170; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewOperatorValueSupplyProfileSectionIntegrity\"");
        assert_contains(fixture, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceProfileSectionReleaseVersion\":\"v1160\"");
        assert_contains(fixture, "\"sourceProfileSectionPublishedStageCount\":25");
        assert_contains(fixture, "\"sourceProfileSectionComplete\":true");
        assert_contains(fixture, "\"operatorValueSupplyProfileSectionCount\":4");
        assert_contains(fixture, "\"sourceNodeProductionLiveProbeReadOnlyWindowReadinessPacketReleaseVersion\":\"Node v1606\"");
        assert_contains(fixture, "\"nodeRendererExecuted\":false");
        assert_contains(fixture, "\"operatorValueImported\":false");
        assert_contains(fixture, "\"freshSiblingEvidenceImported\":false");
        assert_contains(fixture, "\"syntheticEvidenceAccepted\":false");
        assert_contains(fixture, "\"secretValueRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1195\"");
    assert_contains(current, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v1160\"");
    assert_contains(current, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v1170\"");

    return 0;
}
