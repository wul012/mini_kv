#include "minikv/shard_preview_candidate_material_request_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_material_request_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<MaterialAcceptanceCheck, 25> check_catalog = {{
    {1, "reviewer-identity-present", "reviewer identity handle is supplied", "reviewer identity"},
    {2, "source-uri-handle-present", "source URI handle is supplied", "source URI handle"},
    {3, "source-ownership-present", "source ownership attestation is supplied", "ownership attestation"},
    {4, "source-observed-at-present", "source observation timestamp is supplied", "observation timestamp"},
    {5, "document-digest-present", "document digest is supplied", "document digest"},
    {6, "digest-algorithm-present", "digest algorithm is named", "digest algorithm"},
    {7, "canonical-body-present", "canonical body is supplied", "canonical body"},
    {8, "field-table-present", "canonical field table is supplied", "field table"},
    {9, "comparison-binding-present", "comparison binding is supplied", "comparison binding"},
    {10, "signature-attestation-present", "signature or reviewer attestation is supplied", "attestation"},
    {11, "redaction-log-present", "redaction log is supplied", "redaction log"},
    {12, "absence-attestations-present", "absence attestations are supplied", "absence attestations"},
    {13, "archive-index-present", "archive index reference is supplied", "archive index"},
    {14, "closeout-wording-present", "closeout request wording is supplied", "closeout wording"},
    {15, "operator-identity-present", "operator identity handle is supplied", "operator identity"},
    {16, "approval-correlation-present", "approval correlation marker is supplied", "approval correlation"},
    {17, "provenance-note-present", "provenance note is supplied", "provenance note"},
    {18, "retention-marker-present", "retention marker is supplied", "retention marker"},
    {19, "fixture-lineage-present", "fixture lineage is supplied", "fixture lineage"},
    {20, "source-intake-slots-aligned", "ten source intake slots remain aligned", "slot alignment"},
    {21, "source-intake-guards-aligned", "ten source intake guards remain aligned", "guard alignment"},
    {22, "material-format-present", "expected material format is supplied", "material format"},
    {23, "validation-summary-present", "reviewer validation summary is supplied", "validation summary"},
    {24, "rollback-pointer-present", "rollback pointer is supplied", "rollback pointer"},
    {25, "cleanup-proof-present", "cleanup proof is supplied", "cleanup proof"},
}};

std::string format_check_json(const MaterialAcceptanceCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"verifies\":" + json_string(check.verifies) +
           ",\"blockedUntilPresent\":" + json_string(check.blocked_until_present) +
           ",\"checkOnly\":true"
           ",\"materialPresent\":false"
           ",\"checkSatisfiedByRuntime\":false"
           ",\"materialAccepted\":false"
           ",\"materialImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const MaterialAcceptanceCheck> candidate_material_acceptance_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_candidate_material_acceptance_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_candidate_material_acceptance_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{"published material acceptance check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_material_request_checks
