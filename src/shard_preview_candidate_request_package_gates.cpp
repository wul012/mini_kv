#include "minikv/shard_preview_candidate_request_package_gates.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>

namespace minikv::shard_preview_candidate_request_package_gates {
namespace {

constexpr std::array<CandidateRequestGate, 38> request_gates = {{
    {1, "source-intake-fixture-gate", "rolling current used as frozen source", "the request package must cite v880 as a versioned fixture"},
    {2, "source-intake-digest-gate", "source digest drift", "future consumers need a stable source digest reference"},
    {3, "field-count-gate", "candidate field count other than twenty", "Node v1386 requests exactly twenty candidate fields"},
    {4, "request-item-count-gate", "request item count other than fifteen", "request package completeness depends on fifteen items"},
    {5, "acceptance-check-count-gate", "acceptance check count other than fifteen", "review completeness depends on fifteen checks"},
    {6, "gate-count-gate", "gate count other than thirty-eight", "closeout states the full guard inventory explicitly"},
    {7, "real-document-presence-gate", "real candidate document accepted early", "this version is a request package only"},
    {8, "missing-document-gate", "missing document treated as ready", "absence must remain blocked"},
    {9, "synthetic-document-gate", "synthetic document accepted", "fabricated candidate material cannot satisfy the request"},
    {10, "unreviewed-document-gate", "unreviewed document accepted", "manual review must happen in a later intake"},
    {11, "payload-import-gate", "staged payload imported", "request fields are not runtime values"},
    {12, "payload-materialization-gate", "candidate payload materialized", "materialization belongs to a later approved path"},
    {13, "evaluation-gate", "candidate evaluated", "evaluation is not part of request package evidence"},
    {14, "acceptance-gate", "candidate accepted", "acceptance is disabled until real intake"},
    {15, "rejection-gate", "candidate rejected as runtime action", "the package only records review prerequisites"},
    {16, "approval-grant-gate", "approval grant captured", "approval capture remains separated"},
    {17, "signed-approval-gate", "signed approval emitted", "signature emission is out of scope"},
    {18, "signature-parse-gate", "detached signature parsed", "signature references are handles only"},
    {19, "signature-verify-gate", "detached signature verified", "verification execution stays disabled"},
    {20, "operator-value-gate", "operator value imported", "operator values stay at zero"},
    {21, "credential-value-gate", "credential value stored", "credential values are never consumed by mini-kv evidence"},
    {22, "raw-endpoint-gate", "raw endpoint stored", "endpoint references remain handles only"},
    {23, "raw-signature-gate", "raw signature material stored", "raw signature bytes stay absent"},
    {24, "runtime-payload-gate", "runtime payload imported", "runtime payloads are blocked"},
    {25, "runtime-execution-gate", "runtime execution enabled", "the evidence cannot execute"},
    {26, "router-activation-gate", "active shard router installed", "route preview remains inactive"},
    {27, "write-routing-gate", "write routing enabled", "request package evidence is read-only"},
    {28, "write-command-gate", "write command allowed", "write commands are not part of this command"},
    {29, "wal-touch-gate", "WAL touched", "the command must not append storage state"},
    {30, "load-restore-compact-gate", "LOAD RESTORE or COMPACT executed", "storage recovery actions remain disabled"},
    {31, "filesystem-read-gate", "runtime filesystem read performed", "versioned fixtures are cited, not read at runtime"},
    {32, "archive-walk-gate", "runtime archive walk performed", "archive references are metadata only"},
    {33, "service-start-gate", "mini-kv service started", "request evidence does not start services"},
    {34, "sibling-start-gate", "Node or Java service started", "sibling projects remain untouched"},
    {35, "sibling-mutation-gate", "sibling evidence mutated", "this C++ version cannot edit Node or Java state"},
    {36, "approval-policy-gate", "policy lock bypassed", "policy lock handles are prerequisites only"},
    {37, "document-router-gate", "document router opened", "there is no document router in this package"},
    {38, "closeout-read-only-gate", "closeout enables action", "closeout summarizes readiness without opening action paths"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const CandidateRequestGate> candidate_document_request_gates() {
    return {request_gates.data(), request_gates.size()};
}

int planned_candidate_document_request_gate_count() {
    return static_cast<int>(request_gates.size());
}

std::string format_candidate_document_request_gates_json(int completed_gate_count) {
    const int visible_count = std::clamp(completed_gate_count, 0, planned_candidate_document_request_gate_count());
    std::string json = "[";
    for (int index = 0; index < visible_count; ++index) {
        const auto& gate = request_gates[static_cast<std::size_t>(index)];
        if (index != 0) {
            json += ",";
        }
        json += "{\"sequence\":" + std::to_string(gate.sequence) +
                ",\"gateCode\":" + json_string(gate.gate_code) +
                ",\"blocks\":" + json_string(gate.blocks) +
                ",\"reason\":" + json_string(gate.reason) +
                ",\"passed\":true"
                ",\"gateBypassed\":false"
                ",\"executionAllowed\":false}";
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_candidate_request_package_gates
