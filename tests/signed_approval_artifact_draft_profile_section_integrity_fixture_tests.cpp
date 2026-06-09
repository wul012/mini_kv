#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(1060);
    assert_contains(source_fixture, "\"releaseVersion\":\"v1060\"");
    assert_contains(source_fixture, "\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(source_fixture, "\"signedApprovalArtifactDraftProfileSectionOnly\":true");
    assert(source_fixture.find("\"shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity\"") ==
           std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v1085\"");
    assert_contains(current_fixture,
                    "\"shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity\":{\"contract\":"
                    "\"shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1\"");
    assert_contains(current_fixture, "\"sourceProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(current_fixture, "\"sourceProfileSectionFixturePath\":\"fixtures/release/shard-readiness-v1060.json\"");
    assert_contains(current_fixture, "\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_contains(current_fixture, "\"signedApprovalArtifactDraftProfileSectionIntegrityOnly\":true");
    assert_contains(current_fixture, "\"sourceProfileSectionFrozen\":true");
    assert_contains(current_fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(current_fixture, "\"nodeRendererExecuted\":false");
    assert_contains(current_fixture, "\"nodeCatalogImported\":false");
    assert_contains(current_fixture, "\"draftArtifactCreated\":false");
    assert_contains(current_fixture, "\"signedApprovalCreated\":false");
    assert_contains(current_fixture, "\"materialIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"loadRestoreCompactAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
}
