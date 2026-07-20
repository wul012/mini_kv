#include "minikv/shard_preview_candidate_material_request_items.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace items = minikv::shard_preview_candidate_material_request_items;

    const auto catalog = items::candidate_material_request_items();
    assert(catalog.size() == 25);
    assert(items::planned_candidate_material_request_item_count() == 25);
    assert(catalog.front().request_code == std::string{"reviewer-identity"});
    assert(catalog.back().request_code == std::string{"cleanup-proof"});

    const auto partial_json = items::format_candidate_material_request_items_json(2);
    assert_contains(partial_json, "\"requestCode\":\"reviewer-identity\"");
    assert_contains(partial_json, "\"requestCode\":\"source-uri-handle\"");
    assert(partial_json.find("\"requestCode\":\"source-uri-ownership\"") == std::string::npos);

    const auto full_json = items::format_candidate_material_request_items_json(25);
    assert_contains(full_json, "\"requestCode\":\"canonical-body\"");
    assert_contains(full_json, "\"requestCode\":\"source-intake-guards\"");
    assert_contains(full_json, "\"requestCode\":\"cleanup-proof\"");
    assert_contains(full_json, "\"reviewedMaterialPresent\":false");
    assert_contains(full_json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(full_json, "\"materialImported\":false");
    assert_contains(full_json, "\"candidateDocumentEvaluated\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)items::format_candidate_material_request_items_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
