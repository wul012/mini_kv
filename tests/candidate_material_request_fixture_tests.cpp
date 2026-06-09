#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(955);
    assert_contains(source_fixture, "\"releaseVersion\":\"v955\"");
    assert_contains(source_fixture, "\"candidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(source_fixture, "\"candidateIntakePacketOnly\":true");
    assert(source_fixture.find("\"shardPreviewCandidateMaterialRequest\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v965\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateMaterialRequest\":{\"contract\":"
                                     "\"shard-route-preview-candidate-document-material-request.v1\"");
    assert_contains(current_fixture, "\"sourceCandidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(current_fixture, "\"sourceCandidateIntakePacketFixturePath\":\"fixtures/release/shard-readiness-v955.json\"");
    assert_contains(current_fixture, "\"candidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_contains(current_fixture, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(current_fixture, "\"reviewedMaterialPresent\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
}
