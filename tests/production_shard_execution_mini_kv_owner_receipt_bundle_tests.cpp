#include "minikv/command.hpp"
#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp"
#include "minikv/store.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace bundle = minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;
    using minikv::test_support::assert_response_contains;

    const auto json = bundle::format_production_shard_execution_mini_kv_owner_receipt_bundle_json();
    assert_contains(json, "\"contract\":\"shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON\"");
    assert_contains(json, "\"sourceOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1600\"");
    assert_contains(json, "\"miniKvReceiptSlotCount\":4");
    assert_contains(json, "\"miniKvReceiptsComplete\":true");
    assert_contains(json, "\"javaOwnerReceiptsPresent\":false");
    assert_contains(json, "\"crossProjectCleanupReconciliationReceiptPresent\":false");
    assert_contains(json, "\"archiveRoot\":\"f\"");
    assert_contains(json, "\"archiveRootIsF\":true");
    assert_contains(json, "\"versionedArchiveDirectories\":[\"f/1597/\",\"f/1598/\",\"f/1599/\",\"f/1600/\",\"f/1601/\"]");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");
    assert_contains(json, "\"abortExecutionPerformed\":false");
    assert_contains(json, "\"rollbackExecutionPerformed\":false");
    assert_contains(json, "\"lifecycleServiceStarted\":false");
    assert_contains(json, "\"cleanupExecutionPerformed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_not_contains(json, "\"executionAllowed\":true");
    assert_not_contains(json, "\"touchesWal\":true");

    assert(bundle::production_shard_execution_mini_kv_owner_receipt_bundle_status() ==
           "production-shard-execution-mini-kv-owner-receipt-bundle-clean-ci-closeout-read-only");
    assert(bundle::production_shard_execution_mini_kv_owner_receipt_bundle_digest_marker().find(
               "v1601-production-shard-execution-mini-kv-owner-receipt-bundle-clean-ci-closeout") !=
           std::string::npos);

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    auto result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON");
    assert_response_contains(result, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON extra");
    assert_response_contains(result, "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON");

    return 0;
}
