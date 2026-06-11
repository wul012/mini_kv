#include "minikv/command.hpp"
#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.hpp"
#include "minikv/store.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace closeout =
        minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout;
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_not_contains;
    using minikv::test_support::assert_response_contains;

    const auto json = closeout::format_production_shard_execution_external_artifact_dry_run_closeout_json();
    assert_contains(json, "\"contract\":\"shard-production-shard-execution-external-artifact-dry-run-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON\"");
    assert_contains(json, "\"sourceMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1605\"");
    assert_contains(json, "\"dryRunBatchClosed\":true");
    assert_contains(json, "\"nodeOnlyGrowthStopped\":true");
    assert_contains(json, "\"missingRealArtifactBlockerListed\":true");
    assert_contains(json, "\"realExternalArtifactPresent\":false");
    assert_contains(json, "\"externalArtifactParsed\":false");
    assert_contains(json, "\"externalArtifactAccepted\":false");
    assert_contains(json, "\"externalArtifactStored\":false");
    assert_contains(json, "\"dryRunArtifactExecuted\":false");
    assert_contains(json, "\"realSignedApprovalPresent\":false");
    assert_contains(json, "\"managedAuditOwnerBindingPresent\":false");
    assert_contains(json, "\"realJavaReceiptPresent\":false");
    assert_contains(json, "\"realMiniKvSignedReceiptPresent\":false");
    assert_contains(json, "\"realCleanupReceiptPresent\":false");
    assert_contains(json, "\"archiveRoot\":\"f\"");
    assert_contains(json, "\"versionedArchiveDirectories\":[\"f/1602/\",\"f/1603/\",\"f/1604/\",\"f/1605/\",\"f/1606/\"]");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");
    assert_contains(json, "\"productionShardExecutionEnabled\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_not_contains(json, "\"executionAllowed\":true");
    assert_not_contains(json, "\"touchesWal\":true");

    assert(closeout::production_shard_execution_external_artifact_dry_run_closeout_status() ==
           "production-shard-execution-external-artifact-dry-run-closeout-clean-ci-closeout-read-only");
    assert(closeout::production_shard_execution_external_artifact_dry_run_closeout_digest_marker().find(
               "v1606-production-shard-execution-external-artifact-dry-run-closeout-clean-ci-closeout") !=
           std::string::npos);

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    auto result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON");
    assert_response_contains(result,
                             "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");

    result = processor.execute("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON extra");
    assert_response_contains(result, "ERR usage: SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON");

    return 0;
}
