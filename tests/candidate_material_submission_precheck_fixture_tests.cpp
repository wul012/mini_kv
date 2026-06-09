#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(975);
    assert_contains(source_fixture, "\"releaseVersion\":\"v975\"");
    assert_contains(source_fixture, "\"candidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_contains(source_fixture, "\"candidateMaterialRequestIntegrityOnly\":true");
    assert(source_fixture.find("\"shardPreviewCandidateMaterialSubmissionPrecheck\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v1035\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateMaterialSubmissionPrecheck\":{\"contract\":"
                                     "\"shard-route-preview-candidate-document-material-submission-precheck.v1\"");
    assert_contains(current_fixture, "\"sourceCandidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_contains(current_fixture, "\"sourceCandidateMaterialRequestIntegrityFixturePath\":\"fixtures/release/shard-readiness-v975.json\"");
    assert_contains(current_fixture, "\"candidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_contains(current_fixture, "\"nodeStopConditionRespected\":true");
    assert_contains(current_fixture, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(current_fixture, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(current_fixture, "\"reviewedMaterialPresent\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"materialPayloadImported\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
}
