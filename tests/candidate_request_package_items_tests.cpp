#include "minikv/shard_preview_candidate_request_package_items.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace items = minikv::shard_preview_candidate_request_package_items;
    const auto records = items::candidate_document_request_items();

    assert(records.size() == 15);
    assert(items::planned_candidate_document_request_item_count() == 15);
    assert(records.front().item_code == std::string{"source_intake_contract_freeze"});
    assert(records.back().item_code == std::string{"archive_closeout_reference"});

    const auto none = items::format_candidate_document_request_items_json(0);
    assert(none == "[]");

    const auto first = items::format_candidate_document_request_items_json(1);
    assert_contains(first, "\"itemCode\":\"source_intake_contract_freeze\"");
    assert(first.find("archive_closeout_reference") == std::string::npos);

    const auto all = items::format_candidate_document_request_items_json(99);
    assert_contains(all, "\"itemCode\":\"archive_closeout_reference\"");
    assert_contains(all, "\"documentMaterialAccepted\":false");
    assert_contains(all, "\"payloadImported\":false");
    return 0;
}
