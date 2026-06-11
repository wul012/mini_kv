#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_validation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

std::string validation_json(int source_stage_count = 6,
                            bool source_complete = true,
                            bool node_waits = false,
                            bool node_requires_existing_receipts = false,
                            bool mini_kv_receipt_present = false,
                            bool java_receipt_present = false,
                            bool cleanup_receipt_present = false,
                            bool request_packet_only = true,
                            int slot_count = 9,
                            bool per_version_archive_required = true,
                            bool combined_archive_allowed = false,
                            bool production_execution = false,
                            bool managed_audit = false,
                            bool raw_endpoint = false,
                            bool credential_value = false,
                            bool starts_node = false,
                            bool starts_java = false,
                            bool starts_mini_kv = false,
                            bool active_router = false,
                            bool write_routing = false,
                            bool mutates_store = false,
                            bool touches_wal = false,
                            bool execution_allowed = false,
                            int planned_check_count = 12,
                            int completed_check_count = 12,
                            int published_stage_count = 5,
                            int planned_stage_count = 5) {
    return minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_validation::
        format_production_shard_execution_owner_receipt_request_packet_validation_json(
            source_stage_count,
            source_complete,
            node_waits,
            node_requires_existing_receipts,
            mini_kv_receipt_present,
            java_receipt_present,
            cleanup_receipt_present,
            request_packet_only,
            slot_count,
            per_version_archive_required,
            combined_archive_allowed,
            production_execution,
            managed_audit,
            raw_endpoint,
            credential_value,
            starts_node,
            starts_java,
            starts_mini_kv,
            active_router,
            write_routing,
            mutates_store,
            touches_wal,
            execution_allowed,
            planned_check_count,
            completed_check_count,
            published_stage_count,
            planned_stage_count);
}

void assert_passes(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");
}

void assert_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = validation_json();
    assert_passes(valid);
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"nodePlanAllowsParallel\":true");
    assert_contains(valid, "\"signedReceiptsAbsent\":true");
    assert_contains(valid, "\"requestScopeClosed\":true");
    assert_contains(valid, "\"archiveSplitClosed\":true");
    assert_contains(valid, "\"authorityBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");
    assert_contains(valid, "\"readOnly\":true");

    assert_fails(validation_json(5));
    assert_fails(validation_json(6, false));
    assert_fails(validation_json(6, true, true));
    assert_fails(validation_json(6, true, false, true));
    assert_fails(validation_json(6, true, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, false));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 8));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, false));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, false, false, 11));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, false, false, 12, 11));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, false, false, 12, 12, 4));
    assert_fails(validation_json(6, true, false, false, false, false, false, true, 9, true, false, false, false, false, false, false, false, false, false, false, false, false, 12, 12, 5, 6));

    return 0;
}
