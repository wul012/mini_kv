#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(1035);
    assert_contains(source_fixture, "\"releaseVersion\":\"v1035\"");
    assert_contains(source_fixture, "\"candidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(source_fixture, "\"candidateProfileSectionOnly\":true");
    assert(source_fixture.find("\"shardPreviewSignedApprovalArtifactDraftProfileSection\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v1060\"");
    assert_contains(current_fixture,
                    "\"shardPreviewSignedApprovalArtifactDraftProfileSection\":{\"contract\":"
                    "\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\"");
    assert_contains(current_fixture,
                    "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_contains(current_fixture, "\"sourceCandidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(current_fixture, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(current_fixture, "\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(current_fixture, "\"signedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(current_fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(current_fixture, "\"nodeRendererExecuted\":false");
    assert_contains(current_fixture, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_contains(current_fixture, "\"draftArtifactCreated\":false");
    assert_contains(current_fixture, "\"signedApprovalCreated\":false");
    assert_contains(current_fixture, "\"runtimePayloadImported\":false");
    assert_contains(current_fixture, "\"materialIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
    return 0;
}
