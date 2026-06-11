#include "minikv/command.hpp"
#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp"
#include "minikv/store.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace packet = minikv::shard_preview_production_shard_execution_owner_receipt_request_packet;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;
    using minikv::test_support::assert_response_contains;

    const auto json = packet::format_production_shard_execution_owner_receipt_request_packet_json();
    assert_contains(json, "\"contract\":\"shard-production-shard-execution-owner-receipt-request-packet.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\"");
    assert_contains(json, "v2087-production-shard-execution-owner-receipt-request-packet-roadmap.md");
    assert_contains(json, "\"sourceNodeWaitsForMiniKv\":false");
    assert_contains(json, "\"sourceNodeRequiresExistingSignedReceipts\":false");
    assert_contains(json, "\"sourceNodeAllowsJavaMiniKvParallel\":true");
    assert_contains(json, "\"sourceCodeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1591\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1595\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeStart\":\"v1592\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeEnd\":\"v1596\"");
    assert_contains(json, "\"publishedStageCount\":5");
    assert_contains(json, "\"plannedStageCount\":5");
    assert_contains(json, "\"requestedReceiptSlotCount\":9");
    assert_contains(json, "\"ownerReceiptRequestPacketOnly\":true");
    assert_contains(json, "\"signedMiniKvReceiptPresent\":false");
    assert_contains(json, "\"signedJavaReceiptPresent\":false");
    assert_contains(json, "\"signedCrossProjectCleanupReceiptPresent\":false");
    assert_contains(json, "\"anySignedSiblingReceipt\":false");
    assert_contains(json, "\"perVersionArchiveRequired\":true");
    assert_contains(json, "\"combinedArchiveFolderAllowed\":false");
    assert_contains(json, "\"versionedArchiveDirectories\":[\"e/1592/\",\"e/1593/\",\"e/1594/\",\"e/1595/\",\"e/1596/\"]");
    assert_contains(json, "\"batchArchiveFolderUsed\":false");
    assert_contains(json, "\"completedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":12");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");
    assert_contains(json, "\"productionShardExecutionEnabled\":false");
    assert_contains(json, "\"managedAuditStoreBound\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"startsNodeServices\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_not_contains(json, "\"executionAllowed\":true");
    assert_not_contains(json, "\"mutatesStore\":true");
    assert_not_contains(json, "\"touchesWal\":true");

    const auto status = packet::production_shard_execution_owner_receipt_request_packet_status();
    assert(status == "production-shard-execution-owner-receipt-request-packet-clean-ci-closeout-read-only");
    const auto digest = packet::production_shard_execution_owner_receipt_request_packet_digest_marker();
    assert(digest.find("v1596-production-shard-execution-owner-receipt-request-packet-clean-ci-closeout") !=
           std::string::npos);

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    auto result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");
    assert_response_contains(result, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON extra");
    assert_response_contains(result, "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");

    return 0;
}
