#pragma once

#include <string>

namespace minikv::shard_preview_candidate_request_package {

std::string format_candidate_document_request_package_closeout_json();
std::string format_candidate_document_request_package_stage_catalog_json();
std::string candidate_document_request_package_digest_marker();
std::string candidate_document_request_package_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_request_package
