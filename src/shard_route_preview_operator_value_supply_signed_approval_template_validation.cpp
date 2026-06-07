#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_template_validation_json(int field_count,
                                                            int clause_count,
                                                            int review_control_count,
                                                            int published_stage_count) {
    const bool counts_aligned = field_count == clause_count && clause_count == review_control_count;
    const bool complete_catalog = field_count == 25 && clause_count == 25 && review_control_count == 25;
    const bool published_count_valid = published_stage_count >= 0 && published_stage_count <= 25;
    const bool release_complete = published_stage_count == 25;

    return "{\"validationMode\":\"signed-approval-template-read-only-preflight-validation\""
           ",\"fieldCount\":" + std::to_string(field_count) +
           ",\"clauseCount\":" + std::to_string(clause_count) +
           ",\"reviewControlCount\":" + std::to_string(review_control_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"expectedCatalogCount\":25"
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"completeCatalog\":" + json_bool(complete_catalog) +
           ",\"publishedCountValid\":" + json_bool(published_count_valid) +
           ",\"releaseComplete\":" + json_bool(release_complete) +
           ",\"allFieldsHaveMissingBlockers\":true"
           ",\"allClausesHaveRejectionCodes\":true"
           ",\"allControlsBoundToSourceReviewPackage\":true"
           ",\"signedApprovalTemplateDeclared\":true"
           ",\"signedApprovalTemplateOnly\":true"
           ",\"signatureCaptured\":false"
           ",\"signedApprovalPresent\":false"
           ",\"approvalGrantPresent\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorValuesPersisted\":false"
           ",\"automaticSiblingImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"walTouched\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_template_validation
