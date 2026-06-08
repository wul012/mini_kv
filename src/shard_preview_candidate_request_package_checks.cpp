#include "minikv/shard_preview_candidate_request_package_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>

namespace minikv::shard_preview_candidate_request_package_checks {
namespace {

constexpr std::array<CandidateAcceptanceCheck, 15> acceptance_checks = {{
    {1, "source_intake_contract_check", "v880 intake audit release fixture and digest are frozen", "source-intake-unfrozen"},
    {2, "required_candidate_field_check", "twenty requested candidate fields are declared", "candidate-fields-missing"},
    {3, "operator_instruction_check", "operator handoff fields are handles only", "instruction-packet-opens-runtime"},
    {4, "identity_provenance_check", "identity and provenance are handle based", "identity-secret-supplied"},
    {5, "digest_lineage_check", "digest lineage is referenced without payload bytes", "payload-bytes-imported"},
    {6, "signature_envelope_reference_check", "signature envelope remains unparsed", "signature-payload-parsed"},
    {7, "policy_lock_check", "policy locks keep execution disabled", "execution-policy-unlocked"},
    {8, "approval_separation_check", "approval review is separated from capture", "approval-grant-captured"},
    {9, "missing_document_check", "real candidate document absence stays blocked", "missing-document-accepted"},
    {10, "synthetic_document_check", "synthetic candidate material stays rejected", "synthetic-document-accepted"},
    {11, "payload_import_check", "staged payload import remains denied", "payload-imported"},
    {12, "evaluation_acceptance_check", "candidate evaluation acceptance and rejection stay disabled", "candidate-evaluated"},
    {13, "runtime_write_check", "runtime payload write WAL and execution boundaries remain closed", "runtime-write-opened"},
    {14, "sibling_mutation_check", "sibling project and archive mutation remain disabled", "sibling-mutated"},
    {15, "archive_closeout_check", "archive references are recorded without runtime archive walk", "archive-walk-executed"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const CandidateAcceptanceCheck> candidate_document_acceptance_checks() {
    return {acceptance_checks.data(), acceptance_checks.size()};
}

int planned_candidate_document_acceptance_check_count() {
    return static_cast<int>(acceptance_checks.size());
}

std::string format_candidate_document_acceptance_checks_json(int completed_check_count) {
    const int visible_count =
        std::clamp(completed_check_count, 0, planned_candidate_document_acceptance_check_count());
    std::string json = "[";
    for (int index = 0; index < visible_count; ++index) {
        const auto& check = acceptance_checks[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"sequence\":" + std::to_string(check.sequence) +
                ",\"checkCode\":" + json_string(check.check_code) +
                ",\"verifies\":" + json_string(check.verifies) +
                ",\"failureState\":" + json_string(check.failure_state) +
                ",\"passed\":true"
                ",\"documentAccepted\":false"
                ",\"executionAllowed\":false}";
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_candidate_request_package_checks
