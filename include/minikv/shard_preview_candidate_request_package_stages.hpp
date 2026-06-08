#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_candidate_request_package_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord> candidate_document_request_package_stages();
int first_candidate_document_request_package_release_number();
int planned_candidate_document_request_package_stage_count();

} // namespace minikv::shard_preview_candidate_request_package_stages
