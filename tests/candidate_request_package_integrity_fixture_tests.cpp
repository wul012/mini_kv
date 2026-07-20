#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    const auto source_fixture = read_shard_readiness_fixture(895);
    assert_contains(source_fixture, "\"releaseVersion\":\"v895\"");
    assert_contains(source_fixture, "\"candidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(source_fixture, "\"candidateDocumentRequestPackageCloseoutOnly\":true");
    assert_contains(source_fixture, "\"requestPackageReadyForFutureRealDocumentIntake\":true");
    assert(source_fixture.find("\"shardPreviewCandidateRequestPackageIntegrity\"") == std::string::npos);

    const auto current_fixture = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current_fixture, "\"releaseVersion\":\"v920\"");
    assert_contains(current_fixture, "\"shardPreviewCandidateRequestPackageIntegrity\":{\"contract\":"
                                     "\"shard-route-preview-candidate-request-package-fixture-integrity.v1\"");
    assert_contains(current_fixture, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(
        current_fixture,
        "\"sourceCandidateDocumentRequestPackageFixturePath\":\"fixtures/release/shard-readiness-v895.json\"");
    assert_contains(current_fixture, "\"candidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(current_fixture, "\"additionalRequestEchoCreated\":false");
    assert_contains(current_fixture, "\"documentIntakeOpened\":false");
    assert_contains(current_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(current_fixture, "\"touchesWal\":false");
    assert_contains(current_fixture, "\"executionAllowed\":false");
    assert(source_fixture != current_fixture);
    return 0;
}
