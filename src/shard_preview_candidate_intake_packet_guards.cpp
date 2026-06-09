#include "minikv/shard_preview_candidate_intake_packet_guards.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_intake_packet_guards {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<IntakePacketGuard, 10> guard_catalog = {{
    {1, "real-material-required-but-absent", "real candidate document intake",
     "reviewed material must be supplied externally before intake can proceed"},
    {2, "synthetic-material-rejected", "synthetic candidate document acceptance",
     "synthetic material cannot satisfy the reviewed-material gate"},
    {3, "payload-import-blocked", "candidate payload import",
     "payload import remains outside this read-only packet"},
    {4, "candidate-evaluation-blocked", "candidate evaluation acceptance and rejection",
     "evaluation cannot run before reviewed material is present"},
    {5, "approval-capture-blocked", "approval grant capture",
     "approval evidence is not captured by the intake packet"},
    {6, "signature-processing-blocked", "signature emission parsing and verification",
     "signature work waits for a later reviewed-material path"},
    {7, "document-router-blocked", "document router installation",
     "no active router is installed for candidate material"},
    {8, "write-routing-blocked", "write routing and store mutation",
     "the packet remains metadata only"},
    {9, "wal-touch-blocked", "WAL load restore and compaction",
     "storage maintenance paths stay out of intake packet review"},
    {10, "no-extra-intake-echo", "extra intake packet echoes",
     "the chain stops until actual reviewed material is supplied"},
}};

std::string format_guard_json(const IntakePacketGuard& guard) {
    return "{\"sequence\":" + std::to_string(guard.sequence) +
           ",\"guardCode\":" + json_string(guard.guard_code) +
           ",\"blocks\":" + json_string(guard.blocks) +
           ",\"reason\":" + json_string(guard.reason) +
           ",\"readOnly\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidatePayloadImported\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const IntakePacketGuard> candidate_intake_packet_guards() {
    return {guard_catalog.data(), guard_catalog.size()};
}

int planned_candidate_intake_packet_guard_count() {
    return static_cast<int>(guard_catalog.size());
}

std::string format_candidate_intake_packet_guards_json(int published_guard_count) {
    if (published_guard_count < 0 || static_cast<std::size_t>(published_guard_count) > guard_catalog.size()) {
        throw std::out_of_range{"published intake packet guard count exceeds guard catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_guard_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_guard_json(guard_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_intake_packet_guards
