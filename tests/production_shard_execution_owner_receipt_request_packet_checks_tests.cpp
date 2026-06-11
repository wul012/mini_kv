#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    namespace checks =
        minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_checks;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;

    const auto records = checks::production_shard_execution_owner_receipt_request_packet_checks();
    assert(records.size() == 12);
    assert(checks::planned_production_shard_execution_owner_receipt_request_packet_check_count() == 12);

    assert(records.front().sequence == 1);
    assert(records.front().check_code == "node-v2087-plan-observed-only");
    assert(records.front().requested_receipt_slot == "node-v2087-owner-receipt-request-packet");
    assert(records.back().sequence == 12);
    assert(records.back().check_code == "per-version-archive-required");
    assert(records.back().requested_receipt_slot == "per-version-command-archive");

    const auto json = checks::format_production_shard_execution_owner_receipt_request_packet_checks_json(12);
    assert_contains(json, "\"checkCode\":\"mini-kv-abort-receipt-request\"");
    assert_contains(json, "\"requestedReceiptSlot\":\"java.lifecycle-owner-receipt\"");
    assert_contains(json, "\"requestedReceiptSlot\":\"cross-project.cleanup-reconciliation-receipt\"");
    assert_contains(json, "\"receiptRequestOnly\":true");
    assert_contains(json, "\"signedReceiptPresent\":false");
    assert_contains(json, "\"productionShardExecutionEnabled\":false");
    assert_contains(json, "\"managedAuditStoreBound\":false");
    assert_contains(json, "\"startsNodeServices\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_not_contains(json, "\"executionAllowed\":true");

    const auto partial = checks::format_production_shard_execution_owner_receipt_request_packet_checks_json(3);
    assert_contains(partial, "\"checkCode\":\"mini-kv-abort-receipt-request\"");
    assert_not_contains(partial, "\"checkCode\":\"mini-kv-rollback-receipt-request\"");

    bool threw = false;
    try {
        (void)checks::format_production_shard_execution_owner_receipt_request_packet_checks_json(13);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
