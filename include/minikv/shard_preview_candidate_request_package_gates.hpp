#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_request_package_gates {

struct CandidateRequestGate {
    int sequence;
    std::string_view gate_code;
    std::string_view blocks;
    std::string_view reason;
};

std::span<const CandidateRequestGate> candidate_document_request_gates();
int planned_candidate_document_request_gate_count();
std::string format_candidate_document_request_gates_json(int completed_gate_count);

} // namespace minikv::shard_preview_candidate_request_package_gates
