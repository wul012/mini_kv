#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks.hpp"

#include "test_support.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    namespace checks =
        minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;

    const auto records = checks::production_shard_execution_external_artifact_dry_run_closeout_checks();
    assert(records.size() == 12);
    assert(checks::planned_production_shard_execution_external_artifact_dry_run_closeout_check_count() == 12);
    assert(records.front().check_code == "source-v1601-mini-kv-owner-receipt-bundle-observed");
    assert(records.back().check_code == "f-root-per-version-archive-required");

    const auto json = checks::format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(12);
    assert_contains(json, "\"artifactSlot\":\"external-artifact\"");
    assert_contains(json, "\"artifactSlot\":\"real-signed-approval\"");
    assert_contains(json, "\"artifactSlot\":\"managed-audit-owner-binding\"");
    assert_contains(json, "\"artifactSlot\":\"java-mini-kv-cleanup-real-receipts\"");
    assert_contains(json, "\"dryRunCloseoutEvidence\":true");
    assert_contains(json, "\"realExternalArtifactPresent\":false");
    assert_contains(json, "\"externalArtifactParsed\":false");
    assert_contains(json, "\"externalArtifactStored\":false");
    assert_contains(json, "\"dryRunArtifactExecuted\":false");
    assert_contains(json, "\"realSignedApprovalPresent\":false");
    assert_contains(json, "\"realMiniKvSignedReceiptPresent\":false");
    assert_contains(json, "\"managedAuditStoreBound\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_not_contains(json, "\"executionAllowed\":true");

    const auto partial = checks::format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(3);
    assert_contains(partial, "\"checkCode\":\"node-only-growth-stop-recorded\"");
    assert_not_contains(partial, "\"checkCode\":\"real-external-artifact-missing\"");

    bool threw = false;
    try {
        (void)checks::format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(13);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
