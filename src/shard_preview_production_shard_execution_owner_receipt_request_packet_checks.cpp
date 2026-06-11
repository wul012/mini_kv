#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_checks {
namespace {

constexpr std::array<ProductionShardExecutionOwnerReceiptRequestPacketCheck, 12> checks{{
    {1, "node-v2087-plan-observed-only", "node-v2087-owner-receipt-request-packet",
     "mini-kv records exact receipt slots without blocking Node progress or claiming receipts exist"},
    {2, "request-only-packet-boundary", "request-packet-only",
     "the packet names needed receipts but does not act as a signed owner receipt"},
    {3, "mini-kv-abort-receipt-request", "mini-kv.abort-owner-receipt",
     "future mini-kv evidence must prove abort ownership without executing abort"},
    {4, "mini-kv-rollback-receipt-request", "mini-kv.rollback-owner-receipt",
     "future mini-kv evidence must prove rollback ownership without deploying rollback"},
    {5, "mini-kv-lifecycle-owner-receipt-request", "mini-kv.lifecycle-owner-receipt",
     "future mini-kv evidence must identify runtime lifecycle ownership without starting services"},
    {6, "mini-kv-cleanup-no-start-proof-request", "mini-kv.cleanup-no-start-proof",
     "future mini-kv evidence must prove cleanup and no-start behavior separately from execution"},
    {7, "java-abort-rollback-owner-request", "java.abort-rollback-owner-receipt",
     "Java owner receipts are requested but not synthesized inside mini-kv"},
    {8, "java-lifecycle-owner-request", "java.lifecycle-owner-receipt",
     "Java lifecycle owner evidence remains Java-owned and absent until signed by that side"},
    {9, "cross-project-cleanup-reconciliation-request", "cross-project.cleanup-reconciliation-receipt",
     "cleanup reconciliation is requested as a separate cross-project receipt slot"},
    {10, "signed-receipt-absence-explicit", "signed-sibling-receipt-absence",
     "missing signed Java mini-kv and cleanup receipts keep the packet request-only"},
    {11, "production-authority-closed", "production-shard-execution-authority",
     "no owner receipt request opens production shard execution, routing, writes, WAL, or managed audit binding"},
    {12, "per-version-archive-required", "per-version-command-archive",
     "each release keeps screenshots and explanations in its own version directory"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const ProductionShardExecutionOwnerReceiptRequestPacketCheck>
production_shard_execution_owner_receipt_request_packet_checks() {
    return checks;
}

int planned_production_shard_execution_owner_receipt_request_packet_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_production_shard_execution_owner_receipt_request_packet_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds production shard execution owner receipt checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"requestedReceiptSlot\":" + json_string(check.requested_receipt_slot) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"receiptRequestOnly\":true"
                  ",\"signedReceiptPresent\":false"
                  ",\"productionShardExecutionEnabled\":false"
                  ",\"managedAuditStoreBound\":false"
                  ",\"rawEndpointParsed\":false"
                  ",\"credentialValueRead\":false"
                  ",\"startsNodeServices\":false"
                  ",\"startsJavaService\":false"
                  ",\"startsMiniKvService\":false"
                  ",\"activeRouterInstalled\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"mutatesStore\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_checks
