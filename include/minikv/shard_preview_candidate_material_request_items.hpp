#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_material_request_items {

struct MaterialRequestItem {
    int sequence;
    std::string_view request_code;
    std::string_view requested_material;
    std::string_view purpose;
};

std::span<const MaterialRequestItem> candidate_material_request_items();
int planned_candidate_material_request_item_count();
std::string format_candidate_material_request_items_json(int published_item_count);

} // namespace minikv::shard_preview_candidate_material_request_items
