#pragma once

#include <string>

namespace minikv::shard_route_preview_operator_value_draft {

std::string format_value_draft_stage_catalog_json();
std::string format_value_draft_json();
std::string value_draft_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_operator_value_draft
