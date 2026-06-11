#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation.hpp"

#include "test_support.hpp"

#include <string>

namespace {

struct ValidationCase {
    int source_count = 5;
    bool source_complete = true;
    bool source_validation = true;
    bool dry_run_closed = true;
    bool node_growth_stopped = true;
    bool real_artifact = false;
    bool artifact_parsed = false;
    bool artifact_accepted = false;
    bool artifact_stored = false;
    bool artifact_executed = false;
    bool signed_approval = false;
    bool audit_binding = false;
    bool java_receipt = false;
    bool mini_kv_signed_receipt = false;
    bool cleanup_receipt = false;
    bool missing_blocker = true;
    bool per_version_archive = true;
    bool archive_root_f = true;
    bool combined_archive = false;
    bool production_execution = false;
    bool managed_audit = false;
    bool raw_endpoint = false;
    bool credential_value = false;
    bool starts_node = false;
    bool starts_java = false;
    bool starts_mini_kv = false;
    bool active_router = false;
    bool write_routing = false;
    bool mutates_store = false;
    bool touches_wal = false;
    bool execution_allowed = false;
    int planned_checks = 12;
    int completed_checks = 12;
    int published_stages = 5;
    int planned_stages = 5;
};

std::string validation_json(const ValidationCase& validation = {}) {
    return minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation::
        format_production_shard_execution_external_artifact_dry_run_closeout_validation_json(
            validation.source_count,
            validation.source_complete,
            validation.source_validation,
            validation.dry_run_closed,
            validation.node_growth_stopped,
            validation.real_artifact,
            validation.artifact_parsed,
            validation.artifact_accepted,
            validation.artifact_stored,
            validation.artifact_executed,
            validation.signed_approval,
            validation.audit_binding,
            validation.java_receipt,
            validation.mini_kv_signed_receipt,
            validation.cleanup_receipt,
            validation.missing_blocker,
            validation.per_version_archive,
            validation.archive_root_f,
            validation.combined_archive,
            validation.production_execution,
            validation.managed_audit,
            validation.raw_endpoint,
            validation.credential_value,
            validation.starts_node,
            validation.starts_java,
            validation.starts_mini_kv,
            validation.active_router,
            validation.write_routing,
            validation.mutates_store,
            validation.touches_wal,
            validation.execution_allowed,
            validation.planned_checks,
            validation.completed_checks,
            validation.published_stages,
            validation.planned_stages);
}

void assert_passes(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");
}

void assert_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = validation_json();
    assert_passes(valid);
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"dryRunCloseoutLocked\":true");
    assert_contains(valid, "\"realExternalArtifactAbsent\":true");
    assert_contains(valid, "\"realReceiptsMissing\":true");
    assert_contains(valid, "\"archiveSplitClosed\":true");
    assert_contains(valid, "\"authorityBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");

    auto broken = ValidationCase{};
    broken.source_count = 4;
    assert_fails(validation_json(broken));

    broken = {};
    broken.source_complete = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.source_validation = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.dry_run_closed = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.node_growth_stopped = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.real_artifact = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.artifact_parsed = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.artifact_accepted = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.artifact_stored = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.artifact_executed = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.signed_approval = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.audit_binding = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.java_receipt = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.mini_kv_signed_receipt = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.cleanup_receipt = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.missing_blocker = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.per_version_archive = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.archive_root_f = false;
    assert_fails(validation_json(broken));

    broken = {};
    broken.combined_archive = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.production_execution = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.managed_audit = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.raw_endpoint = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.credential_value = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_node = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_java = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.starts_mini_kv = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.active_router = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.write_routing = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.mutates_store = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.touches_wal = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.execution_allowed = true;
    assert_fails(validation_json(broken));

    broken = {};
    broken.planned_checks = 11;
    assert_fails(validation_json(broken));

    broken = {};
    broken.completed_checks = 11;
    assert_fails(validation_json(broken));

    broken = {};
    broken.published_stages = 4;
    assert_fails(validation_json(broken));

    broken = {};
    broken.planned_stages = 4;
    assert_fails(validation_json(broken));

    return 0;
}
