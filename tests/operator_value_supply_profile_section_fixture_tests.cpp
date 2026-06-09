#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1136; version <= 1160; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewOperatorValueSupplyProfileSection\"");
        assert_contains(fixture, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1135\"");
        assert_contains(fixture, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
        assert_contains(fixture, "\"sourceValueDraftReleaseVersion\":\"v585\"");
        assert_contains(fixture, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
        assert_contains(fixture, "\"operatorValueSupplyProfileSectionCount\":4");
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
    assert_contains(current, "\"releaseVersion\":\"v1160\"");
    assert_contains(current, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v1160\"");

    return 0;
}
