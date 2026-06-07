#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace requirements =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements;

    assert(requirements::planned_signed_approval_capture_artifact_draft_authoring_readiness_requirement_count() == 25);
    const std::string first =
        requirements::format_signed_approval_capture_artifact_draft_authoring_readiness_requirements_json(1);
    minikv::test_support::assert_contains(
        first,
        "\"requirementCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST\"");
    minikv::test_support::assert_contains(first, "\"materializesDraftText\":false");
    minikv::test_support::assert_contains(first, "\"acceptsSignatureMaterial\":false");
    minikv::test_support::assert_contains(first, "\"importsOperatorValue\":false");
    minikv::test_support::assert_not_contains(first, "CLOSEOUT");

    const std::string all =
        requirements::format_signed_approval_capture_artifact_draft_authoring_readiness_requirements_json(99);
    minikv::test_support::assert_contains(
        all,
        "\"requirementCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CLOSEOUT\"");
    return 0;
}
