#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(920);
    assert_contains(source_fixture, "\"releaseVersion\":\"v920\"");
    assert_contains(source_fixture, "\"candidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(source_fixture, "\"candidateRequestPackageIntegrityOnly\":true");
    assert(source_fixture.find("\"shardPreviewCandidateSubmissionPrecheck\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v930\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateSubmissionPrecheck\":{\"contract\":"
                                     "\"shard-route-preview-candidate-document-submission-precheck.v1\"");
    assert_contains(current_fixture, "\"sourceCandidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(current_fixture, "\"sourceCandidateRequestPackageIntegrityFixturePath\":\"fixtures/release/shard-readiness-v920.json\"");
    assert_contains(current_fixture, "\"candidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(current_fixture, "\"additionalSubmissionPrecheckEchoCreated\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
}
