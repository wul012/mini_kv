#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation.hpp"

#include "test_support.hpp"

#include <string>

namespace {

std::string validation_json(int source_count = 5,
                            bool source_complete = true,
                            bool source_validation = true,
                            bool abort_receipt = true,
                            bool rollback_receipt = true,
                            bool lifecycle_receipt = true,
                            bool cleanup_receipt = true,
                            bool bundle_only = true,
                            int slot_count = 4,
                            bool java_receipts = false,
                            bool cleanup_reconciliation = false,
                            bool per_version_archive = true,
                            bool archive_root_f = true,
                            bool combined_archive = false,
                            bool abort_execution = false,
                            bool rollback_execution = false,
                            bool lifecycle_start = false,
                            bool cleanup_execution = false,
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
                            int planned_checks = 12,
                            int completed_checks = 12,
                            int published_stages = 5,
                            int planned_stages = 5) {
    return minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation::
        format_production_shard_execution_mini_kv_owner_receipt_bundle_validation_json(
            source_count,
            source_complete,
            source_validation,
            abort_receipt,
            rollback_receipt,
            lifecycle_receipt,
            cleanup_receipt,
            bundle_only,
            slot_count,
            java_receipts,
            cleanup_reconciliation,
            per_version_archive,
            archive_root_f,
            combined_archive,
            abort_execution,
            rollback_execution,
            lifecycle_start,
            cleanup_execution,
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
            planned_checks,
            completed_checks,
            published_stages,
            planned_stages);
}

void assert_passes(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");
}

void assert_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = validation_json();
    assert_passes(valid);
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"miniKvReceiptsComplete\":true");
    assert_contains(valid, "\"externalReceiptsAbsent\":true");
    assert_contains(valid, "\"archiveSplitClosed\":true");
    assert_contains(valid, "\"noRuntimeReceiptExecution\":true");
    assert_contains(valid, "\"authorityBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");

    assert_fails(validation_json(4));
    assert_fails(validation_json(5, false));
    assert_fails(validation_json(5, true, false));
    assert_fails(validation_json(5, true, true, false));
    assert_fails(validation_json(5, true, true, true, false));
    assert_fails(validation_json(5, true, true, true, true, false));
    assert_fails(validation_json(5, true, true, true, true, true, false));
    assert_fails(validation_json(5, true, true, true, true, true, true, false));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 3));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, false));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, false));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, true));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 11));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 12, 11));
    assert_fails(validation_json(5, true, true, true, true, true, true, true, 4, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, 12, 12, 4));

    return 0;
}
