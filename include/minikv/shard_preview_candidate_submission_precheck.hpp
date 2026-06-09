#pragma once

#include <string>

namespace minikv::shard_preview_candidate_submission_precheck {

std::string format_candidate_submission_precheck_json();
std::string format_candidate_submission_precheck_checkpoint_catalog_json();
std::string candidate_submission_precheck_digest_marker();
std::string candidate_submission_precheck_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_submission_precheck
