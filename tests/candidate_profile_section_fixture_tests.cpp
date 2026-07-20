#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(1010);
    assert_contains(source_fixture, "\"releaseVersion\":\"v1010\"");
    assert_contains(source_fixture, "\"candidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_contains(source_fixture, "\"candidateMaterialSubmissionPrecheckIntegrityOnly\":true");
    assert(source_fixture.find("\"shardPreviewCandidateProfileSection\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v1035\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateProfileSection\":{\"contract\":"
                                     "\"shard-route-preview-candidate-document-profile-section-renderer-split.v1\"");
    assert_contains(current_fixture, "\"sourceNodeProfileSectionReleaseVersion\":\"Node v1481\"");
    assert_contains(current_fixture, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_contains(current_fixture, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath\":\"fixtures/"
                                     "release/shard-readiness-v1010.json\"");
    assert_contains(current_fixture, "\"candidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(current_fixture, "\"candidateProfileSectionCount\":5");
    assert_contains(current_fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(current_fixture, "\"nodeRendererExecuted\":false");
    assert_contains(current_fixture, "\"miniKvConsumesProfileMaterial\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"materialSubmissionOpened\":false");
    assert_contains(current_fixture, "\"materialPayloadImported\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
    return 0;
}
