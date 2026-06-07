#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation.hpp"
#include "test_support.hpp"

#include <string>

int main() {
    namespace validation =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation;

    const std::string ok =
        validation::format_signed_approval_capture_artifact_draft_authoring_readiness_validation_json(25, 25, 25, true, 1);
    minikv::test_support::assert_contains(ok, "\"authoringReadinessValidationPassed\":true");
    minikv::test_support::assert_contains(ok, "\"sourceArtifactPreflightChainComplete\":true");
    minikv::test_support::assert_contains(ok, "\"readyForDraftAuthoringReview\":true");
    minikv::test_support::assert_contains(ok, "\"readyForSignedApprovalArtifactDraft\":false");
    minikv::test_support::assert_contains(ok, "\"draftTextGenerationAllowed\":false");
    minikv::test_support::assert_contains(ok, "\"artifactMaterializationAllowed\":false");
    minikv::test_support::assert_contains(ok, "\"executionAllowed\":false");

    const std::string blocked =
        validation::format_signed_approval_capture_artifact_draft_authoring_readiness_validation_json(25, 24, 25, true, 1);
    minikv::test_support::assert_contains(blocked, "\"authoringReadinessValidationPassed\":false");
    return 0;
}
