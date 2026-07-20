#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(930);
    assert_contains(source_fixture, "\"releaseVersion\":\"v930\"");
    assert_contains(source_fixture, "\"candidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(source_fixture, "\"candidateSubmissionPrecheckOnly\":true");
    assert(source_fixture.find("\"shardPreviewCandidateIntakePacket\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v955\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateIntakePacket\":{\"contract\":"
                                     "\"shard-route-preview-candidate-document-intake-packet.v1\"");
    assert_contains(current_fixture, "\"sourceCandidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(current_fixture,
                    "\"sourceCandidateSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v930.json\"");
    assert_contains(current_fixture, "\"candidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(current_fixture, "\"additionalIntakeEchoCreated\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
    return 0;
}
