#include "minikv/shard_preview_candidate_material_request_items.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_material_request_items {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<MaterialRequestItem, 25> request_catalog = {{
    {1, "reviewer-identity", "human reviewer identity handle", "binds later material to a reviewed source"},
    {2, "source-uri-handle", "source URI handle", "names the source without resolving a raw endpoint"},
    {3, "source-uri-ownership", "source ownership attestation", "keeps provenance review explicit"},
    {4, "source-observed-at", "source observation timestamp", "anchors when the reviewer saw the material"},
    {5, "document-digest", "reviewed document digest", "prepares later digest verification without reading content"},
    {6, "digest-algorithm", "digest algorithm name", "prevents ambiguous hash comparisons"},
    {7, "canonical-body", "canonical document body", "requests the body but keeps it absent in mini-kv"},
    {8, "canonical-field-table", "canonical requested field table", "maps twenty fields without parsing values"},
    {9, "comparison-binding", "comparison binding reference", "ties material to the earlier request package"},
    {10, "signature-attestation", "signature or reviewer attestation", "requests approval evidence without verifying it"},
    {11, "redaction-log", "redaction log", "keeps redaction review separate from storage writes"},
    {12, "absence-attestations", "absence attestations", "records missing optional material explicitly"},
    {13, "archive-index", "archive index reference", "points to evidence without walking archives at runtime"},
    {14, "closeout-request-wording", "closeout request wording", "keeps reviewer-facing wording auditable"},
    {15, "operator-identity", "operator identity handle", "prepares later operator correlation"},
    {16, "approval-correlation", "approval correlation marker", "keeps approval linkage non-executable"},
    {17, "provenance-note", "material provenance note", "requests provenance context before intake"},
    {18, "retention-marker", "retention marker", "prepares archive retention without creating data"},
    {19, "fixture-lineage", "source fixture lineage", "binds request to frozen v955 intake packet"},
    {20, "source-intake-slots", "ten source intake slots", "carries Node v1421 slot count as metadata"},
    {21, "source-intake-guards", "ten source intake guards", "carries Node v1421 guard count as metadata"},
    {22, "material-format", "expected material format", "requests shape before accepting payloads"},
    {23, "validation-summary", "reviewer validation summary", "asks for summary without evaluating candidates"},
    {24, "rollback-pointer", "rollback pointer", "keeps abort path visible"},
    {25, "cleanup-proof", "cleanup proof", "prepares later cleanup evidence without starting services"},
}};

std::string format_request_json(const MaterialRequestItem& item) {
    return "{\"sequence\":" + std::to_string(item.sequence) +
           ",\"requestCode\":" + json_string(item.request_code) +
           ",\"requestedMaterial\":" + json_string(item.requested_material) +
           ",\"purpose\":" + json_string(item.purpose) +
           ",\"requestOnly\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"materialAccepted\":false"
           ",\"materialImported\":false"
           ",\"candidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"documentRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const MaterialRequestItem> candidate_material_request_items() {
    return {request_catalog.data(), request_catalog.size()};
}

int planned_candidate_material_request_item_count() {
    return static_cast<int>(request_catalog.size());
}

std::string format_candidate_material_request_items_json(int published_item_count) {
    if (published_item_count < 0 || static_cast<std::size_t>(published_item_count) > request_catalog.size()) {
        throw std::out_of_range{"published material request item count exceeds request catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_item_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_request_json(request_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_material_request_items
