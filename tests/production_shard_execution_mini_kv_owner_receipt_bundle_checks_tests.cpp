#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    namespace checks = minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;

    const auto records = checks::production_shard_execution_mini_kv_owner_receipt_bundle_checks();
    assert(records.size() == 12);
    assert(checks::planned_production_shard_execution_mini_kv_owner_receipt_bundle_check_count() == 12);
    assert(records.front().check_code == "source-v1596-request-packet-observed");
    assert(records.back().check_code == "f-root-per-version-archive-required");

    const auto json = checks::format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(12);
    assert_contains(json, "\"receiptSlot\":\"mini-kv.abort-owner-receipt\"");
    assert_contains(json, "\"receiptSlot\":\"mini-kv.rollback-owner-receipt\"");
    assert_contains(json, "\"receiptSlot\":\"mini-kv.lifecycle-owner-receipt\"");
    assert_contains(json, "\"receiptSlot\":\"mini-kv.cleanup-no-start-proof\"");
    assert_contains(json, "\"miniKvReceiptEvidence\":true");
    assert_contains(json, "\"javaReceiptPresent\":false");
    assert_contains(json, "\"crossProjectReceiptPresent\":false");
    assert_contains(json, "\"abortExecutionPerformed\":false");
    assert_contains(json, "\"rollbackExecutionPerformed\":false");
    assert_contains(json, "\"lifecycleServiceStarted\":false");
    assert_contains(json, "\"cleanupExecutionPerformed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_not_contains(json, "\"executionAllowed\":true");

    const auto partial = checks::format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(2);
    assert_contains(partial, "\"checkCode\":\"mini-kv-abort-owner-receipt-present\"");
    assert_not_contains(partial, "\"checkCode\":\"mini-kv-abort-no-execution\"");

    bool threw = false;
    try {
        (void)checks::format_production_shard_execution_mini_kv_owner_receipt_bundle_checks_json(13);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
