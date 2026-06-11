#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks {
namespace {

constexpr std::array<ProductionShardExecutionMiniKvOwnerReceiptBundleCheck, 12> checks{{
    {1, "source-v1596-request-packet-observed", "v1596-owner-receipt-request-packet",
     "mini-kv receipts are derived from the exact request packet and do not invent new slots"},
    {2, "mini-kv-abort-owner-receipt-present", "mini-kv.abort-owner-receipt",
     "abort ownership is recorded as mini-kv-owned evidence"},
    {3, "mini-kv-abort-no-execution", "mini-kv.abort-owner-receipt",
     "abort ownership evidence does not execute abort behavior"},
    {4, "mini-kv-rollback-owner-receipt-present", "mini-kv.rollback-owner-receipt",
     "rollback ownership is recorded as mini-kv-owned evidence"},
    {5, "mini-kv-rollback-no-execution", "mini-kv.rollback-owner-receipt",
     "rollback ownership evidence does not deploy rollback behavior"},
    {6, "mini-kv-lifecycle-owner-receipt-present", "mini-kv.lifecycle-owner-receipt",
     "runtime lifecycle ownership is recorded without changing lifecycle state"},
    {7, "mini-kv-lifecycle-no-service-start", "mini-kv.lifecycle-owner-receipt",
     "lifecycle ownership evidence does not start Node Java or mini-kv services"},
    {8, "mini-kv-cleanup-no-start-proof-present", "mini-kv.cleanup-no-start-proof",
     "cleanup no-start proof is recorded as a local mini-kv receipt"},
    {9, "mini-kv-cleanup-no-execution", "mini-kv.cleanup-no-start-proof",
     "cleanup proof does not delete files compact storage restore data or mutate archives"},
    {10, "sibling-receipts-remain-absent", "java-and-cross-project-owner-receipts",
     "mini-kv receipt completion does not fabricate Java or cross-project receipts"},
    {11, "production-authority-remains-closed", "production-shard-execution-authority",
     "owner receipt evidence does not open production shard execution routing writes WAL or audit binding"},
    {12, "f-root-per-version-archive-required", "f-root-command-archive",
     "each v1597-v1601 release writes screenshots and explanations under its own f-root version directory"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const ProductionShardExecutionMiniKvOwnerReceiptBundleCheck>
production_shard_execution_mini_kv_owner_receipt_bundle_checks() {
    return checks;
}

int planned_production_shard_execution_mini_kv_owner_receipt_bundle_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds mini-kv owner receipt bundle checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"receiptSlot\":" + json_string(check.receipt_slot) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"miniKvReceiptEvidence\":true"
                  ",\"javaReceiptPresent\":false"
                  ",\"crossProjectReceiptPresent\":false"
                  ",\"abortExecutionPerformed\":false"
                  ",\"rollbackExecutionPerformed\":false"
                  ",\"lifecycleServiceStarted\":false"
                  ",\"cleanupExecutionPerformed\":false"
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

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks
