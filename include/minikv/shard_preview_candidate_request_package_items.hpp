#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_request_package_items {

struct CandidateRequestItem {
    int sequence;
    std::string_view item_code;
    std::string_view requested_field_group;
    std::string_view operator_instruction;
};

std::span<const CandidateRequestItem> candidate_document_request_items();
int planned_candidate_document_request_item_count();
std::string format_candidate_document_request_items_json(int completed_item_count);

} // namespace minikv::shard_preview_candidate_request_package_items
